#include "Utils.h"
#include "Game.h"
using namespace utils;


int Board::nCellsFilled = 0;
Player Board::c[5][5] = { {NOBODY} };
Player Board::hl[5][5] = { {NOBODY} };
Coord Board::pointer = { 2, 2 };


Player Board::cell(int x, int y) {
    return c[x][y];
}
Player Board::cell(Coord coord) {
    return c[coord.X][coord.Y];
}


Player Board::cellHL(int x, int y) {
    return hl[x][y];
}
Player Board::cellHL(Coord coord) {
    return hl[coord.X][coord.Y];
}


Player Board::selectedcell() {
    return c[pointer.X][pointer.Y];
}


void Board::FillCell(Player player) {
    nCellsFilled++;
    c[pointer.X][pointer.Y] = player;
    Display::Cell(pointer);
}


void Board::Highlight(Player player, int x, int y, LineDir dir, int amount) {
    int a = 0, b = 0; // x and y offset
    switch (dir) {
        case HORIZONTAL:   a = -1; break;
        case VERTICAL:     b = -1; break;
        case DIAGONAL_TTL: a = +1; b = -1; break;
        case DIAGONAL_TTR: a = -1; b = -1; break;
    }

    for (int i = 0; i < amount; i++) hl[x + i*a][y + i*b] = player;
}


void Board::Reset() {
    nCellsFilled = 0;
    pointer = { 2, 2 };
    
    for(int x = 0; x < 5; x++) {
        for(int y = 0; y < 5; y++) {
            c[x][y] = NOBODY;
            hl[x][y] = NOBODY;
        }
    }
}


Player Board::FindWinner() {
    Player player = NOBODY;
    int line = 0; // Consecutive symbols in a given line

    //Horizontals
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            if (c[x][y] != player) {
                line = 1;
                player = c[x][y];
            }
            else line++;

            if (line == 4 && player != NOBODY) {
                Highlight(player, x,y, HORIZONTAL, 4);
                return player;
            }
        }
        line = 0;
    }

    //Verticals
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            if (c[x][y] != player) {
                line = 1;
                player = c[x][y];
            }
            else line++;

            if (line == 4 && player != NOBODY) {
                Highlight(player, x, y, VERTICAL, 4);
                return player;
            }
        }
        line = 0;
    }

    //Top-to-left diagonals
    for (int d = 3; d <= 5; d++) { // This will loop through the fourth to sixth diagonal
        for (int y = 0; y <= d; y++) {
            int x = d - y;
            if (x < 5 && y < 5) {
                if (c[x][y] != player) {
                    line = 1;
                    player = c[x][y];
                }
                else line++;

                if (line == 4 && player != NOBODY) {
                    Highlight(player, x, y, DIAGONAL_TTL, 4);
                    return player;
                }
            }
        }
        line = 0;
    }

    //Top-to-right diagonals
    for (int d = 0; d < 9; d++) {
        for (int y = 0; y <= d; y++) {
            int x = 4 - d + y;
            if (x >= 0 && y < 5) {
                if (c[x][y] != player) {
                    line = 1;
                    player = c[x][y];
                }
                else line++;

                if (line == 4 && player != NOBODY) {
                    Highlight(player, x, y, DIAGONAL_TTR, 4);
                    return player;
                }
            }
        }
        line = 0;
    }

    return NOBODY; // If all else failed
}


int Board::CountThrees(Player player) {
    int line = 0; // Consecutive symbols in a given line
    int count = 0;

    //Horizontals
    for(int y = 0; y < 5; y++) {
        for(int x = 0; x < 5; x++) {
            if (c[x][y] == player) line++;
            else line = 0;

            if(line == 3) {
                count++;
                Highlight(player, x, y, HORIZONTAL, 3);
                break;
            }
        }
        line = 0;
    }

    //Verticals
    for(int x = 0; x < 5; x++) {
        for(int y = 0; y < 5; y++) {
            if (c[x][y] == player) line++;
            else line = 0;

            if(line == 3) {
                count++;
                Highlight(player, x, y, VERTICAL, 3);
                break;
            }
        }
        line = 0;
    }

    //Top-to-left diagonals
    for (int d = 2; d <= 6; d++) { // This will loop through the third to seventh diagonal
        for (int y = 0; y <= d; y++) {
            int x = d - y;
            if (x < 5 && y < 5) {
                if (c[x][y] == player) line++;
                else line = 0;

                if (line == 3) {
                    count++;
                    Highlight(player, x, y, DIAGONAL_TTL, 3);
                    break;
                }
            }
        }
        line = 0;
    }

    //Top-to-right diagonals
    for (int d = 2; d <= 6; d++) {
        for (int y = 0; y <= d; y++) {
            int x = 4 - d + y;
            if (x >= 0 && y < 5) {
                if (c[x][y] == player) line++;
                else line = 0;

                if (line == 3) {
                    count++;
                    Highlight(player, x, y, DIAGONAL_TTR, 3);
                    break;
                }
            }
        }
        line = 0;
    }


    return count;
}
