using System;
using System.Collections.Generic;
using System.Threading;
using TTT5.GameElements;

namespace TTT5
{
    /// <summary>
    /// Manages the drawing of all user interface elements of the game in the console.
    /// </summary>
    public static class Display
    {
        // Element positions in the console
        private static Pos Screen;
        private static Pos BorderStart => (1, 1);
        private static Pos BorderEnd => Screen - (1, 2);
        private static Pos OrnamentStart => BorderStart + (2, 1);
        private static Pos OrnamentEnd => BorderEnd - (2, 1);
        private static Pos Grid => Screen/2 -  (9, 4);
        private static Pos Logo => Screen/2 - (31, 6);
        private static Pos Help => Screen/2 - (30, 4);
        private static Pos Controls => Screen - (30, 15);
        private static Pos ControlsSmall => Screen - (23, 7);
        private static Pos Score => (6, 4);
        private static Pos Threes => (6, 12);


        private static readonly string[] Title5 = new[]
        {
            "     ::::::::::",
            "    :+:    :+: ",
            "   +:+         ",
            "  +#++:++#+    ",
            "        +#+    ",
            "#+#    #+#     ",
            "########       ",
        };

        private static readonly string[] TitleT = new[]
        {
            " ::::::::::::",
            "     :+:     ",
            "    +:+      ",
            "   +#+       ",
            "  +#+        ",
            " #+#         ",
            "###          ",
        };

