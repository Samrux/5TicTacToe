#include "Utils.h"
#include "Game.h"
using namespace utils;


// Screen positions
Coord Display::SCR = {};
Coord Display::GRID = {};
Coord Display::LOGO = {};
Coord Display::HELP = {};
Coord Display::CTRL = {}; // Controls
Coord Display::SCTR = {}; // Small controls
Coord Display::SCORE = {};


void Display::SetScrCoords(Coord scr) {
    SCR = scr;
    GRID = { SCR.X/2 - 9, SCR.Y/2 - 4 };
    LOGO = { SCR.X/2 - 31, SCR.Y/2 - 6 };
    HELP = { SCR.X/2 - 30, SCR.Y/2 - 4 };
    CTRL = { SCR.X - 30, SCR.Y - 15 };
    SCTR = { SCR.X - 24, SCR.Y - 7 };
    SCORE = { 5,4 };
}


void Display::ClearGameScreen() {
    utils::ClearScreen({ 2,2 }, { SCR.X - 3, SCR.Y - 3 }, 1);

    //Redraw corner ornaments
    printxy(3, 2, TOPLEFT_CORNER);
    printxy(3, SCR.Y - 3, BOTTOMLEFT_CORNER);
    printxy(SCR.X - 4, 2, TOPRIGHT_CORNER);
    printxy(SCR.X - 4, SCR.Y - 3, BOTTOMRIGHT_CORNER);
}


bool Display::AdjustScreen() {
    Coord size = GetConsoleSize();

    if (size.X != SCR.X || size.Y != SCR.Y) {
        Perform::ProgramSetup();
        ClearScreen(ORIGIN, GetConsoleSize());
        Display::Borders(false);
        return true;
    }
    return false;
}


void Display::Borders(bool animation) {
    if(animation) {  // Fancy borders animation
        setcolor(NORMAL);

        const int time = 1000, highsleep = time/4/SCR.Y, widesleep = time/4/SCR.X;

        for(int x = 2; x < SCR.X-2; x++) {
            printxy(x, 1, HORIZONTAL_DOUBLELINE);
            Wait(widesleep);
        }

        printxy(SCR.X-2, 1, TOPRIGHT_DOUBLECORNER);
        Boop(200, 50);

        for(int y = 2; y < SCR.Y-2; y++) {
            printxy(SCR.X-2, y, VERTICAL_DOUBLELINE);
            Wait(highsleep);
        }

        printxy(SCR.X-2, SCR.Y-2, BOTTOMRIGHT_DOUBLECORNER);
        Boop(300, 50);

        for(int x = SCR.X-3; x > 1; x--) {
            printxy(x, SCR.Y-2, HORIZONTAL_DOUBLELINE);
            Wait(widesleep);
        }

        printxy(1, SCR.Y-2, BOTTOMLEFT_DOUBLECORNER);
        Boop(400, 50);

        for(int y = SCR.Y-3; y > 1; y--) {
            printxy(1, y, VERTICAL_DOUBLELINE);
            Wait(highsleep);
        }

        printxy(1, 1, TOPLEFT_DOUBLECORNER);
        Boop(500, 50);
    }


    // Redraw border in white
    setcolor(WHITE);
    for (int x = 2; x < SCR.X-2; x++) printxy(x, 1, HORIZONTAL_DOUBLELINE);
    for (int y = 2; y < SCR.Y-2; y++) printxy(SCR.X-2, y, VERTICAL_DOUBLELINE);
    for (int x = 2; x < SCR.X-2; x++) printxy(x, SCR.Y-2, HORIZONTAL_DOUBLELINE);
    for (int y = 2; y < SCR.Y-2; y++) printxy(1, y, VERTICAL_DOUBLELINE);
    printxy(1, SCR.Y-2, BOTTOMLEFT_DOUBLECORNER);
    printxy(1, 1, TOPLEFT_DOUBLECORNER);
    printxy(SCR.X-2, 1, TOPRIGHT_DOUBLECORNER);
    printxy(SCR.X-2, SCR.Y-2, BOTTOMRIGHT_DOUBLECORNER);

    setcolor(NORMAL);
}


void Display::Game(Player currentPlayer, int *score, bool openControls, bool firstLaunch) {
    Display::Grid();
    Display::FilledCells();
    Display::Turn(currentPlayer);
    Display::Pointer(currentPlayer);
    Display::Score(score, Display::REDRAW);
    Display::Controls(openControls, firstLaunch);
}


