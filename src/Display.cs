using System;
using System.Collections.Generic;
using System.Threading;

namespace TTT5
{
    /// <summary>
    /// Allows the printing of all user interface elements in the command line.
    /// </summary>
    public static class Display
    {
        // Element positions
        private static Pos Screen;
        private static Pos BorderStart => (1, 1);
        private static Pos BorderEnd   => Screen - (1, 2);
        private static Pos OrnamentStart => BorderStart + (2, 1);
        private static Pos OrnamentEnd   => BorderEnd - (2, 1);
        private static Pos Instructions  => Screen/2 - (30, 5);
        private static Pos Title  => Screen/2 - (31, 6);
        private static Pos Grid   => Screen/2 -  (9, 4);
        private static Pos Score  => (6, 4);
        private static Pos Threes => (6, 12);
        private static Pos HelpBox => Screen - (18, 8);


        private static readonly string[] TitleBig5 = new[]
        {
            "     ::::::::::",
            "    :+:    :+: ",
            "   +:+         ",
            "  +#++:++#+    ",
            "        +#+    ",
            "#+#    #+#     ",
            "########       ",
        };

        private static readonly string[] TitleBigT = new[]
        {
            " ::::::::::::",
            "     :+:     ",
            "    +:+      ",
            "   +#+       ",
            "  +#+        ",
            " #+#         ",
            "###          ",
        };

        private static readonly string[] InstructionsText =
        {
            "You play in a 5x5 board, Player 1 using X, player 2 using O.",
            "To win, you must make more lines of three symbols than your",
            "opponent.",
            "But be careful! If a player makes a line of four, they will",
            "win the game instantly. Don't lose focus.",
            "Use your wits and develop the best strategies!",
            "", "",
            "F1 = Instructions |  F5 = Reset Match |  F10 = Title Screen",
        };

        private static readonly string[] PlayerPointers =
        {
            "", "> <", "( )", null,
        };

        private static readonly string[] PlayerTurnTitles =
        {
            "", "+>> Player Red's turn <<+", "/(( Player Blu's turn ))\\", null,
        };

        private static readonly string[] PlayerWinTitles =
        {
            "", " <@+ Player Red won! +@> ", " ~@{ Player Blu won! }@~ ", "   -=] It's a tie! [=-   ",
        };

        private static readonly ConsoleColor[] PlayerColors =
        {
            ConsoleColor.Gray, ConsoleColor.Red, ConsoleColor.Cyan, ConsoleColor.Green,
        };

        private static readonly ConsoleColor[] PlayerBackgroundColors =
        {
            ConsoleColor.Black, ConsoleColor.DarkRed, ConsoleColor.DarkCyan, ConsoleColor.DarkGreen,
        };


        private static class BoxChars
        {
            public const char
                Horizontal = '─',
                Vertical = '│',
                RightDown = '┌',
                LeftDown = '┐',
                RightUp = '└',
                LeftUp = '┘',
                Joint = '┼',
                UpRightDown = '├',
                UpLeftDown = '┤',
                RightDownLeft = '┬',
                RightUpLeft = '┴',
                DoubleHorizontal = '═',
                DoubleVertical = '║',
                DoubleRightDown = '╔',
                DoubleLeftDown = '╗',
                DoubleUpRight = '╚',
                DoubleUpLeft = '╝',
                Block = '■';
        }




        /// <summary>Writes something to the console in specific coordinates.
        /// Color can be specified to temporarily override the current colors.</summary>
        public static void Draw(Pos pos, object value, ConsoleColor? color = null, ConsoleColor? background = null)
            => Draw(pos.X, pos.Y, value, color, background);

