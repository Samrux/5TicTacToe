/***********************************************************\
 *                                                         *
 *                5-Tic-Tac-Toe -- C++ Game                *
 *                   Made by Samrux (2015)                 *
\***********************************************************/
const bool Debug = false;



#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>



//KEYBOARD KEYS
#define UP                        72
#define DOWN                      80
#define RIGHT                     77
#define LEFT                      75
#define ENTER                     13
#define SPACE                     32
#define BACKSPACE                  8
#define F5                        63
#define F10                       68

//ASCII CHARACTERS
#define HORIZONTAL_LINE          196
#define VERTICAL_LINE            179
#define TOPLEFT_CORNER           218
#define TOPRIGHT_CORNER          191
#define BOTTOMLEFT_CORNER        192
#define BOTTOMRIGHT_CORNER       217

#define CROSS_JOINT              197
#define NOLEFT_JOINT             195
#define NORIGHT_JOINT            180
#define NOUP_JOINT               194
#define NODOWN_JOINT             193

#define HORIZONTAL_DOUBLELINE    205
#define VERTICAL_DOUBLELINE      186
#define TOPLEFT_DOUBLECORNER     201
#define TOPRIGHT_DOUBLECORNER    187
#define BOTTOMLEFT_DOUBLECORNER  200
#define BOTTOMRIGHT_DOUBLECORNER 188

//MACROS
#define PROGRAM_RUNNING            1
#define REDRAW_TRUE                1
#define REDRAW_FALSE               0
#define SKIP_TO_NEXT_LINE          6

//USER KEYWORDS
#define yes                       !0
#define no                         0
#define none                       0
#define empty                      0





//Defining custom variable types with enum:
// * A 'color' type variable can be one of 16 words, that are automatically passed to Windows as numbers 0-15.
//   (but 'NONE' will actually signal the program to not change color when given it)
// * A 'player' variable can be one of two players, nobody, or a tie.

enum color{

	NONE,
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
	WHITE

};

enum player{

	NOBODY,
	PLAYER_1,
	PLAYER_2,
	TIE

};




//A constant to pass onto Windows functions
const HANDLE  CONSOLE_OUTPUT  =  GetStdHandle(STD_OUTPUT_HANDLE);


using std::string;











class Display	{

	void gotoxy     (int x, int y);
	void setcolor   (color newColor);
	void printxy    (int coordX, int coordY, string Text, color newColor = NONE);
	void printCh    (int coordX, int coordY, char Character);
	void printNum   (int coordX, int coordY, int Number);

	int  Background;


 public:

	void Borders    ();
	void Grid       ();
	void TitleScreen(bool DrawTitle, bool shiftPointer = no);
	void HelpScreen ();
	void WinScreen  (player Player);
	void Turn       (player Player);
	void Score      (int score[4], bool DrawBox = no);
	void Controls   (bool DrawBigWindow, bool FirstLaunch = no);
	void Cell       (player Player);
	void FilledCells();
	void Pointer    (int newpx, int newpy, player Player);
	void ClearScreen();


}Display;


class Grid		{

	int  cell     [6][6];
	int  highlight[6][6];

	void doHighlight(player, int x1, int y1, int x2, int y2, int x3, int y3,
	                         int x4=0, int y4=0 //ignores fourth coordinates if none are given
	                );


 public:

	void fillCell    (player Player);
	int  c           (int x, int y);
	int  cHL         (int x, int y);
	int  selectedCell();
	void Reset       ();

	bool findsWinner (player Player);
	int  nOfThrees   (player Player);

	int nCellsFilled;
	COORD p; //pointer, selected cell


}Board;



class Perform	{
//some actions for the main function to activate

public:
	void ProgramSetup       ();
	void OpenTitleScreen    ();
	void OpenInstructions   ();
	void CleanKeyboardBuffer();
	void WaitForEnter       ();


}perform;