void Display::Grid() {

    for (int jy = 1; jy <= 5; jy++) { // Vertical lines
        for (int jx = 1; jx <= 4; jx++) {
            printxy(GRID.X-2 + 4*jx, GRID.Y-2 + 2*jy, VERTICAL_LINE);
        }
    }
    for (int jy = 1; jy < 5; jy++) { // Horizontal lines
        for (int jx = 1; jx < 20; jx++) {
            printxy(GRID.X-2 + 1*jx, GRID.Y-1 + 2*jy, HORIZONTAL_LINE);
        }
    }
    for (int jy = 1; jy < 5; jy++) { // Joints
        for (int jx = 1; jx < 5; jx++) {
            printxy(GRID.X-2 + 4*jx, GRID.Y-1 + 2*jy, CROSS_JOINT);
        }
    }

    printxy(GRID.X - 4, GRID.Y - 4, "-== Player    's turn ==-", GRAY);
}


void Display::Pointer(Player player) {
    printxy(GRID.X - 1 + 4*Board::pointer.X, GRID.Y + 2*Board::pointer.Y, player==1 ? '>' : '(', WHITE);
    printxy(GRID.X + 1 + 4*Board::pointer.X, GRID.Y + 2*Board::pointer.Y, player==1 ? '<' : ')', WHITE);
    if (Board::selectedcell() == NOBODY) printxy(GRID.X + 4*Board::pointer.X, GRID.Y + 2*Board::pointer.Y, "_", player==1 ? RED : CYAN);
}


void Display::Cell(Coord cell) {
    const char* symbol = "   ";
    Color color = NORMAL, backg = BLACK;
    if (Board::cell(cell) == PLAYER_1) {
        color = RED;
        symbol = " X ";
    }
    else if (Board::cell(cell) == PLAYER_2) {
        color = CYAN;
        symbol = " O ";
    }

    if (Board::cellHL(cell) != NOBODY) backg = Color(color - 8);

    printxy(GRID.X - 1 + 4*cell.X, GRID.Y + 2*cell.Y, symbol, color, backg);
}


void Display::FilledCells() {
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            Cell({x, y});
        }
    }
}


void Display::Turn(Player player) {
    setcolor(player == 1 ? RED : CYAN);
    printxy(GRID.X - 4, GRID.Y - 4, player == 1 ? "+>> Player One's turn <<+" : "/(( Player Two's turn ))\\");
    printxy(3, 2, TOPLEFT_CORNER);
    printxy(3, SCR.Y - 3, BOTTOMLEFT_CORNER);
    printxy(SCR.X - 4, 2, TOPRIGHT_CORNER);
    printxy(SCR.X - 4, SCR.Y - 3, BOTTOMRIGHT_CORNER);
    setcolor(NORMAL);
}


void Display::TitleScreen(bool drawTitle, bool shiftPointer) {
    if (drawTitle) {
        //Corner ornaments
        printxy(3, 2, TOPLEFT_CORNER);
        printxy(3, SCR.Y - 3, BOTTOMLEFT_CORNER);
        printxy(SCR.X - 4, 2, TOPRIGHT_CORNER);
        printxy(SCR.X - 4, SCR.Y - 3, BOTTOMRIGHT_CORNER);
        printxy(SCR.X - 12, SCR.Y - 2, "by Samrux", GRAY);
        Wait(200);

        setcolor(RED);
        printxy(LOGO.X, LOGO.Y + 0, "     ::::::::::");
        printxy(LOGO.X, LOGO.Y + 1, "    :+:    :+: ");
        printxy(LOGO.X, LOGO.Y + 2, "   +:+         ");
        printxy(LOGO.X, LOGO.Y + 3, "  +#++:++#+    ");
        printxy(LOGO.X, LOGO.Y + 4, "        +#+    ");
        printxy(LOGO.X, LOGO.Y + 5, "#+#    #+#     ");
        printxy(LOGO.X, LOGO.Y + 6, "########       ");

        printxy(LOGO.X + 12, LOGO.Y + 4, "+#++:++#++:++", GRAY);

        for (int i = 0; i < 3; i++) {
            setcolor(i % 2 == 0 ? CYAN : RED);
            printxy(LOGO.X + 27 + 12*i, LOGO.Y + 0,  " :::::::::::");
            printxy(LOGO.X + 26 + 12*i, LOGO.Y + 1, "     :+:     ");
            printxy(LOGO.X + 26 + 12*i, LOGO.Y + 2, "    +:+      ");
            printxy(LOGO.X + 26 + 12*i, LOGO.Y + 3, "   +#+       ");
            printxy(LOGO.X + 26 + 12*i, LOGO.Y + 4, "  +#+        ");
            printxy(LOGO.X + 26 + 12*i, LOGO.Y + 5, " #+#         ");
            printxy(LOGO.X + 26 + 12*i, LOGO.Y + 6, "###          ");
        }

        printxy(SCR.X/2 - 9, LOGO.Y + 9, "<@ 5-Tic-Tac-Toe @>", WHITE);
        printxy(LOGO.X + 60, LOGO.Y + 10, GAME_VERSION, GRAY);

        printxy(SCR.X/2 - 9, SCR.Y - 5, "Press Enter to start");
    }

    setcolor(GREEN);
    printxy(SCR.X/2 - 13, SCR.Y - 5, shiftPointer ? " >>" : ">> ");
    printxy(SCR.X/2 + 12, SCR.Y - 5, shiftPointer ? "<< " : " <<");
    setcolor(NORMAL);
}


