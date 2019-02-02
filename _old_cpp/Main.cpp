#include "Utils.h"
#include "Game.h"
using namespace utils;



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
