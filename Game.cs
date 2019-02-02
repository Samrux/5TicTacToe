using System;
using System.Collections.Generic;
using System.Linq;

namespace TTT5
{
    public class Game
    {
        /// <summary>RNG used for AI turns.</summary>
        public static readonly Random Random = new Random();

        /// <summary>The game's board.</summary>
        public Board<Player> Board { get; }

        /// <summary>The winning lines in the game's board.</summary>
        public Board<Player> Highlight { get; }

        /// <summary>How many turns this game has lasted for.</summary>
        public int Time { get; private set; }

        /// <summary>The player whose turn it is.</summary>
        public Player Turn { get; private set; }

        /// <summary>The game's winning player.</summary>
        public Player Winner { get; private set; }

        /// <summary>The currently selected cell.</summary>
        public Pos? Pointer { get; set; }

        /// <summary>How many lines of three a player has.</summary>
        public Dictionary<Player, int> Threes { get; } = new Dictionary<Player, int>();


        public Game(Player startingPlayer = default)
        {
            Board = new Player[5, 5];
            Board.Fill(Player.None);
            Highlight = new Player[5, 5];
            Highlight.Fill(Player.None);

            Turn = startingPlayer == default ? Player.One : startingPlayer;
            Pointer = (2, 2);
        }
        



        /// <summary>Moves the pointer by a specified amount.</summary>
        public void ShiftPointer(int x, int y)
        {
            Pointer = Board.Wrap(Pointer.Value + (x, y));
        }


        /// <summary>Places a symbol on the board as the current player,
        /// and checks for a possible winner.</summary>
        public void PlaceSymbol(Pos pos)
        {
            if (Winner != Player.None || Board[pos] != Player.None) return;

            Board[pos] = Turn;
            Time++;
            Winner = FindWinner();
            if (Winner == Player.None) Turn = Turn.Opponent;
        }


        /// <summary>Makes a decision for a position to place a symbol as the current player.</summary>
        public Pos GetTurnAI()
        {
            if (Time == 25) return default;

            var moves = TryCompleteLines(Turn, 4) ?? TryCompleteLines(Turn.Opponent, 4) ?? // Win or avoid losing
                        TryCompleteFlyingLines(Turn) ?? TryCompleteFlyingLines(Turn.Opponent); // Forced win / forced lose situations

            if (Time < 2 && Board[2, 2] == Player.None && Random.Next(4) > 0) moves = new List<Pos> { (2, 2) };

            if (moves == null) // Lines of 3
            {
                var lines = TryCompleteLines(Turn, 3);
                var blocks = TryCompleteLines(Turn.Opponent, 3);

                if (lines == null && blocks == null)
                {
                    moves = TryCompleteLines(Turn, 2) ?? EmptyCells(Board); // Next to itself 
                }
                else
                {
                    var combo = new List<Pos>();
                    if (lines != null)
                    {
                        combo.AddRange(lines.GroupBy(x => x).Where(g => g.Count() > 1).Select(g => g.Key)); // Double line
                    }
                    if (blocks != null)
                    {
                        combo.AddRange(blocks.GroupBy(x => x).Where(g => g.Count() > 1).Select(g => g.Key)); // Double block
                    }
                    if (lines != null && blocks != null)
                    {
                        combo.AddRange(lines.Where(x => blocks.Contains(x))); // line + block
                    }

                    if (combo.Count > 0) moves = combo;
                    else moves = lines ?? blocks;
                }
            }

            return moves[Random.Next(moves.Count)];
        }


        /// <summary>Attempts to find the winner of the game.</summary>
        private Player FindWinner()
        {
            if (Time < 7) return Player.None;

            if (FindLines(Turn, 4) > 0) return Turn;

            if (Time < Board.Length) return Player.None;

            Threes[Player.One] = FindLines(Player.One, 3);
            Threes[Player.Two] = FindLines(Player.Two, 3);

            return Threes[Player.One] > Threes[Player.Two] ? Player.One
                 : Threes[Player.One] < Threes[Player.Two] ? Player.Two
                 : Player.Tie;
        }


        /// <summary>All the cells that haven't been filled.</summary>
        private static List<Pos> EmptyCells(Board<Player> board)
        {
            return board.Positions.Where(p => board[p] == Player.None).ToList();
        }


