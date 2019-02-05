using System;
using System.Collections.Generic;
using System.Threading;

namespace TTT5
{
    /// <summary>
    /// Contains the main logic of the program.
    /// </summary>
    public static class Program
    {
        /// <summary>Entry point of the program.</summary>
        public static void Main(string[] args)
        {
            Console.Title = "5-Tic-Tac-Toe";
            Console.CursorVisible = false;
            WinUtil.DisableMouse();
            
            Display.AdjustSize(false);
            Display.PlayIntro();
            Display.DrawBorders();
            Display.DrawOrnaments();

            bool vsAI = OpenTitle();
            OpenInstructions();


            var next = Player.One;
            var scores = new Dictionary<Player, int>()
            {
                { Player.One, 0 },
                { Player.Two, 0 },
                { Player.Tie, 0 },
            };

            while (true) // Program loop
            {
                var game = new Game(next);
                var ai = vsAI ? new GameAI(game) : null;
                Pos selectedCell = new Pos(2, 2);
                bool forceQuit = false;

                Display.AdjustSize();
                Display.DrawMatch(game, selectedCell, scores);

                while (game.Winner == Player.None) // Game loop
                {
                    var oldSelection = selectedCell;

                    if (ai != null && game.Turn == Player.Two)
                    {
                        var aiSelection = ai.GetNextPosition();
                        selectedCell = aiSelection;
                        Display.DrawCell(game, oldSelection);
                        Display.DrawCell(game, aiSelection, true);

                        Thread.Sleep(100);
                        Beep(game.Turn);
                        Thread.Sleep(100);

                        game.DoTurn(aiSelection);
                        Display.DrawCell(game, aiSelection, true);
                        Display.DrawTurn(game);
                    }
                    else
                    {
                        ClearKeys();

                        switch (Console.ReadKey(true).Key)
                        {
                            case ConsoleKey.LeftArrow:
                                selectedCell += (-1, 0);
                                break;
                            case ConsoleKey.UpArrow:
                                selectedCell += (0, -1);
                                break;
                            case ConsoleKey.RightArrow:
                                selectedCell += (1, 0);
                                break;
                            case ConsoleKey.DownArrow:
                                selectedCell += (0, 1);
                                break;
                            case ConsoleKey.Enter:
                            case ConsoleKey.Spacebar:
                                if (game.Board[selectedCell] == Player.None)
                                {
                                    Beep(game.Turn);
                                    game.DoTurn(selectedCell);
                                    Display.DrawTurn(game);
                                }
                                break;
                            case ConsoleKey.F1:
                                Beep(600, 50);
                                OpenInstructions();
                                Display.AdjustSize();
                                Display.DrawMatch(game, selectedCell, scores);
                                break;
                            case ConsoleKey.F5:
                                Beep(300, 250);
                                forceQuit = true;
                                break;
                            case ConsoleKey.F10:
                                Beep(400, 50);
                                Beep(300, 500);
                                forceQuit = true;
                                scores = new Dictionary<Player, int>()
                                {
                                    { Player.One, 0 },
                                    { Player.Two, 0 },
                                    { Player.Tie, 0 },
                                };
                                vsAI = OpenTitle();
                                break;

                        }

                        if (forceQuit) break;

                        selectedCell = game.Board.Wrap(selectedCell);

                        if (Display.AdjustSize())
                        {
                            Display.DrawMatch(game, selectedCell, scores);
                        }
                        else
                        {
                            Display.DrawCell(game, oldSelection);
                            Display.DrawCell(game, selectedCell, true);
                        }
                    }
                }

                if (forceQuit) continue;

                scores[game.Winner]++;

                if (game.Winner == Player.None) break;
                else if (game.Winner == Player.Tie) next = next.Opponent;
                else next = game.Winner.Opponent;

                Display.DrawGameOver(game, scores);

                Beep(400, 100);
                Beep(300, 100);
                Beep(500, 500);

                ClearKeys();
                Console.ReadKey(true);
            }
        }


        /// <summary>Plays a console beep sound if possible.</summary>
        public static void Beep(int frequency, int duration)
        {
            try { Console.Beep(frequency, duration); }
            catch (PlatformNotSupportedException) { }
        }

        /// <summary>Plays a player's beep sound if possible.</summary>
        public static void Beep(Player player)
        {
            Beep(player == Player.One ? 500 : player == Player.Two ? 400 : 200, 50);
        }


        /// <summary>Removes any queued input from the user.</summary>
        public static void ClearKeys()
        {
            while (Console.KeyAvailable) Console.ReadKey(true);
        }


        /// <summary>Opens the title screen.</summary>
        public static bool OpenTitle()
        {
            Display.DrawTitleScreen();
            bool choosingAI = true;
            bool frame = false;

            ConsoleKey key = 0;
            ClearKeys();
            while (key != ConsoleKey.Enter && key != ConsoleKey.Spacebar)
            {
                if (Display.AdjustSize()) Display.DrawTitleScreen();
                Display.DrawTitlePointer(choosingAI, frame);

                frame = !frame;
                Thread.Sleep(250);
                if (Console.KeyAvailable) key = Console.ReadKey(true).Key;

                if (key == ConsoleKey.LeftArrow || key == ConsoleKey.RightArrow)
                {
                    choosingAI = !choosingAI;
                    frame = false;
                    key = 0;
                }
            }

            Beep(choosingAI ? 500 : 400, 200);

            return choosingAI;
        }


        /// <summary>Opens the instructions screen.</summary>
        public static void OpenInstructions()
        {
            Display.AdjustSize();
            Display.DrawInstructions();

            ClearKeys();
            ConsoleKey key = 0;
            while (key != ConsoleKey.Enter && key != ConsoleKey.Spacebar)
            {
                key = Console.ReadKey(true).Key;
            }
            Beep(600, 200);
        }
    }
}
