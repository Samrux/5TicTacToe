/*******************************************\
 *                                         *
 *       5x5 Tic-Tac-Toe -- 5TTT.cpp       *
 *         Made by Samrux (2015)           *
 *                                         *
\*******************************************/


#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>

#define Key_Up 72
#define Key_Down 80
#define Key_Right 77
#define Key_Left 75
#define Key_Enter 13
#define Key_Backspace 8
#define Key_F3 61
#define Key_F5 63
#define Key_F10 68

using namespace std;



//Game values
int  player;
int  cellsFilled;
int  cell[6][6], highlight[6][6];
int  selected_X, selected_Y;
int  score[3] = {0,0,0};

//Program values
int Background = 0;
bool Debug = 0;

//Game functions
void Display(bool Win);
void Help();
bool IsWinningMove();
int ThreesCount(int player);

//Utility functions
void Color(string color);
void ClearScreen();




int main()
{
	SetConsoleTitle("5-Tic-Tac-Toe");

	Help();

	while(true) //Run program forever.
	{




	//Set up values for the match about to start.
	cellsFilled = 0;
	selected_X = 1; selected_Y = 1;

	if (player == 1)  player = 2; //will switch the player to the one who lost on the last round
	else			  player = 1;

	for(int y=1; y<=5; y++){ //Reset board
		for(int x=1; x<=5; x++){
			cell     [x][y] = 0;
			highlight[x][y] = 0;
		}
	}



	while(true) //The current match. Every button press will loop through this code until someone wins.
	{

		Display(0); //Displays the board


		bool placedSymbol = false;

        switch(int input = getch()) //Scan the keyboard
        {

        	case Key_Enter:
        		if (cell[selected_X][selected_Y] == 0) //If the selected cell is empty
				{
					cell[selected_X][selected_Y] = player; //Place an X or O
					cellsFilled++;
					placedSymbol = true;
				}
        		break;


			case Key_Up:
				if(selected_Y > 1) selected_Y--;
				break;

			case Key_Down:
				if(selected_Y < 5) selected_Y++;
				break;

			case Key_Left:
				if(selected_X > 1) selected_X--;
				break;

			case Key_Right:
				if(selected_X < 5) selected_X++;
				break;

			case 'h':
				Help();
				break;



			case Key_F3:
				cellsFilled = 25;
				break;


			case Key_F5:
				for(int x=1; x<=5; x++){ //resets board
					for(int y=1; y<=5; y++){
						cell[x][y] = 0; }}
				cellsFilled = 0; //resets values
				player = 1;

				break;



			case Key_F10:

				score[0] = 0; //resets score
				score[1] = 0;
				score[2] = 0;

				for(int y=1; y<=5; y++){ //resets board
					for(int x=1; x<=5; x++){
						cell[x][y] = 0; }}

				cellsFilled = 0; //resets values
				player = 1;
				selected_X = 1; selected_Y = 1;

				ClearScreen();
				cout << "\n > Program and score reset.";
				getch();
				break;


/*			case Key_Backspace:
				if(!Debug) Debug = true;
				else       Debug = false;
				break;
*/
        }


		if(placedSymbol)
		{
			if(IsWinningMove())
			{
				cellsFilled--; //this is signaling to the winning screen that it is not a tie (by not being able to equal 25)
				break; //Someone won, so it stops the game
			}
			else
			{ //if not a win, make it the other player's turn
				if     (player == 1) player = 2;
				else if(player == 2) player = 1;
			}

		}


		if(cellsFilled == 25) break; //board full means it's a tie and stops the game




	} //end current match, open win screen

	selected_X = 0; selected_Y = 0; //no selected cell


	if(cellsFilled == 25) //board full
	{
		if     (ThreesCount(1) > ThreesCount(2))
		{
			player = 1; //signals the winner
			cellsFilled--; //signals it's not a tie anymore
		}
		else if(ThreesCount(1) < ThreesCount(2))
		{
			player = 2;
			cellsFilled--;
		}

		Display(1);

		cout << " Lines of three:  ";
		Color("Red");   cout << ThreesCount(1);
		Color("Reset"); cout << " / ";
		Color("Cyan");  cout << ThreesCount(2);

	}else Display(1);

	cout << "\n\n";


	//The turns will be inverted at the start of a new round, making it so the loser will play first.
	//Or, in case of a tie, the player who played second on the last round (because the number of cells is uneven).
	if     (player == 1 && cellsFilled < 25){
		Color("Red");  cout << "\n\t   Player One wins!"; Color("Reset");
		score[1]++;
	}
	else if(player == 2 && cellsFilled < 25){
		Color("Cyan"); cout << "\n\t   Player Two wins!"; Color("Reset");
		score[2]++;
	}
	else
	{
		Color("Green");  cout << "\n\t      It's a tie!"; Color("Reset");
		score[0]++;
	}

	//Score
	Color("Lwhite"); cout << "\n\n @ "; Color("Reset");
	cout << "Player One: "; Color("Red"); cout << score[1];
	Color("Lwhite"); cout << "\n @ "; Color("Reset");
	cout << "Player Two: "; Color("Cyan"); cout << score[2];
	Color("Lwhite"); cout << "\n @ "; Color("Reset");
	cout << "Ties: "; Color("Green"); cout << score[0];
	Color("Reset");
	cout << endl;



	Color("Gray"); cout << "\n\n\t Press Enter to reset."; Color("Reset");

	while(getch() != Key_Enter);

	} //End loop

} //end main