        /// <summary>Writes something to the console in specific coordinates.
        /// Color can be specified to temporarily override the current colors.</summary>
        public static void Draw(int x, int y, object value, ConsoleColor? color = null, ConsoleColor? background = null)
        {
            var oldFg = Console.ForegroundColor;
            var oldBg = Console.BackgroundColor;
            if (color != null) Console.ForegroundColor = color.Value;
            if (background != null) Console.BackgroundColor = background.Value;

            Pos oldPos = (Console.CursorLeft, Console.CursorTop);

            try
            {
                Console.SetCursorPosition(x, y);
                Console.Write(value);
                Console.SetCursorPosition(oldPos.X, oldPos.Y);
            }
            catch (ArgumentOutOfRangeException) { } // Console was resized mid-draw
            finally
            {
                Console.ForegroundColor = oldFg;
                Console.BackgroundColor = oldBg;
            }
        }


        /// <summary>Draws a horizontal line composed of the given character.</summary>
        public static void DrawHorizontal(Pos pos, int length, char value = BoxChars.Horizontal, ConsoleColor? color = null, ConsoleColor? background = null)
        {
            Draw(pos, new string(value, length), color, background);
        }

        /// <summary>Draws a vertical line composed of the given value.</summary>
        public static void DrawVertical(Pos pos, int length, char value = BoxChars.Vertical, ConsoleColor? color = null, ConsoleColor? background = null)
        {
            for (int y = 0; y < length; y++) Draw(pos + (0, y), value, color, background);
        }


        /// <summary>Check whether the console has been resized. If so, adjusts and clears the screen.</summary>
        public static bool AdjustSize(bool redrawBorders = true)
        {
            if (Console.WindowWidth < 78 || Console.WindowHeight < 23) // Sensible minimum size
            {
                Console.WindowWidth = 80;
                Console.WindowHeight = 25;
            }

            if (Screen != (Console.WindowWidth, Console.WindowHeight))
            {
                Screen = (Console.WindowWidth, Console.WindowHeight);
                try { Console.SetBufferSize(Screen.X, Screen.Y); }
                catch (PlatformNotSupportedException) { }

                Console.Clear();
                if (redrawBorders)
                {
                    DrawBorders();
                    DrawOrnaments();
                }
                return true;
            }

            return false;
        }


        /// <summary>Clears the game screen.</summary>
        public static void ClearGame()
        {
            for (int y = BorderStart.Y + 1; y < BorderEnd.Y; y++)
            {
                Draw(BorderStart.X + 1, y, new string(' ', (BorderEnd - BorderStart).X - 1));
                Thread.Sleep(10);
            }

            DrawOrnaments();
            Console.ResetColor();
        }


        /// <summary>Draws the entire game screen.</summary>
        public static void DrawMatch(Game game, Pos? selectedCell, IDictionary<Player, int> scores)
        {
            ClearGame();
            DrawBoard(game, selectedCell);
            DrawTurn(game);
            DrawScores(scores);
            DrawHelpBox();
        }


        /// <summary>Draws ornaments inside the game border.</summary>
        public static void DrawOrnaments(ConsoleColor? color = null)
        {
            var oldColor = Console.ForegroundColor;
            if (color != null) Console.ForegroundColor = oldColor;

            Draw(OrnamentStart, BoxChars.RightDown);
            Draw(OrnamentStart.X, OrnamentEnd.Y, BoxChars.RightUp);
            Draw(OrnamentEnd.X, OrnamentStart.Y, BoxChars.LeftDown);
            Draw(OrnamentEnd, BoxChars.LeftUp);

            Console.ForegroundColor = oldColor;
        }