int main()
{


	perform.ProgramSetup();



	if(Debug == no){

	perform.OpenTitleScreen ();

	perform.OpenInstructions();

	}






	//Creating game variables

	player currentPlayer, lastPlayer, winner;
	bool   EndMatch, openControlsWindow = yes, FirstLaunch = yes;
	int    score[4] = {0,0,0,0};
	char   keyboardInput;


	while(PROGRAM_RUNNING)
	{

		//Setting up the match about to begin

		if(lastPlayer == PLAYER_1) currentPlayer = PLAYER_2;
		else                       currentPlayer = PLAYER_1;

		EndMatch = no;
		winner   = NOBODY;

		Board.Reset();


		Display.Grid    (                    );
		Display.Turn    ( currentPlayer      );
		Display.Score   ( score, REDRAW_TRUE );
		Display.Controls( openControlsWindow, FirstLaunch );

		if(FirstLaunch) FirstLaunch = no;

		Board.p.X = 3;
		Board.p.Y = 3;
		Display.Pointer ( Board.p.X, Board.p.Y, currentPlayer );





		while(EndMatch == no){


				keyboardInput = getch(); //Waits for a keyboard press

				switch(keyboardInput)
				{

					case ENTER:
					case SPACE:
						if( Board.selectedCell() == empty ){

							Board.fillCell(currentPlayer);

							if( Board.findsWinner(currentPlayer) )
							{
								EndMatch = yes;
								winner   = currentPlayer;
							}

							else{

								Beep(250, 50);

								if(currentPlayer == PLAYER_1) currentPlayer = PLAYER_2;
								else                          currentPlayer = PLAYER_1;

								Display.Pointer( Board.p.X, Board.p.Y, currentPlayer );
								Display.Turn   ( currentPlayer );

							}


						}break;





					case UP:
						if(Board.p.Y > 1){
							Display.Pointer( Board.p.X  , Board.p.Y-1, currentPlayer );
							Board.p.Y--;
							Beep(600, 25);

						}break;



					case DOWN:
						if(Board.p.Y < 5){
							Display.Pointer( Board.p.X  , Board.p.Y+1, currentPlayer );
							Board.p.Y++;
							Beep(300, 25);

						}break;



					case LEFT:
						if(Board.p.X > 1){
							Display.Pointer( Board.p.X-1, Board.p.Y  , currentPlayer );
							Board.p.X--;
							Beep(400, 25);

						}break;



					case RIGHT:
						if(Board.p.X < 5){
							Display.Pointer( Board.p.X+1, Board.p.Y  , currentPlayer );
							Board.p.X++;
							Beep(500, 25);

						}break;





					case BACKSPACE:
						openControlsWindow = !openControlsWindow;

						Display.Controls( openControlsWindow );
						Beep(250, 50);
						break;


					case 'h':
						Beep(700, 100);
						perform.OpenInstructions();
						//redisplays entire game
						Display.Grid       (                    );
						Display.FilledCells(                    );
						Display.Controls   ( openControlsWindow );
						Display.Turn       ( currentPlayer      );
						Display.Score      ( score, REDRAW_TRUE );
						Display.Pointer    ( Board.p.X, Board.p.Y, currentPlayer );

						break;


					case F5:
						Beep(400, 100);
						Beep(400, 100);
						EndMatch      = yes;
						currentPlayer = NOBODY;
						break;


					case F10:
						EndMatch           = yes;
						openControlsWindow = no;
						currentPlayer      = NOBODY;

						for(int i = 1; i <= 3; i++)
							score[i] = 0;

						Beep(200, 100);
						Beep(400, 500);

						Display.ClearScreen    ();
						perform.OpenTitleScreen();
						break;



					default: break;



				}

				if(Board.nCellsFilled == 25) EndMatch = yes;

		} //End match


		//Skips the winning screen if the match was forced to end
		if(currentPlayer != NOBODY){


			//If the game ended without a winner (full board, a tie), count the threes and decide on that
			if(winner == NOBODY){

				if     ( Board.nOfThrees(PLAYER_1) > Board.nOfThrees(PLAYER_2) )
				winner = PLAYER_1;

				else if( Board.nOfThrees(PLAYER_1) < Board.nOfThrees(PLAYER_2) )
				winner = PLAYER_2;

				else
				winner = TIE;

			}

			score[winner]++;
			Display.Score( score, REDRAW_FALSE );





			Display.WinScreen(winner);
			perform.WaitForEnter();





			//Decides next first player

			if(winner != TIE)
				lastPlayer = winner;

			else{

				if(lastPlayer == PLAYER_1) lastPlayer = PLAYER_2;
				else                       lastPlayer = PLAYER_1;

			}



		} //End winscreen

	} //Game will loop forever
} //End of int main()







void Perform::ProgramSetup       ()                                             {

	SetConsoleTitle( "5-Tic-Tac-Toe" );

//Set the window size
	COORD windowSize;
	windowSize.X = 80;
	windowSize.Y = 25;

	SetConsoleScreenBufferSize( CONSOLE_OUTPUT, windowSize );


//Make console cursor invisible
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible       = no;
	cursor.dwSize         = 10;

	SetConsoleCursorInfo      ( CONSOLE_OUTPUT,   &cursor  );



	Display.Borders();

}

void Perform::OpenTitleScreen    ()                                             {

	Display.TitleScreen(REDRAW_TRUE);


	char input        = none;
	bool shiftPointer =   no;

	CleanKeyboardBuffer();

	while( input != ENTER )
	{
		Display.TitleScreen( REDRAW_FALSE, shiftPointer );

		shiftPointer = !shiftPointer;
		Sleep(500);
		if(kbhit()) input = getch(); //Constantly scans for keyboard presses

		CleanKeyboardBuffer();

	}

	Beep(500, 100);

}

void Perform::OpenInstructions   ()                                             {

	Display.HelpScreen();
	getch();

	CleanKeyboardBuffer();

}

void Perform::CleanKeyboardBuffer()                                             {

	while(kbhit()) getch();

}

void Perform::WaitForEnter       ()                                             {

	CleanKeyboardBuffer();
	while( getch() != ENTER );

}