void Help()
{
	ClearScreen();

	Color("Green"); cout << "\n\n\t\t\t<@ 5-Tic-Tac-Toe @> v1\n\n\n"; Color("Reset");
	cout << "  The game is played by two people, using the same computer.\n\n";
	cout << "  You play in a 5x5 grid, player 1 using "; Color("Red"); cout << "X"; Color("Reset"); cout << ", and player 2 using "; Color("Cyan"); cout << "O"; Color("Reset"); cout << ". To win,\n";
	cout << " the players must line up "; Color("Lwhite"); cout << "four"; Color("Reset"); cout << " of their symbols in any direction, while\n";
	cout << " blocking the opponent's attempts to do the same.\n\n";
	cout << "  In case of a draw (full board but no lines of four), the winner will\n";
	cout << " instead be the player that made the most amount of "; Color("Lwhite"); cout << "lines of three symbols\n"; Color("Reset");
	cout << " during the game.\n\n\n\n";
	cout << " Use the arrow keys to select a cell, and Enter to place a symbol.\n";
	cout << " F5 will reset the current match. F10 will reset all (including score).\n";
	cout << " F3 will force the match to end and look for a winner immediately.\n\n";
	Color("Lwhite"); cout << "@> Press any key to start the game.\n"; Color("Reset");

	getch();
}