void Display::HelpScreen() {
    ClearGameScreen();

    printxy(SCR.X/2 - 11, HELP.Y - 3, "-<@  Instructions  @>-", GREEN);
    printxy(HELP.X, HELP.Y + 0, "The game is played by two people, using the same computer.");
    printxy(HELP.X, HELP.Y + 1, "You play in a 5x5 board, Player 1 using X, player 2 using O.");
    printxy(HELP.X, HELP.Y + 2, "To win, you must make more lines of three symbols than your");
    printxy(HELP.X, HELP.Y + 3, "opponent.");
    printxy(HELP.X, HELP.Y + 4, "But be careful! If a player makes a line of four, they will");
    printxy(HELP.X, HELP.Y + 5, "win the game instantly. Don't lose focus.");
    printxy(HELP.X, HELP.Y + 7, "Use your wits and develop the best strategies!");

    printxy(HELP.X + 40, HELP.Y + 1, "X", RED);
    printxy(HELP.X + 58, HELP.Y + 1, "O", CYAN);
    printxy(HELP.X + 27, HELP.Y + 2, "lines of three symbols", WHITE);
    printxy(HELP.X + 44, HELP.Y + 4, "four", WHITE);

    printxy(SCR.X/2 - 13, SCR.Y - 5, "{ Press any key to begin }", GRAY);
}


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


void Display::Score(int score[4], bool drawBox) {
    if (drawBox) {
        setcolor(WHITE);
        printxy(SCORE.X, SCORE.Y, TOPLEFT_CORNER);
        for (int ps = 1; ps <= 13; ps++) printxy(SCORE.X + ps, SCORE.Y, HORIZONTAL_LINE);
        printxy(SCORE.X + 14, SCORE.Y, TOPRIGHT_CORNER);
        printxy(SCORE.X + 4, SCORE.Y, "<Score>");
        printxy(SCORE.X, SCORE.Y + 1, VERTICAL_LINE);
        printxy(SCORE.X, SCORE.Y + 2, NOLEFT_JOINT);
        printxy(SCORE.X, SCORE.Y + 3, NOLEFT_JOINT);
        printxy(SCORE.X, SCORE.Y + 4, NOLEFT_JOINT);
        printxy(SCORE.X, SCORE.Y + 5, VERTICAL_LINE);
        printxy(SCORE.X, SCORE.Y + 6, BOTTOMLEFT_CORNER);

        printxy(SCORE.X + 2, SCORE.Y + 2, "Player 1:   ", RED);
        printxy(SCORE.X + 2, SCORE.Y + 3, "Player 2:   ", CYAN);
        printxy(SCORE.X + 2, SCORE.Y + 4, "Ties:       ", GREEN);
    }

    printxy(SCORE.X + 12, SCORE.Y + 2, score[PLAYER_1]);
    printxy(SCORE.X + 12, SCORE.Y + 3, score[PLAYER_2]);
    printxy(SCORE.X + 12, SCORE.Y + 4, score[TIE]);
}