void Display::gotoxy      (int x, int y)                                        {

	COORD newCoordinates;
	newCoordinates.X = x;
	newCoordinates.Y = y;

	SetConsoleCursorPosition( CONSOLE_OUTPUT, newCoordinates );

}
void Display::setcolor    (color newColor)                                      {

	if(newColor == NORMAL) Background = 0;

	SetConsoleTextAttribute( CONSOLE_OUTPUT, (newColor + (Background * 16)) );


}
void Display::printxy     (int coordX, int coordY, string Text, color newColor) {

	gotoxy(coordX, coordY);

	if(newColor != NONE) setcolor(newColor);

	std::cout << Text;

	if(newColor != NONE) setcolor(NORMAL);

	gotoxy(0,0);

}
void Display::printCh     (int coordX, int coordY, char Character)              {

	gotoxy(coordX, coordY);
	std::cout << Character;
	gotoxy(0,0);

}
void Display::printNum    (int coordX, int coordY, int Number)                  {

	gotoxy(coordX, coordY);
	std::cout << Number;
	gotoxy(0,0);

}


void Display::Borders     ()                                                    {


	if(!Debug)
	{
		setcolor(NORMAL);


		for(int x = 2; x < 78; x++){
			printCh( x, 1,  HORIZONTAL_DOUBLELINE);
			Sleep(10);
		}

		Beep(200, 25);

		printCh(78, 1,  TOPRIGHT_DOUBLECORNER);

		for(int y = 2; y < 23; y++){
			printCh(78, y,  VERTICAL_DOUBLELINE);
			Sleep(34);
		}

		Beep(300, 25);

		printCh(78,23,  BOTTOMRIGHT_DOUBLECORNER);

		for(int x = 77; x > 1; x--){
			printCh( x,23,  HORIZONTAL_DOUBLELINE);
			Sleep(10);
		}

		Beep(400, 25);

		printCh( 1,23,  BOTTOMLEFT_DOUBLECORNER);

		for(int y = 22; y > 1; y--){
			printCh( 1, y,  VERTICAL_DOUBLELINE);
			Sleep(34);
		}

		printCh( 1, 1,  TOPLEFT_DOUBLECORNER);

		Beep(500, 25);


	}



	setcolor(WHITE);
	// Resetting border to white

	printCh( 1,23,  BOTTOMLEFT_DOUBLECORNER );
	printCh( 1, 1,  TOPLEFT_DOUBLECORNER    );


	for(int x = 2; x < 78; x++)
		printCh( x, 1,  HORIZONTAL_DOUBLELINE );

	for(int y = 2; y < 23; y++)
		printCh(78, y,	VERTICAL_DOUBLELINE   );

	for(int x = 2; x < 78; x++)
		printCh( x,23,  HORIZONTAL_DOUBLELINE );

	for(int y = 2; y < 23; y++)
		printCh( 1, y,  VERTICAL_DOUBLELINE   );


	printCh(78, 1,  TOPRIGHT_DOUBLECORNER    );
	printCh(78,23,  BOTTOMRIGHT_DOUBLECORNER );


	setcolor(NORMAL);

}

void Display::Grid        ()                                                    {

	ClearScreen();



	// Base position for the grid in the game screen
	const COORD grid = {30, 8};


	for(int jy = 1; jy <= 5; jy++)			// Vertical lines
		for(int jx = 1; jx <= 4; jx++)
			printCh( grid.X - 1 + 4*jx, grid.Y-2 + 2*jy,    VERTICAL_LINE   );

	for(int jy = 1; jy < 5; jy++) 			// Horizontal lines
		for(int jx = 1; jx < 20; jx++)
			printCh( grid.X - 1 + 1*jx, grid.Y-1 + 2*jy,    HORIZONTAL_LINE );

	for(int jy = 1; jy < 5; jy++) 			// Joints
		for(int jx = 1; jx < 5; jx++)
			printCh( grid.X - 1 + 4*jx, grid.Y-1 + 2*jy,    CROSS_JOINT     );




	printxy(27, 4,  " <@ Player    's turn @> ",  GRAY);

	setcolor(NORMAL);


}