void Display(bool Win)
{
	ClearScreen();


	if(!Win)
	{
		cout << "\n      ";
		if     (player == 1){ Color("Red");  cout << "+>> Player One's turn <<+";  Color("Reset");}
		else if(player == 2){ Color("Cyan"); cout << "/(( Player Two's turn ))\\"; Color("Reset");}
	}

	cout << "\n\n\n";




	for(int y=1; y<=5; y++) //The entire board, cell by cell
	{
		cout << "\t ";

		for(int x=1; x<=5; x++)
		{



			if(highlight[x][y] > 0)
			{

				if( (highlight[x][y] == 1) ||
					(highlight[x][y] == 3 && player == 1))
				{
					Background = 4; //highlights the cell
						Color("Reset"); //applies background
				}
				if( (highlight[x][y] == 2) ||
					(highlight[x][y] == 3 && player == 2))
				{
					Background = 3;
						Color("Reset");
				}

			}




			if (x == selected_X && y == selected_Y) //if the cell is selected
			{
				Color("Lwhite");
				if      (player == 1) cout << ">";
				else if (player == 2) cout << "(";
				Color("Reset");

			}else cout << " ";






			if (cell[x][y] == 0) //If the cell is empty
			{
				if(x == selected_X && y == selected_Y) //If it is selected
				{
					if      (player == 1) Color("Red");
					else if (player == 2) Color("Cyan");

					cout << "_"; Color("Reset");
				}else cout << " "; //If not selected
			}
			else if(cell[x][y] == 1){
				Color("Red"); cout << "X";
				Color("Reset");
			}
			else if (cell[x][y] == 2){
				Color("Cyan"); cout << "O";
				Color("Reset");
			}
			else cout << "?"; //error


			if (x == selected_X && y == selected_Y)
			{
				Color("Lwhite");
				if      (player == 1) cout << "<";
				else if (player == 2) cout << ")";
				Color("Reset");

			}else cout << " ";



			if(highlight[x][y] > 0 || (x == selected_X && y == selected_Y)) //ends the highlight from before
			{
				Background = 0;
				Color("Reset");
			}



			//Only shows a line if it is not the very edge
			if (x < 5){ Color("Gray"); cout << "|"; Color("Reset");}

		}
		if (y < 5){
				Color("Gray");
				cout << "\n\t ";

				for(int n = 1; n <=19; n++)
				{
					if(n%4 == 0 && n != 19)
						cout << "+";
					else cout << "-";
				}
				cout << "\n";
				Color("Reset");
		}


	}
	cout << "\n\n";
	if(!Win) cout << endl;


	if (Debug) cout << " *** Debug mode active. Press the Backspace key again to turn off.\n\n";
	else if(!Win)
	{
		cout << " =>";
		Color("Gray"); cout << " Press H to open the help screen.\n"; Color("Reset");

		if(score[0] > 0 || score[1] > 0 || score[2] > 0) //Displays the scores
		{
			Color("Lwhite"); cout << "\n\n @ "; Color("Reset");
			cout << "Player One: "; Color("Red"); cout << score[1];
			Color("Lwhite"); cout << "\n @ "; Color("Reset");
			cout << "Player Two: "; Color("Cyan"); cout << score[2];
			Color("Lwhite"); cout << "\n @ "; Color("Reset");
			cout << "Ties: "; Color("Green"); cout << score[0];
			Color("Reset");
			cout << endl;
		}
	}



}





