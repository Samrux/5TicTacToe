#include "Utils.h"
using namespace utils;


//Coords

void utils::Coord::swap() {
    Coord old(X,Y);
    X = old.Y;
    Y = old.X;
}

bool utils::Coord::operator == (const Coord other) {
    return (X == other.X && Y == other.Y);
}

void utils::Coord::operator += (const Coord other) {
    X += other.X;
    Y += other.Y;
}

void utils::Coord::operator -= (const Coord other) {
    X -= other.X;
    Y -= other.Y;
}

Coord utils::Coord::operator + (const Coord other) {
    return Coord(X+other.X, Y+other.Y);
}

Coord utils::Coord::operator - (const Coord other) {
    return Coord(X-other.X, Y-other.Y);
}

int utils::Coord::operator >> (const Coord other) {
    int area = (other.X - X) * (other.Y - Y);
    if(area < 0) area = -area;
    return area;
}



void utils::gotoxy(int x, int y) {
#ifdef _WIN32
    SetConsoleCursorPosition(CONSOLE_OUTPUT, {(short int)x, (short int)y});
#elif __linux__
    move(x, y);
    refresh();
#endif
}
void utils::gotoxy(Coord coord) {
    gotoxy(coord.X, coord.Y);
}


void utils::setcolor(Color foreground, Color background) {
#ifdef _WIN32
    SetConsoleTextAttribute(CONSOLE_OUTPUT, (foreground + (background * 16)));
#elif __linux__
    // todo
#endif
}


int utils::AwaitKeyPress() {
#ifdef _WIN32
    return _getch();

#elif __linux__
    nodelay(stdscr, FALSE);
    int ch = getch();
    nodelay(stdscr, TRUE);
    return ch;
#endif
}


int utils::KeyPressed() {
#ifdef _WIN32
    if (_kbhit()) return _getch();
    else return false;

#elif __linux__
    nodelay(stdscr, TRUE);
    int key = getch();
    return key == ERR ? false : key;
#endif
}


void utils::Wait(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}


void utils::Boop(int tone, int duration) {
#ifdef _WIN32
    Beep(tone, duration);
#endif
}


void utils::WaitForEnter() {
    CleanKbBuffer();
    while(AwaitKeyPress() != ENTER_KEY);
}


template <typename var>
void utils::printxy(int x, int y, var text, Color txtClr, Color bgClr) {
    gotoxy(x, y);
    if (txtClr != KEEP) setcolor(txtClr, bgClr);
#ifdef _WIN32
    std::cout << text;
#elif __linux__
    // todo, ncurses is stupid
#endif
    if (txtClr != KEEP) setcolor(NORMAL, BLACK);
    gotoxy(ORIGIN);
}
template void utils::printxy<int>(int, int, int, Color, Color);
template void utils::printxy<char>(int, int, char, Color, Color);
template void utils::printxy<const char*>(int, int, const char*, Color, Color);


template <typename var>
void utils::printxy(Coord c, var text, Color txtClr, Color bgClr) {
    printxy(c.X, c.Y, text, txtClr, bgClr);
}
template void utils::printxy<int>(Coord, int, Color, Color);
template void utils::printxy<char>(Coord, char, Color, Color);
template void utils::printxy<const char*>(Coord, const char*, Color, Color);


void utils::CleanKbBuffer() {
#ifdef _WIN32
    while(_kbhit()) _getch();
#endif
}


void utils::ClearScreen(Coord start, Coord end, int wipeDelay) {
    for (int x = start.X; x <= end.X; x++) {
        for (int y = start.Y; y <= end.Y; y++) {
            printxy(x, y, ' ');
        }
        if (wipeDelay > 0 && x%2 == 0) Wait(wipeDelay); // Effect
    }
}


Coord utils::GetConsoleSize() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(CONSOLE_OUTPUT, &bufferInfo)) throw std::exception("Can't get buffer info");
    return Coord(bufferInfo.srWindow.Right + 1, bufferInfo.srWindow.Bottom + 1);

#elif __linux__
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return Coord(size.ws_row, size.ws_col);
#endif
}


bool utils::ResizeConsole(int x, int y) {
#ifdef _WIN32
    COORD largestSize = GetLargestConsoleWindowSize(CONSOLE_OUTPUT);
    if (x > largestSize.X || y > largestSize.Y) return false;

    Coord windowSize = GetConsoleSize();
    if (windowSize.X > x || windowSize.Y > y) {
        // Window size needs to be adjusted before the buffer size can be reduced.
        SMALL_RECT info = { 0, 0, x<windowSize.X ? x-1 : windowSize.X-1, y<windowSize.Y ? y-1 : windowSize.Y-1 };

        if (!SetConsoleWindowInfo(CONSOLE_OUTPUT, TRUE, &info)) return false;
    }

    COORD size = {x, y};
    if (!SetConsoleScreenBufferSize(CONSOLE_OUTPUT, size)) return false;

    SMALL_RECT info = {0, 0, x - 1, y - 1};
    if (!SetConsoleWindowInfo(CONSOLE_OUTPUT, TRUE, &info)) return false;

    return true;

#elif __linux__
    return true;
#endif
}
bool utils::ResizeConsole(Coord size) {
    return ResizeConsole(size.X, size.Y);
}