void Display::TitleScreen (bool DrawTitle, bool shiftPointer)                   {

	// Base position for the content in the title screen
	const COORD logo = { 8,  4}; // Logo position
	const COORD text = {30, 19}; // Press Enter to Start position


	if(DrawTitle == yes)
	{

		//Corner ornaments
		printCh( 3, 2,  TOPLEFT_CORNER     );
		printCh( 3,22,  BOTTOMLEFT_CORNER  );
		printCh(76, 2,  TOPRIGHT_CORNER    );
		printCh(76,22,  BOTTOMRIGHT_CORNER );

		printxy(68,23, "by Samrux",   GRAY );

		Sleep(200);



		setcolor(RED);
		printxy(logo.X   , logo.Y+0, "     ::::::::::");
		printxy(logo.X   , logo.Y+1, "    :+:    :+: ");
		printxy(logo.X   , logo.Y+2, "   +:+         ");
		printxy(logo.X   , logo.Y+3, "  +#++:++#+    ");
		printxy(logo.X   , logo.Y+4, "        +#+    ");
		printxy(logo.X   , logo.Y+5, "#+#    #+#     ");
		printxy(logo.X   , logo.Y+6, "########       ");

		printxy(logo.X+12, logo.Y+4 , "+#++:++#++:++", GRAY);

		setcolor(CYAN);
		printxy(logo.X+27, logo.Y+0,  " :::::::::::"  );
		printxy(logo.X+26, logo.Y+1, "     :+:     "  );
		printxy(logo.X+26, logo.Y+2, "    +:+      "  );
		printxy(logo.X+26, logo.Y+3, "   +#+       "  );
		printxy(logo.X+26, logo.Y+4, "  +#+        "  );
		printxy(logo.X+26, logo.Y+5, " #+#         "  );
		printxy(logo.X+26, logo.Y+6, "###          "  );

		setcolor(RED);
		printxy(logo.X+39, logo.Y+0,  " :::::::::::"  );
		printxy(logo.X+38, logo.Y+1, "     :+:     "  );
		printxy(logo.X+38, logo.Y+2, "    +:+      "  );
		printxy(logo.X+38, logo.Y+3, "   +#+       "  );
		printxy(logo.X+38, logo.Y+4, "  +#+        "  );
		printxy(logo.X+38, logo.Y+5, " #+#         "  );
		printxy(logo.X+38, logo.Y+6, "###          "  );

		setcolor(CYAN);
		printxy(logo.X+51, logo.Y+0,  " :::::::::::"  );
		printxy(logo.X+50, logo.Y+1, "     :+:     "  );
		printxy(logo.X+50, logo.Y+2, "    +:+      "  );
		printxy(logo.X+50, logo.Y+3, "   +#+       "  );
		printxy(logo.X+50, logo.Y+4, "  +#+        "  );
		printxy(logo.X+50, logo.Y+5, " #+#         "  );
		printxy(logo.X+50, logo.Y+6, "###          "  );

		printxy(logo.X+22, logo.Y+9,  "<@ 5-Tic-Tac-Toe @>", WHITE);
		printxy(logo.X+60, logo.Y+10, "v1.0"               , GRAY );

		printxy(text.X   , text.Y  ,  "Press Enter to start"      );


	}

	setcolor(GREEN);
	if(shiftPointer == yes)
	{
		printxy(text.X-4 , text.Y ,  " >>");
		printxy(text.X+21, text.Y ,  "<< ");
	}
	else
	{
		printxy(text.X-4 , text.Y ,  ">> ");
		printxy(text.X+21, text.Y ,  " <<");
	}

	setcolor(NORMAL);


}

void Display::HelpScreen  ()                                                    {

	ClearScreen();


	const COORD text = { 4, 8};


	printxy(27, 5, "-<@ Quick Introduction @>-", GREEN);


	printxy(text.X   , text.Y  ,    " The game is played by two people, using the same computer.");

	printxy(text.X   , text.Y+2,    " You play in a 5x5 grid, player 1 using"                    );
	printxy(text.X+40, text.Y+2,    "X", RED                                                     );
	printxy(text.X+41, text.Y+2,    ", player 2 using"                                           );
	printxy(text.X+58, text.Y+2,    "O", CYAN                                                    );
	printxy(text.X+59, text.Y+2,    ". To win, the"                                              );

	printxy(text.X   , text.Y+3,    "players must line up"                                       );
	printxy(text.X+21, text.Y+3,    "FOUR", WHITE                                                );
	printxy(text.X+26, text.Y+3,    "of their symbols in any direction, while"                   );

	printxy(text.X   , text.Y+4,    "blocking the opponent's attempts to do the same."           );
	printxy(text.X   , text.Y+6,    " In case of a tie (full board but no lines of four), the winner will");

	printxy(text.X   , text.Y+7,    "instead be the player that made the most amount of"         );
	printxy(text.X+51, text.Y+7,    "lines of three symbols", WHITE                              );

	printxy(text.X   , text.Y+8,    "during the game."                                           );

	printxy(text.X   , text.Y+11,   "@ Press any key to start the game. Good luck!"              );


}