bool IsWinningMove()
{
	int consecutiveSymbols = 0;

// Horizontals
	for(int y=1; y<=5; y++)
	{

		for(int x=1; x<=5; x++)
		{
			if (cell[x][y] == player) consecutiveSymbols++;
			else					  consecutiveSymbols = 0;

			if(consecutiveSymbols >= 4) //if it is a win, highlight the winning cells
			{
				highlight[x]  [y] = 3;
				highlight[x-1][y] = 3;
				highlight[x-2][y] = 3;
				highlight[x-3][y] = 3;

				return true;
			}
		}
		consecutiveSymbols = 0;
	}


// Verticals
	for(int x=1; x<=5; x++)
	{

		for(int y=1; y<=5; y++)
		{
			if (cell[x][y] == player) consecutiveSymbols++;
			else 	 				  consecutiveSymbols = 0;

			if(consecutiveSymbols >= 4)
			{
				highlight[x][y]   = 3;
				highlight[x][y-1] = 3;
				highlight[x][y-2] = 3;
				highlight[x][y-3] = 3;

				return true;
			}

		}
		consecutiveSymbols = 0;
	}



// Up-to-right diagonals
	for(int a=0; a<=4; a++) //center line
	{
		if (cell[1+a][1+a] == player) consecutiveSymbols++;
		else 						  consecutiveSymbols = 0;

		if(consecutiveSymbols >= 4)
		{
			highlight[1+a][1+a] = 3;
			highlight[a]  [a]   = 3;
			highlight[a-1][a-1] = 3;
			highlight[a-2][a-2] = 3;

			return true;
		}
	} consecutiveSymbols = 0;

	if(cell[1][2]==player && cell[2][3]==player && cell[3][4]==player && cell[4][5]==player){
		highlight[1][2] = 3; highlight[2][3] = 3; highlight[3][4] = 3; highlight[4][5] = 3;
		return true;
	}
	if(cell[2][1]==player && cell[3][2]==player && cell[4][3]==player && cell[5][4]==player){
		 highlight[2][1] = 3; highlight[3][2] = 3; highlight[4][3] = 3; highlight[5][4] = 3;
		return true;
	}



// Up-to-left diagonals
	for(int b=0; b<=4; b++)
	{
		if (cell[5-b][1+b] == player) consecutiveSymbols++;
		else 						  consecutiveSymbols = 0;

		if(consecutiveSymbols >= 4)
		{
			highlight[5-b][1+b] = 3;
			highlight[6-b][b]   = 3;
			highlight[7-b][b-1] = 3;
			highlight[8-b][b-2] = 3;

			return true;
		}
	} consecutiveSymbols = 0;

	if(cell[4][1]==player && cell[3][2]==player && cell[2][3]==player && cell[1][4]==player){
		highlight[4][1] = 3; highlight[3][2] = 3; highlight[2][3] = 3; highlight[1][4] = 3;
		return true;
	}
	if(cell[5][2]==player && cell[4][3]==player && cell[3][4]==player && cell[2][5]==player){
		 highlight[5][2] = 3; highlight[4][3] = 3; highlight[3][4] = 3; highlight[2][5] = 3;
		return true;
	}



	return false; //If no winning lines were found
}





