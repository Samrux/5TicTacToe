#ifndef Game_5TTT
#define Game_5TTT

#include "Utils.h"


static const char* GAME_VERSION = "v2.3";
static const bool DEBUG = false;


enum Player{
    NOBODY,
    PLAYER_1,
    PLAYER_2,
    TIE
};

static Player otherp(Player player) { // Opposite player
    if (player == 1) return PLAYER_2;
    else return PLAYER_1;
}


enum LineDir {
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_TTL,
    DIAGONAL_TTR
};


class Display {
private:
    static utils::Coord SCR, GRID, LOGO, HELP, CTRL, SCTR, SCORE; // Screen position coordinates

public:
    static const bool REDRAW = true, NO_REDRAW = false;

    static void SetScrCoords(utils::Coord);
    static void ClearGameScreen();
    static bool AdjustScreen(); // Resizes game area then returns true if the console size has changed

    static void Game(Player, int*, bool, bool=false);
    static void Borders(bool);
    static void Grid();
    static void Pointer(Player);
    static void Cell(utils::Coord);
    static void FilledCells();
    static void Turn(Player);

    static void TitleScreen(bool, bool=false);
    static void HelpScreen();
    static void Controls(bool, bool = false);
    static void Score(int[4], bool = false);
    static void WinScreen(Player, int[2]);
};


class Board {
private:
    static Player c[5][5];
    static Player hl[5][5];

    static void Highlight(Player, int, int, LineDir, int);

public:
    static utils::Coord pointer; //Selected cell
    static int nCellsFilled;
        
    static Player cell(int, int);
    static Player cell(utils::Coord);
    static Player cellHL(int, int);
    static Player cellHL(utils::Coord);

    static Player selectedcell();
    static void FillCell(Player);
    static void Reset();
    static Player FindWinner();
    static int CountThrees(Player);
};


class Perform { //some actions for the main function to activate
 public:
    static void ProgramSetup();
    static void OpenTitleScreen();
    static void OpenInstructions();
};


#endif // Game_5TTT