void Display::WinScreen   (player winner)                                       {

	printxy(27, 4,	"                         " );


	if		(winner == PLAYER_1){
		setcolor(RED  );
		printxy(29,19,  "<@+ Player 1 won! +@>" );
		printxy(29, 5,  "<@+ Player 1 won! +@>" );

	}
	else if	(winner == PLAYER_2){
		setcolor(CYAN );
		printxy(29,19,  "~@{ Player 2 won! }@~" );
		printxy(29, 5,  "~@{ Player 2 won! }@~" );

	}
	else if	(winner ==		TIE){
		setcolor(GREEN);
		printxy(30,19,   "-=] It's a tie! [=-"  );
		printxy(30, 5,   "-=] It's a tie! [=-"  );

	}


	//Corners

	printCh( 3, 2,  TOPLEFT_CORNER     );
	printCh( 3,22,  BOTTOMLEFT_CORNER  );

	for(int s=1; s<=8; s++) printCh( 3+s,  2,  HORIZONTAL_LINE );
	for(int s=1; s<=8; s++) printCh( 3+s, 22,  HORIZONTAL_LINE );
	for(int s=1; s<=8; s++) printCh(67+s,  2,  HORIZONTAL_LINE );
	for(int s=1; s<=8; s++) printCh(67+s, 22,  HORIZONTAL_LINE );

	printCh(76, 2,  TOPRIGHT_CORNER    );
	printCh(76,22,  BOTTOMRIGHT_CORNER );

	for(int s=1; s<=5; s++) printCh( 3,  2+s,  VERTICAL_LINE   );
	for(int s=1; s<=5; s++) printCh( 3, 16+s,  VERTICAL_LINE   );
	for(int s=1; s<=5; s++) printCh(76,  2+s,  VERTICAL_LINE   );
	for(int s=1; s<=5; s++) printCh(76, 16+s,  VERTICAL_LINE   );


	setcolor(NORMAL);



	//Highlight
	FilledCells();






	//The position of the windows
	const COORD nt = { 5,12};
	const COORD mw = {55,10};



	for(int x = 0; x < 26; x++)
		for(int y = 0; y < 12; y++)
			printCh(50+x, 10+y,  ' ');



	setcolor(WHITE);
	printCh(nt.X   , nt.Y  ,    TOPLEFT_CORNER    );

	for(int ps = 1; ps <= 12; ps++)
		printCh(nt.X+ps, nt.Y,  HORIZONTAL_LINE );

	printCh(nt.X+13, nt.Y  ,    TOPRIGHT_CORNER   );
	printxy(nt.X+ 3, nt.Y  ,    "<Threes>"        );

	printCh(nt.X   , nt.Y+1,    VERTICAL_LINE     );
	printCh(nt.X   , nt.Y+2,    NOLEFT_JOINT      );
	printCh(nt.X   , nt.Y+3,    VERTICAL_LINE     );
	printCh(nt.X   , nt.Y+4,    NOLEFT_JOINT      );
	printCh(nt.X   , nt.Y+5,    VERTICAL_LINE     );
	printCh(nt.X   , nt.Y+6,    BOTTOMLEFT_CORNER );

	setcolor(RED   );
	printNum(nt.X+4,  nt.Y+2,   Board.nOfThrees(PLAYER_1) );
	setcolor(WHITE );
	printCh	(nt.X+7,  nt.Y+3,   '/'                       );
	setcolor(CYAN  );
	printNum(nt.X+10, nt.Y+4,   Board.nOfThrees(PLAYER_2) );



	//Melody
	Beep(500, 100);
	Beep(300, 100);
	Beep(200, 100);
	Beep(300, 100);
	Beep(400, 100);
	Beep(300, 300);
	Sleep(1000);

	setcolor(WHITE);

	printCh(mw.X   , mw.Y  ,    TOPLEFT_CORNER     );

	for(int ps = 1; ps <= 18; ps++)
	printCh(mw.X+ps, mw.Y  ,    HORIZONTAL_LINE    );

	printCh(mw.X+19, mw.Y  ,    TOPRIGHT_CORNER    );
	printCh(mw.X+19, mw.Y+1,    VERTICAL_LINE      );
	printCh(mw.X+19, mw.Y+2,    VERTICAL_LINE      );
	printCh(mw.X+19, mw.Y+3,    BOTTOMRIGHT_CORNER );


	printxy(mw.X+2,mw.Y+1,  "     Enter =    " );

	setcolor(NORMAL);
	printxy(mw.X+2,mw.Y+2,  "  Restart game  " );


}



void Display::Turn        (player Player)                                       {

	if(Player == 1)
	{
		setcolor(RED );
		printxy(27, 4,  "+>> Player One's turn <<+"  );

	}
	else if(Player == 2)
	{
		setcolor(CYAN);
		printxy(27, 4,  "/(( Player Two's turn ))\\" );

	}

	printCh( 3, 2,  TOPLEFT_CORNER     );
	printCh( 3,22,  BOTTOMLEFT_CORNER  );
	printCh(76, 2,  TOPRIGHT_CORNER    );
	printCh(76,22,  BOTTOMRIGHT_CORNER );

	setcolor(NORMAL);

}

void Display::Score       (int score[4], bool DrawBox)                          {

	const COORD sc  = {      5,      4}; //score
	const COORD scv = {sc.X+12, sc.Y+2}; //score values

	if(DrawBox == yes)
	{
		//Drawing the score box

		setcolor(WHITE);
		printCh(sc.X   , sc.Y  ,    TOPLEFT_CORNER    );

		for(int ps = 1; ps <= 13; ps++)
			printCh(sc.X+ps, sc.Y,  HORIZONTAL_LINE );

		printCh(sc.X+14, sc.Y  ,    TOPRIGHT_CORNER   );
		printxy(sc.X+ 4, sc.Y  ,    "<Score>"         );

		printCh(sc.X   , sc.Y+1,    VERTICAL_LINE     );
		printCh(sc.X   , sc.Y+2,    NOLEFT_JOINT      );
		printCh(sc.X   , sc.Y+3,    NOLEFT_JOINT      );
		printCh(sc.X   , sc.Y+4,    NOLEFT_JOINT      );
		printCh(sc.X   , sc.Y+5,    VERTICAL_LINE     );
		printCh(sc.X   , sc.Y+6,    BOTTOMLEFT_CORNER );


		printxy (sc.X+2, sc.Y+2,    "Player 1:   ",	RED   );
		printxy (sc.X+2, sc.Y+3,    "Player 2:   ",	CYAN  );
		printxy (sc.X+2, sc.Y+4,    "Ties:       ",	GREEN );


	}

	printNum(scv.X, scv.Y+0,    score[PLAYER_1] );
	printNum(scv.X, scv.Y+1,    score[PLAYER_2] );
	printNum(scv.X, scv.Y+2,    score[TIE]      );


}

