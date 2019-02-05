using System;
using System.Collections.Generic;
using System.Linq;

namespace TTT5
{
    /// <summary>
    /// Provides autonomous decision-making for a 5-Tic-Tac-Toe match.
    /// </summary>
    public class GameAI
    {
        private static readonly Random Random = new Random();

        private Game game;
        private Board<Player> Board => game.Board;

        public GameAI(Game game)
        {
            this.game = game;
        }


        /// <summary>Returns the chosen position to place a symbol as the current player.</summary>
        public Pos GetNextPosition()
        {
            if (game.Time == 25) return default;

            List<Pos> moves;
            if (game.Time < 2 && Board[2, 2] == Player.None && Random.Next(4) > 0)
            {
                moves = new List<Pos> { (2, 2) };
            }
            else
            {
                moves = TryCompleteLines(game.Turn, 4) ?? TryCompleteLines(game.Turn.Opponent, 4) ?? // Win or avoid losing
                        TryCompleteFlyingLines(game.Turn) ?? TryCompleteFlyingLines(game.Turn.Opponent); // Forced win / forced lose situations
            }

            if (moves == null) // Lines of 3
            {
                var lines = TryCompleteLines(game.Turn, 3);
                var blocks = TryCompleteLines(game.Turn.Opponent, 3);

                if (lines == null && blocks == null)
                {
                    moves = TryCompleteLines(game.Turn, 2) ?? game.EmptyCells(); // Next to itself 
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