        /// <summary>Draws the screen borders.</summary>
        public static void DrawBorders()
        {
            Console.ForegroundColor = ConsoleColor.White;

            Draw(BorderStart, BoxChars.DoubleRightDown);
            Draw(BorderStart.X, BorderEnd.Y, BoxChars.DoubleUpRight);
            Draw(BorderEnd, BoxChars.DoubleUpLeft);
            Draw(BorderEnd.X, BorderStart.Y, BoxChars.DoubleLeftDown);

            DrawHorizontal(BorderStart + (1, 0),           (BorderEnd-BorderStart).X-1,     BoxChars.DoubleHorizontal);
            DrawHorizontal((BorderStart.X+1, BorderEnd.Y), (BorderEnd-BorderStart).X-1,     BoxChars.DoubleHorizontal);

            DrawVertical(BorderStart + (0, 1),             (BorderEnd - BorderStart).Y - 1, BoxChars.DoubleVertical);
            DrawVertical((BorderEnd.X, BorderStart.Y+1),   (BorderEnd - BorderStart).Y - 1, BoxChars.DoubleVertical);

            Draw(Screen.X - 11, BorderEnd.Y, "By Samrux", ConsoleColor.Gray);
        }





        /// <summary>Draws the grid of the game board.</summary>
        public static void DrawGrid()
        {
            for (int jx = 1; jx <= 4; jx++)
            {
                DrawVertical(Grid + (4*jx - 2, 0), 9);
            }
            for (int jy = 1; jy < 5; jy++)
            {
                DrawHorizontal(Grid + (-1, 2*jy - 1), 19);
            }
            for (int jy = 1; jy < 5; jy++)
            {
                for (int jx = 1; jx < 5; jx++)
                {
                    Draw(Grid + (4*jx - 2, 2*jy - 1), BoxChars.Joint);
                }
            }
        }


        /// <summary>Draws a cell of the board.</summary>
        public static void DrawCell(Game game, Pos pos, bool selected = false)
        {
            var cell = game.Board[pos];

            if (selected)
            {
                Draw(Grid + (4*pos.X - 1, 2*pos.Y), PlayerPointers[game.Turn], ConsoleColor.White);

                var symbol = cell == Player.None ? '_' : cell.Symbol;
                var color = cell == Player.None ? PlayerColors[game.Turn] : PlayerColors[cell];
                Draw(Grid + (4*pos.X, 2*pos.Y), symbol, color);
            }
            else
            {
                Draw(Grid + (4*pos.X - 1, 2*pos.Y), $" {cell.Symbol} ",
                    PlayerColors[cell], PlayerBackgroundColors[game.Highlights[pos]]);
            }
        }


        /// <summary>Draws the entire game board.</summary>
        public static void DrawBoard(Game game, Pos? selectedCell)
        {
            DrawGrid();

            for (int y = 0; y < 5; y++)
            {
                for (int x = 0; x < 5; x++)
                {
                    Pos pos = (x, y);
                    DrawCell(game, pos, pos == selectedCell);
                }
            }
        }


        /// <summary>Draws the elements of the current player's turn.</summary>
        public static void DrawTurn(Game game)
        {
            Draw(Grid - (4, 4), PlayerTurnTitles[game.Turn], PlayerColors[game.Turn]);
            DrawOrnaments(PlayerColors[game.Turn]);
        }


        /// <summary>Draws the score box with each player's score.</summary>
        public static void DrawScores(IDictionary<Player, int> scores)
        {
            Console.ForegroundColor = ConsoleColor.White;

            Draw(Score, BoxChars.RightDown);
            DrawHorizontal(Score + (1, 0), 11);
            Draw(Score + (12, 0), BoxChars.LeftDown);
            Draw(Score + (3, 0), "<Score>");
            DrawVertical(Score + (0, 1), 5);
            DrawVertical(Score + (0, 2), 3, BoxChars.UpRightDown);
            DrawVertical(Score + (1, 2), 3, BoxChars.Horizontal);
            Draw(Score + (0, 6), BoxChars.RightUp);
            foreach (var p in new[] { Player.One, Player.Two, Player.Tie })
            {
                Draw(Score + (3, 1 + p), $"{p}: {scores[p].ToString().PadLeft(2)}", PlayerColors[p]);
            }
        }


