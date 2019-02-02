#include "Utils.h"
#include "Game.h"
using namespace utils;


void Perform::ProgramSetup() {
#ifdef _WIN32
    SetConsoleTitleA("5-Tic-Tac-Toe");

    //Make console cursor invisible
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = false;
    cursor.dwSize = 10;
    SetConsoleCursorInfo(CONSOLE_OUTPUT, &cursor);

    //Disable selection, which breaks the program when clicking
    DWORD mode;
    GetConsoleMode(CONSOLE_INPUT, &mode);
    SetConsoleMode(CONSOLE_INPUT, mode & ~ENABLE_QUICK_EDIT_MODE);

#elif __linux__
    initscr();
    noecho();
    cbreak();
#endif

    Coord consoleSize = utils::GetConsoleSize();

    if (consoleSize.X < 78 || consoleSize.Y < 23) { // Minimum size before things start to break
        consoleSize = { consoleSize.X>=78 ? consoleSize.X : 80, consoleSize.Y>=23 ? consoleSize.Y : 25 };
        if (!utils::ResizeConsole(consoleSize)) {
            printxy(ORIGIN, "Can't resize console");
            WaitForEnter();
        }
    }

    Display::SetScrCoords(consoleSize);
}


void Perform::OpenTitleScreen() {
    Display::TitleScreen(Display::REDRAW);

    int input = 0;
    bool shiftPointer = false;

    CleanKbBuffer();
    while(input != ENTER_KEY) {
        Display::TitleScreen(Display::NO_REDRAW, shiftPointer);
        shiftPointer = !shiftPointer;
        Wait(500);
        input = KeyPressed();
        CleanKbBuffer();
    }

    Boop(500, 100);
    Display::ClearGameScreen();
}


void Perform::OpenInstructions() {
    Display::HelpScreen();
    AwaitKeyPress();
    CleanKbBuffer();
    Display::ClearGameScreen();
}