        private static readonly string[] Instructions = new[]
        {
            "You play in a 5x5 board, Player 1 using X, player 2 using O.",
            "To win, you must make more lines of three symbols than your",
            "opponent.",
            "But be careful! If a player makes a line of four, they will",
            "win the game instantly. Don't lose focus.",
            "Use your wits and develop the best strategies!",
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
        public static void DrawHorizontal(Pos pos, int length, char value, ConsoleColor? color = null, ConsoleColor? background = null)
        {
            Draw(pos, new string(value, length), color, background);
        }

        /// <summary>Draws a vertical line composed of the given value.</summary>
        public static void DrawVertical(Pos pos, int length, object value, ConsoleColor? color = null, ConsoleColor? background = null)
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
                Console.SetBufferSize(Screen.X, Screen.Y);
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
        public static void DrawMatch(Game game, IDictionary<Player, int> scores)
        {
            ClearGame();
            DrawBoard(game);
            DrawTurn(game);
            DrawScores(scores);
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
            DrawHorizontal(BorderStart + (1, 0), (BorderEnd-BorderStart).X-1, BoxChars.DoubleHorizontal);
            DrawHorizontal((BorderStart.X+1, BorderEnd.Y), (BorderEnd-BorderStart).X-1, BoxChars.DoubleHorizontal);
            DrawVertical(BorderStart + (0, 1), (BorderEnd - BorderStart).Y - 1, BoxChars.DoubleVertical);
            DrawVertical((BorderEnd.X, BorderStart.Y+1), (BorderEnd - BorderStart).Y - 1, BoxChars.DoubleVertical);

            Draw(Screen.X - 11, BorderEnd.Y, "By Samrux", ConsoleColor.Gray);
        }





        /// <summary>Draws the grid of the game board.</summary>
        public static void DrawGrid()
        {
            for (int jx = 1; jx <= 4; jx++)
            {
                DrawVertical(Grid + (4*jx - 2, 0), 9, BoxChars.Vertical);
            }
            for (int jy = 1; jy < 5; jy++)
            {
                DrawHorizontal(Grid + (-1, 2*jy - 1), 19, BoxChars.Horizontal);
            }
            for (int jy = 1; jy < 5; jy++)
            {
                for (int jx = 1; jx < 5; jx++)
                {
                    Draw(Grid + (4*jx - 2, 2*jy - 1), BoxChars.Joint);
                }
            }
        }


        /// <summary>Draws a cell of the board, and the pointer if it's selecting the cell.</summary>
        public static void DrawCell(Game game, Pos pos)
        {
            var cell = game.Board[pos];

            if (pos == game.Pointer)
            {
                Draw(Grid + (4*pos.X - 1, 2*pos.Y), game.Turn.Pointer, ConsoleColor.White);

                var symbol = cell == Player.None ? '_' : cell.Symbol;
                var color = cell == Player.None ? game.Turn.Color : cell.Color;
                Draw(Grid + (4*pos.X, 2*pos.Y), symbol, color);
            }
            else
            {
                Draw(Grid + (4*pos.X - 1, 2*pos.Y), $" {cell.Symbol} ", cell.Color, game.Highlight[pos].BgColor);
            }
        }


        /// <summary>Draws the entire game board.</summary>
        public static void DrawBoard(Game game)
        {
            DrawGrid();

            for (int y = 0; y < 5; y++)
                for (int x = 0; x < 5; x++)
                    DrawCell(game, (x, y));
        }


        /// <summary>Draws the elements of the current player's turn.</summary>
        public static void DrawTurn(Game game)
        {
            Draw(Grid - (4, 4), game.Turn.TurnTitle, game.Turn.Color);
            DrawOrnaments(game.Turn.Color);
        }


        public static void DrawScores(IDictionary<Player, int> scores)
        {
            Console.ForegroundColor = ConsoleColor.White;

            Draw(Score, BoxChars.RightDown);
            DrawHorizontal(Score + (1, 0), 11, BoxChars.Horizontal);
            Draw(Score + (12, 0), BoxChars.LeftDown);
            Draw(Score + (3, 0), "<Score>");
            Draw(Score + (0, 1), BoxChars.Vertical);
            DrawVertical(Score + (0, 2), 3, $"{BoxChars.UpRightDown}{BoxChars.Horizontal}");
            Draw(Score + (0, 5), BoxChars.Vertical);
            Draw(Score + (0, 6), BoxChars.RightUp);
            for (var p = Player.One; p <= Player.Tie; p++)
            {
                Draw(Score + (3, 1 + p), $"{p}: {scores[p].ToString().PadLeft(2)}", p.Color);
            }
        }


        /// <summary>Displays the game over screen.</summary>
        public static void DrawGameOver(Game game, IDictionary<Player, int> scores)
        {
            DrawScores(scores);
            DrawBoard(game);

            Console.ForegroundColor = game.Winner.Color;
            Draw(Grid - (4, 4), game.Winner.WinTitle);
            Draw(Grid + (-4, 11), game.Winner.WinTitle);

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
                Draw(Threes + (4, 2),  game.Threes[Player.One], Player.One.Color);
                Draw(Threes + (10, 4), game.Threes[Player.Two], Player.Two.Color);
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

            Console.Beep(300, 50); // I think it takes a bit for them to play, so this ends up in sync
            Console.Beep(400, 50);
            Thread.Sleep(50);

            for (int y = BorderStart.Y + 1; y < BorderEnd.Y; y++)
            {
                Draw(BorderEnd.X, y, BoxChars.DoubleVertical);
                Thread.Sleep(sleepY);
            }
            Draw(BorderEnd, BoxChars.DoubleUpLeft);

            Console.Beep(500, 50);
            Thread.Sleep(50);

            for (int x = BorderEnd.X - 1; x > BorderStart.X; x--)
            {
                Draw(x, BorderEnd.Y, BoxChars.DoubleHorizontal);
                Thread.Sleep(sleepX);
            }
            Draw(BorderStart.X, BorderEnd.Y, BoxChars.DoubleUpRight);

            Console.Beep(600, 50);
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
                Draw(Logo + (0, i), Title5[i], Player.One.Color);
            }
            Draw(Logo + (12, 4), "+#++:++#++:++", ConsoleColor.Gray);

            for (int t = 0; t < 3; t++)
            {
                var color = t % 2 == 0 ? Player.Two.Color : Player.One.Color;
                for (int i = 0; i < 7; i++)
                {
                    Draw(Logo + (26 + 12 * t, i), TitleT[i], color);
                }
            }

            Draw(Screen.X/2 -  9, Logo.Y + 9, "<@ 5-Tic-Tac-Toe @>", ConsoleColor.White);

            Draw(Screen.X/2 - 12, Screen.Y - 5, "Play vs AI");
            Draw(Screen.X/2 +  4, Screen.Y - 5, "Play 1vs1");
        }


        /// <summary>Draws the 2-frame animation of the title screen.</summary>
        public static void DrawTitleAnimation(bool choice, bool shift)
        {
            Draw(Screen.X/2 - 15, Screen.Y - 5, choice ? shift ? " >>" : ">> " : "   ", Player.Tie.Color);
            Draw(Screen.X/2 - 2, Screen.Y - 5, choice ? shift ? "<< " : " <<" : "   ", Player.Tie.Color);

            Draw(Screen.X/2 + 1, Screen.Y - 5, choice ? "   " : shift ? " >>" : ">> ", Player.Tie.Color);
            Draw(Screen.X/2 + 13, Screen.Y - 5, choice ? "   " : shift ? "<< " : " <<", Player.Tie.Color);
        }