        /// <summary>Draws the help prompt in the corner of the game screen.</summary>
        public static void DrawHelpBox()
        {
            Console.ForegroundColor = ConsoleColor.White;

            Draw(HelpBox + (2, 1), "Press   ", ConsoleColor.Gray);
            Draw(HelpBox + (8, 1), "F1");
            Draw(HelpBox + (2, 2), "for help", ConsoleColor.Gray);

            Console.ForegroundColor = ConsoleColor.White;
            DrawHorizontal(HelpBox + (1, 0), 11);
            Draw(HelpBox, BoxChars.RightDown);
            DrawVertical(HelpBox + (11, 1), 3);
            Draw(HelpBox + (11, 0), BoxChars.LeftDown);
            Draw(HelpBox + (11, 3), BoxChars.LeftUp);
        }


        /// <summary>Displays the game over screen.</summary>
        public static void DrawGameOver(Game game, IDictionary<Player, int> scores)
        {
            DrawScores(scores);
            DrawBoard(game, null);

            Console.ForegroundColor = PlayerColors[game.Winner];
            Draw(Grid - (4, 4), PlayerWinTitles[game.Winner]);
            Draw(Grid + (-4, 11), PlayerWinTitles[game.Winner]);

            // Extended corners
            DrawOrnaments();

            DrawHorizontal((OrnamentStart.X+1, OrnamentStart.Y), 8, BoxChars.Horizontal);
            DrawHorizontal((OrnamentStart.X+1, OrnamentEnd.Y),   8, BoxChars.Horizontal);
            DrawHorizontal((OrnamentEnd.X-8,   OrnamentStart.Y), 8, BoxChars.Horizontal);
            DrawHorizontal((OrnamentEnd.X-8,   OrnamentEnd.Y),   8, BoxChars.Horizontal);

            DrawVertical((OrnamentStart.X, OrnamentStart.Y+1), 4, BoxChars.Vertical);
            DrawVertical((OrnamentEnd.X,   OrnamentStart.Y+1), 4, BoxChars.Vertical);
            DrawVertical((OrnamentStart.X, OrnamentEnd.Y-4),   4, BoxChars.Vertical);
            DrawVertical((OrnamentEnd.X,   OrnamentEnd.Y-4),   4, BoxChars.Vertical);

            Console.ForegroundColor = ConsoleColor.White;

            // Threes
            if (game.Threes.Count > 0)
            {
                Draw(Threes, BoxChars.RightDown);
                DrawHorizontal(Threes + (1, 0), 12, BoxChars.Horizontal);
                Draw(Threes + (13, 0), BoxChars.LeftDown);
                Draw(Threes + (3, 0), "<Threes>");
                DrawVertical(Threes + (0, 1), 5, BoxChars.Vertical);
                Draw(Threes + (0, 2), BoxChars.UpRightDown);
                Draw(Threes + (0, 4), BoxChars.UpRightDown);
                Draw(Threes + (0, 6), BoxChars.RightUp);
                Draw(Threes + (7, 3), '/');
                Draw(Threes + (4, 2),  game.Threes[Player.One], PlayerColors[Player.One]);
                Draw(Threes + (10, 4), game.Threes[Player.Two], PlayerColors[Player.Two]);
            }
        }


        /// <summary>Executes the short animation when the game starts up.</summary>
        public static void PlayIntro()
        {
            Console.ForegroundColor = ConsoleColor.Gray;

            int time = 1000;
            int sleepY = time / 4 / Screen.Y;
            int sleepX = time / 4 / Screen.X;

            for (int x = BorderStart.X + 1; x < BorderEnd.X; x++)
            {
                Draw(x, BorderStart.Y, BoxChars.DoubleHorizontal);
                Thread.Sleep(sleepX);
            }
            Draw(BorderEnd.X, BorderStart.Y, BoxChars.DoubleLeftDown);

            Program.Beep(300, 50); // I think it takes a bit for them to play, so this ends up in sync
            Program.Beep(400, 50);
            Thread.Sleep(50);

            for (int y = BorderStart.Y + 1; y < BorderEnd.Y; y++)
            {
                Draw(BorderEnd.X, y, BoxChars.DoubleVertical);
                Thread.Sleep(sleepY);
            }
            Draw(BorderEnd, BoxChars.DoubleUpLeft);

            Program.Beep(500, 50);
            Thread.Sleep(50);

            for (int x = BorderEnd.X - 1; x > BorderStart.X; x--)
            {
                Draw(x, BorderEnd.Y, BoxChars.DoubleHorizontal);
                Thread.Sleep(sleepX);
            }
            Draw(BorderStart.X, BorderEnd.Y, BoxChars.DoubleUpRight);

            Program.Beep(600, 50);
            Thread.Sleep(50);

            for (int y = BorderEnd.Y - 1; y > BorderStart.Y; y--)
            {
                Draw(BorderStart.X, y, BoxChars.DoubleVertical);
                Thread.Sleep(sleepY);
            }
            Draw(BorderStart, BoxChars.DoubleRightDown);
        }


