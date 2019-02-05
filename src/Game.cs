using System.Collections.Generic;
using System.Linq;

namespace TTT5
{
    /// <summary>
    /// Represent an instance of a 5-Tic-Tac-Toe game.
    /// </summary>
    public class Game
    {
        /// <summary>The game's board.</summary>
        public Board<Player> Board { get; }

        /// <summary>The winning lines in the game's board.</summary>
        public Board<Player> Highlights { get; }

        /// <summary>How many turns this game has lasted for.</summary>
        public int Time { get; private set; }

        /// <summary>The player whose turn it is.</summary>
        public Player Turn { get; private set; }

        /// <summary>The game's winning player.</summary>
        public Player Winner { get; private set; }

        /// <summary>How many lines of three a player has.</summary>
        public Dictionary<Player, int> Threes { get; } = new Dictionary<Player, int>();


        public Game(Player startingPlayer = default)
        {
            Board = new Board<Player>(5, 5, Player.None);
            Highlights = Board.Copy();

            Turn = startingPlayer == default ? Player.One : startingPlayer;
        }


        
        /// <summary>Places a symbol on the board as the current player and advances to the next turn,
        /// checking for a possible winner.</summary>
        public void DoTurn(Pos pos)
        {
            if (Winner != Player.None || Board[pos] != Player.None) return;

            Board[pos] = Turn;
            Time++;
            Winner = FindWinner();
            if (Winner == Player.None) Turn = Turn.Opponent;
        }


        /// <summary>All the cells that haven't been filled.</summary>
        public List<Pos> EmptyCells()
        {
            return Board.Positions.Where(p => Board[p] == Player.None).ToList();
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


        /// <summary>Finds lines of a certain length in the board and marks them in <see cref="Highlights"/>.</summary>
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
                        foreach (Pos p in line) Highlights[p] = player;
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
    }
}