void Display::Controls    (bool DrawBigWindow, bool FirstLaunch)                {

	const COORD cs = {56, 18}; //controls small
	const COORD cb = {50, 10}; //controls big


	if(DrawBigWindow == no)
	{

		for(int x = 0; x < 26; x++)
			for(int y = 0; y < 12; y++)
				printCh(cb.X+x, cb.Y+y,  ' ');


		setcolor(NORMAL);

		printCh(cs.X   , cs.Y  ,    TOPLEFT_CORNER     );

		for(int ps = 1; ps <= 18; ps++)
			printCh(cs.X+ps, cs.Y,  HORIZONTAL_LINE );

		printCh(cs.X+19, cs.Y  ,    TOPRIGHT_CORNER    );
		printCh(cs.X+19, cs.Y+1,    VERTICAL_LINE      );
		printCh(cs.X+19, cs.Y+2,    VERTICAL_LINE      );
		printCh(cs.X+19, cs.Y+3,    BOTTOMRIGHT_CORNER );

		setcolor(GRAY);

		printxy(cs.X+ 2, cs.Y+1,    "Press Backspace"  );
		printxy(cs.X+ 2, cs.Y+2,    "to open controls" );


		setcolor(NORMAL);


	}

	else
	{
		for(int x = 0; x < 20; x++)
			for(int y = 0; y < 4; y++)
				printCh(cs.X+x, cs.Y+y,	 ' ');


		setcolor(WHITE);

		printCh(cb.X   , cb.Y  ,    TOPLEFT_CORNER     );

		for(int ps = 1; ps <= 24; ps++)
			printCh(cb.X+ps, cb.Y,  HORIZONTAL_LINE );

		printCh(cb.X+25, cb.Y  ,    TOPRIGHT_CORNER    );

		for(int s = 1; s <= 10; s++)
			printCh(cb.X+25, cb.Y+s, VERTICAL_LINE  );

		printCh(cs.X+19, cs.Y+3,    BOTTOMRIGHT_CORNER );

		printxy(cb.X+8 , cb.Y  ,    "<Controls>"       );

		setcolor(GREEN);

		printxy(cb.X+2 , cb.Y+2,    "< ^ v >"          );
		printxy(cb.X+2 , cb.Y+3,    "Enter"            );
		printxy(cb.X+2 , cb.Y+5,    "H"                );
		printxy(cb.X+2 , cb.Y+6,    "F5"               );
		printxy(cb.X+2 , cb.Y+7,    "F10"              );
		printxy(cb.X+2 , cb.Y+9,    "Backspace"        );

		setcolor(NORMAL);

		printxy(cb.X+11, cb.Y+2,    "Move pointer"     );
		printxy(cb.X+11, cb.Y+3,    "Place Symbol"     );
		printxy(cb.X+11, cb.Y+5,    "Instructions"     );
		printxy(cb.X+11, cb.Y+6,    "Reset Match "     );
		printxy(cb.X+11, cb.Y+7,    "Restart all "     );
		printxy(cb.X+11, cb.Y+9,    " Close this "     );
		printxy(cb.X+11, cb.Y+10,   " controls tab"    );



		if(FirstLaunch)
		{
			//I wanted to bring the attention of the user to the controls

			Sleep(200);

			for(int i = 0; i < 4; i++)
			{
				printxy(cb.X+11, cb.Y-3,  "V V V", WHITE);
				printxy(cb.X+11, cb.Y-2,  "     ");
				Beep(400, 150);
				Sleep(100);
				printxy(cb.X+11, cb.Y-3,  "     ");
				printxy(cb.X+11, cb.Y-2,  "V V V", WHITE);
				Beep(300, 150);
				Sleep(100);
			}

			Sleep(200);
			printxy(cb.X+11, cb.Y-2,  "     ");


			perform.CleanKeyboardBuffer();

		}


	}


}

void Display::Cell        (player Player)                                       {

	char Symbol;

	if      (Player == 1)
	{
		setcolor(RED );
		Symbol = 'X';
	}
	else if (Player == 2)
	{
		setcolor(CYAN);
		Symbol = 'O';
	}
	else Symbol = '?';


	printCh( 27 + (Board.p.X * 4),  6 + (Board.p.Y * 2),    Symbol );

	setcolor(NORMAL);


}

void Display::FilledCells ()                                                    {

	for(int y = 1; y <=5; y++){
		for(int x = 1; x <=5; x++){

			if      (Board.c(x,y) == empty)
				printxy(26+4*x, 6+2*y,    "   ");

			else if (Board.c(x,y) == PLAYER_1){

				if(Board.cHL(x,y) == PLAYER_1) Background = DARK_RED;
				printxy(26+4*x, 6+2*y,    " X ",  RED);

			}
			else if (Board.c(x,y) == PLAYER_2){

				if(Board.cHL(x,y) == PLAYER_2) Background = DARK_CYAN;
				printxy(26+4*x, 6+2*y,    " O ", CYAN);

			}
			setcolor(NORMAL);

		}
	}

}