        /// <summary>Finds lines of a certain length in the board and marks them in the highlights.</summary>
        private int FindLines(Player player, int length)
        {
            int count = 0;
            List<Pos> line = new List<Pos>();


            void CheckCell(int x, int y)
            {
                Pos pos = (x, y);
                if (Board[pos] == player)
                {
                    line.Add(pos);

                    if (line.Count >= length)
                    {
                        if (line.Count == length) count++;
                        foreach (Pos p in line) Highlight[p] = player;
                    }
                }
                else line = new List<Pos>();
            }


            for (int y = 0; y < Board.Height; y++) // Horizontals
            {
                for (int x = 0; x < Board.Width; x++)
                {
                    CheckCell(x, y);
                }
                line = new List<Pos>();
            }

            for (int x = 0; x < Board.Width; x++) // Verticals
            {
                for (int y = 0; y < Board.Height; y++)
                {
                    CheckCell(x, y);
                }
                line = new List<Pos>();
            }

            for (int d = length - 1; d <= Board.Width + Board.Height - length; d++) // Top-to-left diagonals
            {
                for (int x, y = 0; y <= d; y++)
                {
                    if (y < Board.Height && (x = d - y) < Board.Width)
                    {
                        CheckCell(x, y);
                    }
                }
                line = new List<Pos>();
            }

            for (int d = length - 1; d <= Board.Width + Board.Height - length; d++) // Top-to-right diagonals
            {
                for (int x, y = 0; y <= d; y++)
                {
                    if (y < Board.Height && (x = Board.Width - 1 - d + y) >= 0)
                    {
                        CheckCell(x, y);
                    }
                }
                line = new List<Pos>();
            }

            return count;
        }


        /// <summary>Finds positions that would complete a line of a certain length for a given player.</summary>
        private List<Pos> TryCompleteLines(Player player, int length)
        {
            uint count = 0;
            List<Pos> matches = new List<Pos>();
            Pos? missing = null;


            void CheckCell(int x, int y)
            {
                Pos pos = (x, y);

                if (Board[pos] == player) count++; // Consecutive symbols
                else if (Board[pos] == Player.None) // Find a gap
                {
                    if (missing != null) count = 0; // There was already a gap, line is broken
                    missing = pos;
                }
                else // line is broken
                {
                    count = 0;
                    missing = null;
                }

                if (count == length - 1 && missing.HasValue) matches.Add(missing.Value);
            }


            for (int y = 0; y < Board.Height; y++) // Rows
            {
                for (int x = 0; x < Board.Width; x++)
                {
                    CheckCell(x, y);
                }
                count = 0;
                missing = null;
            }

            for (int x = 0; x < Board.Width; x++) // Columns
            {
                for (int y = 0; y < Board.Height; y++)
                {
                    CheckCell(x, y);
                }
                count = 0;
                missing = null;
            }

            for (int d = length - 1; d <= Board.Height + Board.Width - length; d++) // Top-to-left diagonals
            {
                for (int x, y = 0; y <= d; y++)
                {
                    if (y < Board.Height && (x = d - y) < Board.Width)
                    {
                        CheckCell(x, y);
                    }
                }
                count = 0;
                missing = null;
            }

            for (int d = length - 1; d <= Board.Height + Board.Width - length; d++) // Top-to-right diagonals
            {
                for (int x, y = 0; y <= d; y++)
                {
                    if (y < Board.Height && (x = Board.Width - 1 - d + y) >= 0)
                    {
                        CheckCell(x, y);
                    }
                }
                count = 0;
                missing = null;
            }

            return matches.Count > 0 ? matches : null;
        }


        /// <summary>Finds positions that would complete a line of 3 in the center of the board.</summary>
        private List<Pos> TryCompleteFlyingLines(Player player)
        {
            uint count;
            Pos? missing;
            var matches = new List<Pos>();


            void CheckCell(int x, int y)
            {
                Pos pos = (x, y);
                if (Board[pos] == player) count++; // Consecutive symbols
                else if (Board[pos] == Player.None) missing = pos; // Find a gap

                if (count == 2 && missing.HasValue) matches.Add(missing.Value);
            }


            for (int y = 0; y < Board.Height; y++) // Rows
            {
                count = 0;
                missing = null;
                if (Board[0, y] != Player.None || Board[Board.Width - 1, y] != Player.None) continue;

                for (int x = 1; x < Board.Width - 1; x++) CheckCell(x, y);
            }

            for (int x = 0; x < Board.Width; x++) // Columns
            {
                count = 0;
                missing = null;
                if (Board[x, 0] != Player.None || Board[x, Board.Height - 1] != Player.None) continue;

                for (int y = 1; y < Board.Height - 1; y++) CheckCell(x, y);
            }

            if (Board[0, 0] == Player.None && Board[Board.Width - 1, Board.Height - 1] == Player.None)
            {
                count = 0;
                missing = null;

                for (int d = 1; d < Board.Width - 1; d++) CheckCell(d, d);
            }

            if (Board[Board.Width - 1, 0] == Player.None && Board[0, Board.Height - 1] == Player.None)
            {
                count = 0;
                missing = null;

                for (int d = 1; d < Board.Width - 1; d++) CheckCell(Board.Width - 1 - d, d);
            }
            
            return matches.Count > 0 ? matches : null;
        }
    }
}
