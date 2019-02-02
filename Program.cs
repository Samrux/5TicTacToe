using System;
using System.Collections.Generic;
using System.Threading;

namespace TTT5
{
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

            bool ai = OpenTitle();
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
                bool forceQuit = false;
                Display.AdjustSize();
                Display.DrawMatch(game, scores);

                while (game.Winner == Player.None) // Game loop
                {
                    var oldPointer = game.Pointer.Value;

                    if (ai && game.Turn == Player.Two)
                    {
                        var aiPos = game.GetTurnAI();
                        game.Pointer = aiPos;
                        Display.DrawCell(game, oldPointer);
                        Display.DrawCell(game, aiPos);

                        Thread.Sleep(100);
                        Console.Beep(game.Turn.Tone, 50);
                        Thread.Sleep(100);

                        game.PlaceSymbol(aiPos);
                        Display.DrawCell(game, aiPos);
                        Display.DrawTurn(game);
                    }
                    else
                    {
                        ClearKeys();

                        switch (Console.ReadKey(true).Key)
                        {
                            case ConsoleKey.LeftArrow:
                                game.ShiftPointer(-1, 0);
                                break;
                            case ConsoleKey.UpArrow:
                                game.ShiftPointer(0, -1);
                                break;
                            case ConsoleKey.RightArrow:
                                game.ShiftPointer(1, 0);
                                break;
                            case ConsoleKey.DownArrow:
                                game.ShiftPointer(0, 1);
                                break;
                            case ConsoleKey.Enter:
                            case ConsoleKey.Spacebar:
                                Console.Beep(game.Turn.Tone, 50);
                                game.PlaceSymbol(game.Pointer.Value);
                                Display.DrawTurn(game);
                                break;
                            case ConsoleKey.F1:
                                Console.Beep(600, 50);
                                OpenInstructions();
                                Display.AdjustSize();
                                Display.DrawMatch(game, scores);
                                break;
                            case ConsoleKey.F5:
                                Console.Beep(300, 250);
                                forceQuit = true;
                                break;
                            case ConsoleKey.F10:
                                Console.Beep(400, 50);
                                Console.Beep(300, 500);
                                forceQuit = true;
                                scores = new Dictionary<Player, int>()
                                {
                                    { Player.One, 0 },
                                    { Player.Two, 0 },
                                    { Player.Tie, 0 },
                                };
                                ai = OpenTitle();
                                break;

                        }

                        if (forceQuit) break;

                        if (Display.AdjustSize())
                        {
                            Display.DrawMatch(game, scores);
                        }
                        else
                        {
                            Display.DrawCell(game, game.Pointer.Value);
                            Display.DrawCell(game, oldPointer);
                        }
                    }
                }

                if (forceQuit) continue;

                game.Pointer = null;
                scores[game.Winner]++;

                if (game.Winner == Player.None) break;
                else if (game.Winner == Player.Tie) next = next.Opponent;
                else next = game.Winner.Opponent;

                Display.DrawGameOver(game, scores);

                Console.Beep(400, 100);
                Console.Beep(300, 100);
                Console.Beep(500, 500);

                ClearKeys();
                Console.ReadKey(true);
            }
        }


        /// <summary>Removes any queued input from the user.</summary>
        public static void ClearKeys()
        {
            while (Console.KeyAvailable) Console.ReadKey(true);
        }


        /// <summary>Open the title screen.</summary>
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

            Console.Beep(choosingAI ? 500 : 400, 200);

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
            Console.Beep(600, 200);
        }
    }
}