void Display::WinScreen(Player winner, int threes[2]) {
    printxy(GRID.X-3, GRID.Y-4, "                         "); // Erases turn text

    Color color = KEEP;
    const char* message = "";
    if (winner == PLAYER_1) {
        color = RED;
        message = "  <@+ Player 1 won! +@> ";
    }
    else if (winner == PLAYER_2) {
        color = CYAN;
        message = "  ~@{ Player 2 won! }@~ ";
    }
    else if (winner == TIE) {
        color = GREEN;
        message = "   -=] It's a tie! [=-  ";
    }

    setcolor(color);

    printxy(GRID.X - 4, GRID.Y - 4, message);
    printxy(GRID.X - 4, GRID.Y + 11, message);

    // Ornamented corners
    const Coord c1 = {3, 2}, c2 = {SCR.X-4, SCR.Y-3};
    printxy(c1.X, c1.Y, TOPLEFT_CORNER);
    printxy(c1.X, c2.Y, BOTTOMLEFT_CORNER);
    printxy(c2.X, c1.Y, TOPRIGHT_CORNER);
    printxy(c2.X, c2.Y, BOTTOMRIGHT_CORNER);
    for (int x = 1; x <= 8; x++) printxy(c1.X+x, c1.Y, HORIZONTAL_LINE);
    for (int x = 1; x <= 8; x++) printxy(c1.X+x, c2.Y, HORIZONTAL_LINE);
    for (int x = 1; x <= 8; x++) printxy(c2.X-x, c1.Y, HORIZONTAL_LINE);
    for (int x = 1; x <= 8; x++) printxy(c2.X-x, c2.Y, HORIZONTAL_LINE);
    for (int y = 1; y <= 4; y++) printxy(c1.X, c1.Y+y, VERTICAL_LINE);
    for (int y = 1; y <= 4; y++) printxy(c2.X, c1.Y+y, VERTICAL_LINE);
    for (int y = 1; y <= 4; y++) printxy(c1.X, c2.Y-y, VERTICAL_LINE);
    for (int y = 1; y <= 4; y++) printxy(c2.X, c2.Y-y, VERTICAL_LINE);

    FilledCells(); // Highlight


    const Coord nt = {5, 12}; // Number of threes window
    const Coord rm = {(SCR.X + GRID.X + 20)/2 - 11, SCR.Y/2 - 2 }; // Restart message window

    utils::ClearScreen(CTRL, { CTRL.X + 25, CTRL.Y + 11 }); // Clear controls box area

    //'Threes' window
    if (threes[0] < 0 || threes[1] < 0){
        threes[0] = Board::CountThrees(PLAYER_1);
        threes[1] = Board::CountThrees(PLAYER_2);
    }

    setcolor(WHITE);
    for(int ps = 1; ps <= 12; ps++) printxy(nt.X+ps, nt.Y, HORIZONTAL_LINE);
    printxy(nt.X   , nt.Y  , TOPLEFT_CORNER);
    printxy(nt.X+13, nt.Y  , TOPRIGHT_CORNER);
    printxy(nt.X+3 , nt.Y  , "<Threes>");
    printxy(nt.X   , nt.Y+1, VERTICAL_LINE);
    printxy(nt.X   , nt.Y+2, NOLEFT_JOINT);
    printxy(nt.X   , nt.Y+3, VERTICAL_LINE);
    printxy(nt.X   , nt.Y+4, NOLEFT_JOINT);
    printxy(nt.X   , nt.Y+5, VERTICAL_LINE);
    printxy(nt.X   , nt.Y+6, BOTTOMLEFT_CORNER);
    printxy(nt.X+4 , nt.Y+2, threes[0], RED);
    printxy(nt.X+7 , nt.Y+3, '/', WHITE);
    printxy(nt.X+10, nt.Y+4, threes[1], CYAN);

    //Melody
    Boop(400, 100);
    Boop(300, 100);
    Boop(500, 500);
    Wait(400);

    CleanKbBuffer();

    //Message window
    setcolor(WHITE);
    printxy(rm.X   , rm.Y  , TOPLEFT_CORNER);
    printxy(rm.X+19, rm.Y  , TOPRIGHT_CORNER);
    printxy(rm.X+19, rm.Y+1, VERTICAL_LINE);
    printxy(rm.X+19, rm.Y+2, VERTICAL_LINE);
    printxy(rm.X+19, rm.Y+3, BOTTOMRIGHT_CORNER);
    for (int ps = 1; ps <= 18; ps++) printxy(rm.X + ps, rm.Y, HORIZONTAL_LINE);
    setcolor(NORMAL);
    printxy(rm.X+3, rm.Y+1, "Press       to");
    printxy(rm.X+3, rm.Y+2, " restart game ");
    printxy(rm.X+9, rm.Y+1, "Enter", WHITE);
}