int ThreesCount(int player)
{

	int consecutiveSymbols = 0;
	int Count = 0;

// Horizontals
	for(int y=1; y<=5; y++)
	{
		for(int x=1; x<=5; x++)
		{
			if (cell[x][y] == player) consecutiveSymbols++;
			else 					  consecutiveSymbols = 0;

			if(consecutiveSymbols == 3){ //line found
				Count++;

				highlight[x]  [y] = player;
				highlight[x-1][y] = player;
				highlight[x-2][y] = player;

				x = 6; //skips to the next row
			}
		}
		consecutiveSymbols = 0;
	}


// Verticals
	for(int x=1; x<=5; x++)
	{

		for(int y=1; y<=5; y++)
		{
			if (cell[x][y] == player) consecutiveSymbols++;
			else					  consecutiveSymbols = 0;

			if(consecutiveSymbols == 3)
			{
				Count++;

				highlight[x][y]   = player;
				highlight[x][y-1] = player;
				highlight[x][y-2] = player;

				y = 6;
			}
		}
		consecutiveSymbols = 0;
	}



// Up-to-right diagonals
//(In lack of a way to run for-loops to check it automatically that I could come up with, I just run through every possible configuration)

    if(cell[1][1] == player && cell[2][2] == player && cell[3][3] == player){
		highlight[1][1] = player; highlight[2][2] = player; highlight[3][3] = player;
		Count++;
	}else if(cell[2][2] == player && cell[3][3] == player && cell[4][4] == player){
		highlight[2][2] = player; highlight[3][3] = player; highlight[4][4] = player;
		Count++;
	}else if(cell[3][3] == player && cell[4][4] == player && cell[5][5] == player){
		highlight[3][3] = player; highlight[4][4] = player; highlight[5][5] = player;
		Count++;
	}


	if(cell[2][1] == player && cell[3][2] == player && cell[4][3] == player){
		highlight[2][1] = player; highlight[3][2] = player; highlight[4][3] = player;
		Count++;
	}else if(cell[3][2] == player && cell[4][3] == player && cell[5][4] == player){
		highlight[3][2] = player; highlight[4][3] = player; highlight[5][4] = player;
		Count++;
	}

	if(cell[1][2] == player && cell[2][3] == player && cell[3][4] == player){
		highlight[1][2] = player; highlight[2][3] = player; highlight[3][4] = player;
		Count++;
	}else if(cell[2][3] == player && cell[3][4] == player && cell[4][5] == player){
		highlight[2][3] = player; highlight[3][4] = player; highlight[4][5] = player;
		Count++;
	}


	if(    cell[3][1] == player && cell[4][2] == player && cell[5][3] == player){
		highlight[3][1] = player; highlight[4][2] = player; highlight[5][3] = player;
		Count++;
	}

	if(    cell[1][3] == player && cell[2][4] == player && cell[3][5] == player){
		highlight[1][3] = player; highlight[2][4] = player; highlight[3][5] = player;
		Count++;
	}




// Up-to-left diagonals
//(In lack of a way to run for-loops to check it automatically that I could come up with, I just run through every possible configuration)

	if(cell[5][1] == player && cell[4][2] == player && cell[3][3] == player){
		highlight[5][1] = player; highlight[4][2] = player; highlight[3][3] = player;
		Count++;
	}else if(cell[4][2] == player && cell[3][3] == player && cell[2][4] == player){
		highlight[4][2] = player; highlight[3][3] = player; highlight[2][4] = player;
		Count++;
	}else if(cell[3][3] == player && cell[2][4] == player && cell[1][5] == player){
		highlight[3][3] = player; highlight[2][4] = player; highlight[1][5] = player;
		Count++;
	}


	if( cell[5][2] == player && cell[4][3] == player && cell[3][4] == player ){
		highlight[5][2] = player; highlight[4][3] = player; highlight[3][4] = player;
		Count++;
	}else if( cell[4][3] == player && cell[3][4] == player && cell[2][5] == player ){
		highlight[4][3] = player; highlight[3][4] = player; highlight[2][5] = player;
		Count++;
	}

	if( cell[4][1] == player && cell[3][2] == player && cell[2][3] == player ){
		highlight[4][1] = player; highlight[3][2] = player; highlight[2][3] = player;
		Count++;
	}else if( cell[3][2] == player && cell[2][3] == player && cell[1][4] == player ){
		highlight[3][2] = player; highlight[2][3] = player; highlight[1][4] = player;
		Count++;
	}


	if( cell[5][3] == player && cell[4][4] == player && cell[3][5] == player ){
		highlight[5][3] = player; highlight[4][4] = player; highlight[3][5] = player;
		Count++;
	}

	if( cell[3][1] == player && cell[2][2] == player && cell[1][3] == player ){
		highlight[3][1] = player; highlight[2][2] = player; highlight[1][3] = player;
		Count++;
	}




	return Count;

}






//Utility

void Color(string color){
    int colorNum;

	/**/ if (color=="Lwhite") colorNum=15;
    else if (color=="Gray"  ) colorNum= 8;
    else if (color=="Red"   ) colorNum=12;
    else if (color=="Cyan"  ) colorNum=11;
    else if (color=="Green" ) colorNum= 2;
    else if (color=="Reset" ) colorNum= 7;

    //Sets text color and applies background
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (colorNum + (Background * 16)));

}


void ClearScreen() //This I don't understand, but I prefer it a thousand times more than system()
{
	if(!Debug)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coordScreen = { 0, 0 };
		DWORD cCharsWritten;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD dwConSize;

		if( !GetConsoleScreenBufferInfo(hConsole, &csbi) ) return;

		dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

		if( !FillConsoleOutputCharacter( hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten ) ) return;
		if( !GetConsoleScreenBufferInfo( hConsole, &csbi )) return;
		if( !FillConsoleOutputAttribute( hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten ) ) return;

		SetConsoleCursorPosition(hConsole, coordScreen);
	}
}





