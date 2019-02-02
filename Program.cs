using System;
using System.Collections.Generic;
using System.Threading;
using TTT5.GameElements;

namespace TTT5
{
    public static class Program
    {
        /// <summary>Entry point of the program.</summary>
        public static void Main(string[] args)
        {
            Console.Title = "5-Tic-Tac-Toe";
            Console.CursorVisible = false;
            Console.CursorSize = 1;

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
                Display.AdjustSize();
                Display.DrawMatch(game, scores);

                if (ai && game.Turn == Player.Two)
                {
                    Display.DrawCell(game, game.PlaceSymbolAI());
                }

                ClearKeys();

                while (game.Winner == Player.None) // Game loop
                {
                    var oldPointer = game.Pointer.Value;
                    var key = Console.ReadKey(true).Key;

                    switch (key)
                    {
                        case ConsoleKey.LeftArrow:
                            game.Pointer += Direction.Left;
                            break;
                        case ConsoleKey.UpArrow:
                            game.Pointer += Direction.Up;
                            break;
                        case ConsoleKey.RightArrow:
                            game.Pointer += Direction.Right;
                            break;
                        case ConsoleKey.DownArrow:
                            game.Pointer += Direction.Down;
                            break;
                        case ConsoleKey.Enter:
                        case ConsoleKey.Spacebar:
                            Console.Beep(game.Turn.Tone, 50);
                            game.PlaceSymbol(game.Pointer.Value);

                            if (ai && game.Winner == Player.None)
                            {
                                var aiPos = game.GetSymbolAI();
                                game.Pointer = aiPos;
                                Display.DrawCell(game, oldPointer);
                                Display.DrawCell(game, aiPos);
                                Display.DrawTurn(game);

                                Thread.Sleep(100);
                                Console.Beep(game.Turn.Tone, 50);
                                Thread.Sleep(100);

                                game.PlaceSymbol(aiPos);
                                Display.DrawCell(game, aiPos);
                            }

                            Display.DrawTurn(game);
                            break;
                    }

                    game.Pointer = game.Board.Wrap(game.Pointer.Value);

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
                Display.DrawTitleAnimation(choosingAI, frame);

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

            return choosingAI;
        }


        /// <summary>Opens the instructions screen.</summary>
        public static void OpenInstructions()
        {
            Display.AdjustSize();
            Display.DrawInstructions();
            ClearKeys();
            Console.ReadKey(true);
        }
    }
}
/*
int main()
{
    Perform::ProgramSetup();
    Display::Borders(!DEBUG);
    if (!DEBUG) {
        Perform::OpenTitleScreen();
        Perform::OpenInstructions();
    }

    //Creating game variables
    Player currentPlayer = NOBODY, lastPlayer = NOBODY, winner;
    bool endMatch, controlsOpen = true, firstLaunch = true;
    int score[4] = {0};

    while (true) // Main program loop
    {
        //Set up the match about to begin
        winner = NOBODY;
        endMatch = false;
        currentPlayer = otherp(lastPlayer);
        Board::Reset();

        Display::AdjustScreen();
        Display::Game(currentPlayer, score, controlsOpen, firstLaunch);

        firstLaunch = false;

        while (winner == NOBODY && Board::nCellsFilled < 25 && !endMatch)
        {
            switch (AwaitKeyPress()) {
                case ENTER_KEY: case SPACEBAR:
                    if (Board::selectedcell() == NOBODY) {
                        Board::FillCell(currentPlayer);
                        winner = Board::FindWinner();
                        if (winner == NOBODY) {
                            if (Board::nCellsFilled < 25) Boop(currentPlayer*300, 100);
                            currentPlayer = otherp(currentPlayer);
                            Display::Pointer(currentPlayer);
                            Display::Turn(currentPlayer);
                        }
                    }
                    break;

                case UP_KEY:
                    if (Board::pointer.Y > 0) {
                        Display::Cell(Board::pointer);
                        Board::pointer.Y--;
                        Display::Pointer(currentPlayer);
                    }
                    break;

                case DOWN_KEY:
                    if (Board::pointer.Y < 4) {
                        Display::Cell(Board::pointer);
                        Board::pointer.Y++;
                        Display::Pointer(currentPlayer);
                    }
                    break;

                case LEFT_KEY:
                    if (Board::pointer.X > 0) {
                        Display::Cell(Board::pointer);
                        Board::pointer.X--;
                        Display::Pointer(currentPlayer);
                    }
                    break;

                case RIGHT_KEY:
                    if (Board::pointer.X < 4) {
                        Display::Cell(Board::pointer);
                        Board::pointer.X++;
                        Display::Pointer(currentPlayer);
                    }
                    break;

                case BACKSPACE:
                    controlsOpen = !controlsOpen;
                    Display::Controls(controlsOpen);
                    Boop(250, 100);
                    break;

                case F1_KEY: // Instructions
                    Boop(700, 100);
                    Perform::OpenInstructions();
                    Display::Game(currentPlayer, score, controlsOpen);
                    break;

                case F5_KEY: // Restart match
                    Boop(400, 100);
                    Boop(400, 100);
                    endMatch = true;
                    break;

                case F10_KEY: // Soft-reboot game
                    endMatch = true;
                    controlsOpen = false;
                    for (int i = 1; i <= 3; i++) score[i] = 0;

                    Boop(200, 100); Boop(400, 500);
                    Display::ClearGameScreen();
                    Perform::OpenTitleScreen();
                    break;

                case 'a': case 'A':
                    if (DEBUG) Board::nCellsFilled = 25; // Exit and check winner
                    break;

                default: break;
            }

            //Redraws game if screen was resized
            if (Display::AdjustScreen()) Display::Game(currentPlayer, score, controlsOpen, firstLaunch);

        } //End match


        if (!endMatch) { // Game wasn't forced to end
            int threes[2] = {-1, -1};
            if (winner == NOBODY) {
                threes[0] = Board::CountThrees(PLAYER_1);
                threes[1] = Board::CountThrees(PLAYER_2);
                if (threes[0] == threes[1]) winner = TIE;
                else winner = (threes[0] > threes[1]) ? PLAYER_1 : PLAYER_2;
            }

            score[winner]++;
            Display::Score(score, Display::NO_REDRAW);
            Display::WinScreen(winner, threes);
            WaitForEnter();
            Display::ClearGameScreen();

            // Last player defines which one will play first next
            if (winner != TIE) lastPlayer = winner;
            else lastPlayer = otherp(lastPlayer);
        }

    } // End game loop
}
*/