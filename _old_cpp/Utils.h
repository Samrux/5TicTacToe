#ifndef Sam_Utils
#define Sam_Utils

#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include <conio.h>
#elif __linux__
#include <ncurses.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif

#include <chrono>
#include <thread>


// Ascii  -  Believe me I would use an enum if I could
#define HORIZONTAL_LINE          (char)196
#define VERTICAL_LINE            (char)179
#define TOPLEFT_CORNER           (char)218
#define TOPRIGHT_CORNER          (char)191
#define BOTTOMLEFT_CORNER        (char)192
#define BOTTOMRIGHT_CORNER       (char)217
                                 
#define CROSS_JOINT              (char)197
#define NOLEFT_JOINT             (char)195
#define NORIGHT_JOINT            (char)180
#define NOUP_JOINT               (char)194
#define NODOWN_JOINT             (char)193
                                 
#define HORIZONTAL_DOUBLELINE    (char)205
#define VERTICAL_DOUBLELINE      (char)186
#define TOPLEFT_DOUBLECORNER     (char)201
#define TOPRIGHT_DOUBLECORNER    (char)187
#define BOTTOMLEFT_DOUBLECORNER  (char)200
#define BOTTOMRIGHT_DOUBLECORNER (char)188
                                 
#define CHAR_BLOCK               (char)254


namespace utils
{
    #ifdef _WIN32
    const HANDLE CONSOLE_OUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
    const HANDLE CONSOLE_INPUT = GetStdHandle(STD_INPUT_HANDLE);
    #endif

    enum Color {
        BLACK,
        DARK_BLUE,
        GREEN,
        DARK_CYAN,
        DARK_RED,
        PURPLE,
        DARK_YELLOW,
        NORMAL,
        GRAY,
        BLUE,
        LIME,
        CYAN,
        RED,
        PINK,
        YELLOW,
        WHITE,
        KEEP,

        RESET = NORMAL
    };

    /*static const int AnsiColor[17] = { // Corresponds to the Color enum
        30, 34, 32, 36, 31, 35, 33, 0, 90, 94, 92, 96, 91, 95, 93, 97, -1
    };*/


    enum Keys {
        UP_KEY    = 72,
        DOWN_KEY  = 80,
        RIGHT_KEY = 77,
        LEFT_KEY  = 75,
        ENTER_KEY = 13,
        SPACEBAR  = 32,
        BACKSPACE =  8,
        ESC_KEY   = 27,
        F1_KEY = 59, F2_KEY, F3_KEY, F4_KEY, F5_KEY, F6_KEY, F7_KEY, F8_KEY, F9_KEY, F10_KEY,
        F11_KEY = 133, F12_KEY
    };


    struct Coord{
        int X;
        int Y;

        Coord(int x=0, int y=0) : X(x), Y(y) {}

        void swap(); //Swaps the X and Y coordinates

        bool operator ==(const Coord);
        void operator +=(const Coord);
        void operator -=(const Coord);
        Coord operator +(const Coord);
        Coord operator -(const Coord);

        int operator >> (const Coord); //Returns the area of a square formed between two points
    };

    const Coord ORIGIN = {0, 0};



    void gotoxy(
        int,
        int
    );
    void gotoxy(
        Coord
    );

    void setcolor(
        Color,      // New Color in which to print following text.
        Color=BLACK // Color of the background of the text
    );

    int AwaitKeyPress(
        void
    );

    int KeyPressed(
        void
    );

    void Wait(
        int  // Time to wait in milliseconds
    );

    void Boop(
        int, // Tone
        int  // Duration
    );

    void WaitForEnter(
        void
    );
 

    template <typename var>void printxy(
        int,        // X coordinate at which to write
        int,        // Y coordinate at which to write
        var,        // Given content to print
        Color=KEEP, // Color in which to print text; unaltered by default
        Color=BLACK // Color of the background; black by default
    );
    template <typename var>void printxy(
        Coord,        // Coordinates at which to write
        var,          // Given content to print
        Color = KEEP, // Color in which to print text; unaltered by default
        Color = BLACK // Color of the background; black by default
    );


    void CleanKbBuffer(
        void
    );

    void ClearScreen(
        Coord,     // Coordinates at which to start cleaning
        Coord,     // Coordinates at which to stop cleaning
        int=0 // Whether to use a slower wipe effect
    );

    Coord GetConsoleSize();

    bool ResizeConsole(
        int, // Width of the screen
        int  // Height of the screen

    );
    bool ResizeConsole(
        Coord // Dimensions of the screen
    );

}


#endif // Sam_Utils