        /// <summary>Draws the instructions screen.</summary>
        public static void DrawInstructions()
        {
            ClearGame();

            for (int i = 0; i < Instructions.Length; i++)
            {
                Draw(Help + (0, i), Instructions[i]);
            }

            Draw(Help + (40, 0), Player.One.Symbol, Player.One.Color);
            Draw(Help + (58, 0), Player.Two.Symbol, Player.Two.Color);
            Draw(Help + (27, 1), "lines of three symbols", ConsoleColor.White);
            Draw(Help + (44, 3), "four", ConsoleColor.White);

            Draw(Screen.X / 2 - 10, Help.Y - 3, "-<@  Instructions  @>-", Player.Tie.Color);
            Draw(Screen.X / 2 - 12, Screen.Y - 5, "{ Press any key to begin }", ConsoleColor.Gray);
        }
    }
}

/*
void Display::Controls(bool bigWindow, bool animation) {
    utils::ClearScreen(CTRL, { CTRL.X+25, CTRL.Y+11 }, 1); // Clear controls box area

    if (!bigWindow) {
        setcolor(GRAY);
        printxy(SCTR.X + 2, SCTR.Y + 1, "Press Backspace ");
        printxy(SCTR.X + 2, SCTR.Y + 2, "to open controls");

        setcolor(NORMAL);
        for (int x = 1; x <= 18; x++) printxy(SCTR.X + x, SCTR.Y, HORIZONTAL_LINE);
        printxy(SCTR.X, SCTR.Y, TOPLEFT_CORNER);
        printxy(SCTR.X + 19, SCTR.Y, TOPRIGHT_CORNER);
        printxy(SCTR.X + 19, SCTR.Y + 1, VERTICAL_LINE);
        printxy(SCTR.X + 19, SCTR.Y + 2, VERTICAL_LINE);
        printxy(SCTR.X + 19, SCTR.Y + 3, BOTTOMRIGHT_CORNER);

    }
    else {
        setcolor(WHITE);
        for (int x = 1; x <= 24; x++) printxy(CTRL.X + x, CTRL.Y, HORIZONTAL_LINE);
        for (int y = 1; y <= 11; y++) printxy(CTRL.X + 25, CTRL.Y + y, VERTICAL_LINE);
        printxy(CTRL.X, CTRL.Y, TOPLEFT_CORNER);
        printxy(CTRL.X + 25, CTRL.Y, TOPRIGHT_CORNER);
        printxy(SCTR.X + 19, CTRL.Y + 11, BOTTOMRIGHT_CORNER);
        printxy(CTRL.X + 8, CTRL.Y, "<Controls>");

        setcolor(GREEN);
        printxy(CTRL.X + 2, CTRL.Y + 2, "< ^ v >");
        printxy(CTRL.X + 2, CTRL.Y + 3, "Enter");
        printxy(CTRL.X + 2, CTRL.Y + 5, "F1");
        printxy(CTRL.X + 2, CTRL.Y + 6, "F5");
        printxy(CTRL.X + 2, CTRL.Y + 7, "F10");
        printxy(CTRL.X + 2, CTRL.Y + 9, "Backspace");

        setcolor(NORMAL);
        printxy(CTRL.X + 11, CTRL.Y + 2, "Move pointer ");
        printxy(CTRL.X + 11, CTRL.Y + 3, "Place symbol ");
        printxy(CTRL.X + 11, CTRL.Y + 5, "Instructions ");
        printxy(CTRL.X + 11, CTRL.Y + 6, "Reset Match  ");
        printxy(CTRL.X + 11, CTRL.Y + 7, "Restart all  ");
        printxy(CTRL.X + 11, CTRL.Y + 9, " Close this  ");
        printxy(CTRL.X + 11, CTRL.Y + 10," controls tab");

        if (animation && !DEBUG) { // Bring attention to the controls
            for (int i = 0; i < 2; i++) {
                printxy(CTRL.X + 11, CTRL.Y - 3, "V V V", WHITE);
                printxy(CTRL.X + 11, CTRL.Y - 2, "     ");
                Wait(150);
                printxy(CTRL.X + 11, CTRL.Y - 3, "     ");
                printxy(CTRL.X + 11, CTRL.Y - 2, "V V V", WHITE);
                Wait(150);
            }

            Wait(200);
            printxy(CTRL.X + 11, CTRL.Y - 2, "     ");

            CleanKbBuffer(); // Ignore any input during the animation
        }
    }
}
*/