void Display::Pointer     (int newpx, int newpy, player Player)                 {


	printCh( 26 + (Board.p.X * 4),	6 + (Board.p.Y * 2),      ' ' );
	printCh( 28 + (Board.p.X * 4),	6 + (Board.p.Y * 2),      ' ' );

	if(Board.selectedCell() == empty)
	printCh( 27 + (Board.p.X * 4),	6 + (Board.p.Y * 2),      ' ' );




	if(Player == PLAYER_2){
		printxy( 26 + (newpx * 4),	6 + (newpy * 2),      "(",  WHITE);
		printxy( 28 + (newpx * 4),	6 + (newpy * 2),      ")",  WHITE);
	}
	else{
		printxy( 26 + (newpx * 4),	6 + (newpy * 2),      ">",  WHITE);
		printxy( 28 + (newpx * 4),	6 + (newpy * 2),      "<",  WHITE);
	}



	if(Board.c(newpx, newpy) == empty){
		if(Player == 1) printxy( 27 + (newpx * 4),	6 + (newpy * 2),      "_",  RED );
		if(Player == 2) printxy( 27 + (newpx * 4),	6 + (newpy * 2),      "_",  CYAN);
	}



}



void Display::ClearScreen ()                                                    {

	for(int x = 2; x < 78; x++)
		for(int y = 2; y < 23; y++)
			printCh(x,y,	' ');

	//Redraw corner ornaments
	printCh( 3, 2,  TOPLEFT_CORNER		);
	printCh( 3,22,  BOTTOMLEFT_CORNER	);
	printCh(76, 2,  TOPRIGHT_CORNER		);
	printCh(76,22,  BOTTOMRIGHT_CORNER	);

}






void Grid::fillCell       (player Player)                                       {

	nCellsFilled++;

	cell[p.X][p.Y] = Player;

	Display.Cell(Player);


}

void Grid::doHighlight    (player Player, int x1, int y1, int x2, int y2,
                                          int x3, int y3, int x4, int y4)       {

	if(Player == 1 || Player == 2)
	{
		highlight[x1][y1] = Player;
		highlight[x2][y2] = Player;
		highlight[x3][y3] = Player;

		if(x4!=0 && y4!=0)
		highlight[x4][y4] = Player;

	}

}

int  Grid::c              (int x, int y)                                        {

	return cell[x][y];

}

int  Grid::cHL            (int x, int y)                                        {

	return highlight[x][y];

}

int  Grid::selectedCell   ()                                                    {

	return cell[p.X][p.Y];

}

void Grid::Reset          ()                                                    {

	for(int x = 1; x <= 5; x++){
		for(int y = 1; y <= 5; y++){

			cell      [x][y] = empty;
			highlight [x][y] = none;

		}
	}

	nCellsFilled = 0;


}







bool Grid::findsWinner    (player Player)                                       {

	int consecutiveSymbols = 0;




	// Horizontals
	for(int y=1; y<=5; y++)
	{

		for(int x=1; x<=5; x++)
		{
			if ( cell[x][y] == Player ) consecutiveSymbols++;
			else                        consecutiveSymbols = 0;

			if( consecutiveSymbols >= 4 )
			{
				doHighlight(Player,    x,y,  x-1,y,  x-2,y,  x-3,y);
				return yes;
			}
		}
		consecutiveSymbols = 0;
	}




	// Verticals
	for(int x=1; x<=5; x++)
	{

		for(int y=1; y<=5; y++)
		{
			if ( cell[x][y] == Player ) consecutiveSymbols++;
			else                        consecutiveSymbols = 0;

			if( consecutiveSymbols >= 4 )
			{
				doHighlight(Player,    x,y,  x,y-1,  x,y-2,  x,y-3);
				return yes;
			}

		}
		consecutiveSymbols = 0;
	}




	// Up-to-right diagonals
	for(int a=0; a<=4; a++) //center line
	{
		if ( cell[1+a][1+a] == Player ) consecutiveSymbols++;
		else                            consecutiveSymbols = 0;

		if( consecutiveSymbols >= 4 )
		{
			doHighlight(Player,    a+1,a+1,   a+0,a+0,   a-1,a-1,   a-2,a-2);
			return yes;
		}

	}
	consecutiveSymbols = 0;



	if( cell[1][2]==Player && cell[2][3]==Player && cell[3][4]==Player && cell[4][5]==Player ){
		doHighlight(Player,    1,2,  2,3,  3,4,  4,5);
		return yes;
	}
	if( cell[2][1]==Player && cell[3][2]==Player && cell[4][3]==Player && cell[5][4]==Player ){
		doHighlight(Player,    2,1,  3,2,  4,3,  5,4);
		return yes;
	}




	// Up-to-left diagonals
	for(int b=0; b<=4; b++)
	{
		if ( cell[5-b][1+b] == Player ) consecutiveSymbols++;
		else                            consecutiveSymbols = 0;

		if( consecutiveSymbols >= 4 )
		{
			doHighlight(Player,    5-b, b+1,   6-b, b+0,   7-b, b-1,   8-b, b-2);
			return yes;
		}

	}
	consecutiveSymbols = 0;



	if( cell[4][1]==Player && cell[3][2]==Player && cell[2][3]==Player && cell[1][4]==Player ){
		doHighlight(Player,    4,1,  3,2,  2,3,  1,4);
		return yes;
	}
	if( cell[5][2]==Player && cell[4][3]==Player && cell[3][4]==Player && cell[2][5]==Player ){
		doHighlight(Player,    5,2,  4,3,  3,4,  2,5);
		return yes;
	}


	//If all else failed
	return no;
}