        /// <summary>Draws the title screen and splash logo.</summary>
        public static void DrawTitleScreen()
        {
            ClearGame();

            for (int i = 0; i < 7; i++)
            {
                Draw(Title + (0, i), TitleBig5[i], PlayerColors[Player.One]);
            }
            Draw(Title + (12, 4), "+#++:++#++:++", ConsoleColor.Gray);

            for (int t = 0; t < 3; t++)
            {
                var color = PlayerColors[t % 2 == 0 ? Player.Two : Player.One];
                for (int i = 0; i < 7; i++)
                {
                    Draw(Title + (26 + 12 * t, i), TitleBigT[i], color);
                }
            }

            Draw(Screen.X/2 -  9, Title.Y + 9, "<@ 5-Tic-Tac-Toe @>", ConsoleColor.White);

            Draw(Screen.X/2 - 12, Screen.Y - 5, "Play vs AI");
            Draw(Screen.X/2 +  4, Screen.Y - 5, "Play 1vs1");
        }


        /// <summary>Draws the nimation of the title screen pointer.</summary>
        public static void DrawTitlePointer(bool choice, bool shift)
        {
            Console.ForegroundColor = PlayerColors[Player.Tie];

            Draw(Screen.X/2 - 15, Screen.Y - 5, choice ? shift ? " >>" : ">> " : "   ");
            Draw(Screen.X/2 - 2, Screen.Y - 5, choice ? shift ? "<< " : " <<" : "   ");

            Draw(Screen.X/2 + 1, Screen.Y - 5, choice ? "   " : shift ? " >>" : ">> ");
            Draw(Screen.X/2 + 13, Screen.Y - 5, choice ? "   " : shift ? "<< " : " <<");
        }


        /// <summary>Draws the instructions screen.</summary>
        public static void DrawInstructions()
        {
            ClearGame();
            Console.ResetColor();

            Draw(Screen.X/2 - 11, Instructions.Y - 3, "-<@  Instructions  @>-", PlayerColors[Player.Tie]);
            Draw(Screen.X/2 - 12, Screen.Y - 7, "{ Press Enter or Space }", ConsoleColor.Gray);

            for (int i = 0; i < InstructionsText.Length; i++)
            {
                Draw(Instructions + (0, i), InstructionsText[i]);
            }

            Draw(Instructions + (40, 0), Player.One.Symbol, PlayerColors[Player.One]);
            Draw(Instructions + (58, 0), Player.Two.Symbol, PlayerColors[Player.Two]);
            Draw(Instructions + (27, 1), "lines of three symbols", ConsoleColor.White);
            Draw(Instructions + (44, 3), "four", ConsoleColor.White);
            Draw(Instructions + (0,  InstructionsText.Length-1), "F1",  PlayerColors[Player.Tie]);
            Draw(Instructions + (21, InstructionsText.Length-1), "F5",  PlayerColors[Player.Tie]);
            Draw(Instructions + (41, InstructionsText.Length-1), "F10", PlayerColors[Player.Tie]);
        }
    }
}