int Grid::nOfThrees       (player Player)                                       {

	int consecutiveSymbols = 0;
	int Count              = 0;

// Horizontals
	for(int y=1; y<=5; y++)
	{
		for(int x=1; x<=5; x++)
		{
			if (cell[x][y] == Player) consecutiveSymbols++;
			else                      consecutiveSymbols = 0;

			if(consecutiveSymbols == 3)
			{
				Count++;

				doHighlight(Player,	x,y,  x-1,y,  x-2,y);

				x = SKIP_TO_NEXT_LINE;
			}
		}
		consecutiveSymbols = 0;
	}


// Verticals
	for(int x=1; x<=5; x++)
	{

		for(int y=1; y<=5; y++)
		{
			if (cell[x][y] == Player) consecutiveSymbols++;
			else                      consecutiveSymbols = 0;

			if(consecutiveSymbols == 3)
			{
				Count++;

				doHighlight(Player,	x,y,  x,y-1,  x,y-2);

				y = SKIP_TO_NEXT_LINE;
			}
		}
		consecutiveSymbols = 0;
	}



// Up-to-right diagonals
//(In lack of a way to run for-loops to check it automatically that I could come up with, I just run through every possible configuration)

    if      ( cell[1][1] == Player && cell[2][2] == Player && cell[3][3] == Player ){
		doHighlight(Player,	1,1,  2,2,  3,3);
		Count++;
	}else if( cell[2][2] == Player && cell[3][3] == Player && cell[4][4] == Player ){
		doHighlight(Player,	2,2,  3,3,  4,4);
		Count++;
	}else if( cell[3][3] == Player && cell[4][4] == Player && cell[5][5] == Player ){
		doHighlight(Player,	3,3,  4,4,  5,5);
		Count++;
	}


	if      ( cell[2][1] == Player && cell[3][2] == Player && cell[4][3] == Player ){
		doHighlight(Player,	2,1,  3,2,  4,3);
		Count++;
	}else if( cell[3][2] == Player && cell[4][3] == Player && cell[5][4] == Player ){
		doHighlight(Player,	3,2,  4,3,  5,4);
		Count++;
	}

	if      ( cell[1][2] == Player && cell[2][3] == Player && cell[3][4] == Player ){
		doHighlight(Player,	1,2,  2,3,  3,4);
		Count++;
	}else if( cell[2][3] == Player && cell[3][4] == Player && cell[4][5] == Player ){
		doHighlight(Player,	2,3,  3,4,  4,5);
		Count++;
	}


	if( cell[3][1] == Player && cell[4][2] == Player && cell[5][3] == Player )      {
		doHighlight(Player,	3,1,  4,2,  5,3);
		Count++;
	}

	if( cell[1][3] == Player && cell[2][4] == Player && cell[3][5] == Player )      {
		doHighlight(Player,	1,3,  2,4,  3,5);
		Count++;
	}




// Up-to-left diagonals
//(In lack of a way to run for-loops to check it automatically that I could come up with, I just run through every possible configuration)

	if      ( cell[5][1] == Player && cell[4][2] == Player && cell[3][3] == Player ){
		doHighlight(Player,	5,1,  4,2,  3,3);
		Count++;
	}else if( cell[4][2] == Player && cell[3][3] == Player && cell[2][4] == Player ){
		doHighlight(Player,	4,2,  3,3,  2,4);
		Count++;
	}else if( cell[3][3] == Player && cell[2][4] == Player && cell[1][5] == Player ){
		doHighlight(Player,	3,3,  2,4,  1,5);
		Count++;
	}


	if      ( cell[5][2] == Player && cell[4][3] == Player && cell[3][4] == Player ){
		doHighlight(Player,	5,2,  4,3,  3,4);
		Count++;
	}else if( cell[4][3] == Player && cell[3][4] == Player && cell[2][5] == Player ){
		doHighlight(Player,	4,3,  3,4,  2,5);
		Count++;
	}

	if      ( cell[4][1] == Player && cell[3][2] == Player && cell[2][3] == Player ){
		doHighlight(Player,	4,1,  3,2,  2,3);
		Count++;
	}else if( cell[3][2] == Player && cell[2][3] == Player && cell[1][4] == Player ){
		doHighlight(Player,	3,2,  2,3,  1,4);
		Count++;
	}


	if( cell[5][3] == Player && cell[4][4] == Player && cell[3][5] == Player )     {
		doHighlight(Player,	5,3,  4,4,  3,5);
		Count++;
	}

	if( cell[3][1] == Player && cell[2][2] == Player && cell[1][3] == Player )     {
		doHighlight(Player,	3,1,  2,2,  1,3);
		Count++;
	}




	return Count;

}








