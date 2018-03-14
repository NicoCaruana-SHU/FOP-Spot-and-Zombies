// TODO My tasks, code the holes and the cheats.!!!
// ---------------------------------------------------------------------------
// Program: Skeleton for Task 1c – group assignment
// Author: Pascale Vacher
// Last updated: 26 February 2018
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// ----- include libraries
// ---------------------------------------------------------------------------

// include standard libraries
#include <iostream>	
#include <iomanip> 
#include <conio.h> 
#include <cassert> 
#include <string>
#include <sstream>
#include <time.h> // For time display
#include <vector>

using namespace std;

// include our own libraries
#include "RandomUtils.h"    // for Seed, Random
#include "ConsoleUtils.h"	// for Clrscr, Gotoxy, etc.

// ---------------------------------------------------------------------------
// ----- define constants
// ---------------------------------------------------------------------------

// defining the size of the grid
const int  SIZEX(25);    		// horizontal dimension
const int  SIZEY(20);			// vertical dimension
// defining symbols used for display of the grid and content
const char SPOT('@');   		// spot
const char TUNNEL(' ');			// tunnel
const char WALL('#');    		// border
const char HOLE('0');			// Character used to represent holes
const char PILL('*');
// defining the command letters to move the spot on the maze
const int  UP(72);			// up arrow
const int  DOWN(80); 		// down arrow
const int  RIGHT(77);		// right arrow
const int  LEFT(75);		// left arrow

// defining the other command letters
const char QUIT('Q');			// to end the game
const char FREEZE('F');			// Cheat command 1, to freeze the zombies in place.
const char EXTERMINATE('X');	// Cheat command 2, eliminate all zombies
const char EAT('E');			// Cheat command 3, eat all remaining pills.

struct Item
{
	int x, y;
	char symbol;
};

//struct Spot : Item {
//	int lives;
//};

struct GameSpaceManager
{
	char grid[SIZEY][SIZEX];			// grid for display
	char maze[SIZEY][SIZEX];			// structure of the maze
};

struct GameObjectManager
{
	vector<Item> holes, pills;
	Item spot;
};

// TODO Add a difficulty struct with max number of items - Advanced tasks!


// ---------------------------------------------------------------------------
// ----- run game
// ---------------------------------------------------------------------------

int main()
{
	// function declarations (prototypes)
	void displayEntryScreen();
	void initialiseGame(GameSpaceManager& gsm, const int numberOfHoles, const int MAXPILLS, GameObjectManager& gom);
	void paintGame(const GameSpaceManager& gsm, const string& mess);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess);
	void updateGrid(GameSpaceManager& gsm, const GameObjectManager& gom);
	void endProgram();
	void getTime(struct tm &timeLocal);

	// local variable declarations

	GameSpaceManager gsm;
	GameObjectManager gom;

	string message("LET'S START...");	// current message to player

	// TODO difficulty level related variables - will need to change location when we add 
	const int numberOfHoles(12);
	const int MAXPILLS(8);

	// action...
	Seed();								// seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");
	displayEntryScreen();
	cin.get(); // HACK temporarily hold display here to view entry screen.
	Clrscr();							// Using included libraries, clears the game screen - sets it all grey.
	initialiseGame(gsm, numberOfHoles, MAXPILLS, gom);	// initialise grid (incl. walls and spot)
	paintGame(gsm, message);			// display game info, modified grid and messages
	int key;							// current key selected by player
	do
	{
		key = toupper(getKeyPress()); 	// read in  selected key: arrow or letter command
		if (isArrowKey(key))
		{
			updateGameData(gsm.grid, gom.spot, key, message);		// move spot in that direction
			updateGrid(gsm, gom);									// update grid information
		}
		else {
			switch (key) {
			case FREEZE:
				//TODO STUB - Freeze the zombies in place
				break;
			case EXTERMINATE:
				//TODO STUB - Exterminate all zombies on screen, pressing again respawns them back at corners
				break;
			case EAT:
				//TODO STUB - Eat all pills. Permanently disappear from the board and numremainingpills set to 0;
				break;
			case QUIT:
				break; //Maybe do something here.. put it in for now to simply get rid of the invalid key message before terminating loop on quit command.

			default:
				message = "INVALID KEY!";	// set 'Invalid key' message
			}
		}
		paintGame(gsm, message);		// display game info, modified grid and messages
	} while (!wantsToQuit(key));		// while user does not want to quit
	endProgram();						// display final message
	return 0;
}


// ---------------------------------------------------------------------------
// ----- initialise game state
// ---------------------------------------------------------------------------

void initialiseGame(GameSpaceManager& gsm, const int numberOfHoles, const int MAXPILLS, GameObjectManager& gom)
{
	// initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setItemInitialCoordinates(Item& item, const char itemSymbol, char[][SIZEX]);
	void updateGrid(GameSpaceManager& gsm, const GameObjectManager& gom);
	void setMultipleItems(const char itemSymbol, int maxNumOfItems, vector<Item>& itemStore, char maze[][SIZEX]);

	setInitialMazeStructure(gsm.maze);			// initialise maze
	setMultipleItems(HOLE, numberOfHoles, gom.holes, gsm.maze);
	setMultipleItems(PILL, MAXPILLS, gom.pills, gsm.maze);
	setItemInitialCoordinates(gom.spot, SPOT, gsm.maze);
	updateGrid(gsm, gom);						// prepare grid
}

void setItemInitialCoordinates(Item& item, const char itemSymbol, char maze[][SIZEX])
{												// NICO - Replaced the constant recalling itself with a do-while loop - more efficient!
	do {
		item.x = Random(SIZEX - 2);
		item.y = Random(SIZEY - 2);
	} while (maze[item.y][item.x] != TUNNEL);
	item.symbol = itemSymbol;
}

void setInitialMazeStructure(char maze[][SIZEX])
{
	/*
	// set the position of the walls in the maze
	// initialise maze configuration
	char initialMaze[SIZEY][SIZEX] = {	// local array to store the maze structure
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', '#', ' ', ' ', '#' },
		{ '#', ' ', ' ', '#', ' ', ' ', '#', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#' },
		{ '#', '#', '#', '#', '#', '#', '#', '#', '#', '#' } };
	// with '#' for wall, ' ' for tunnel, etc.
	// copy into maze structure with appropriate symbols
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			switch (initialMaze[row][col])
			{
				// not a direct copy, in case the symbols used change
				case '#': maze[row][col] = WALL; break;
				case ' ': maze[row][col] = TUNNEL; break;
			}
	*/
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			maze[row][col] = (col == 0 || col == SIZEX - 1 || row == 0 || row == SIZEY - 1) ? WALL : TUNNEL;
}

void setMultipleItems(const char itemSymbol, int maxNumOfItems, vector<Item>& itemStore, char maze[][SIZEX])
{
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i = maxNumOfItems; i > 0; --i) {
		Item i1;
		setItemInitialCoordinates(i1, itemSymbol, maze);
		itemStore.push_back(i1);
		placeItem(maze, i1);
	}
}

// ---------------------------------------------------------------------------
// ----- update grid state
// ---------------------------------------------------------------------------

void updateGrid(GameSpaceManager& gsm, const GameObjectManager& gom)
{
	// update grid configuration after each move
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item& spot);
	void placeMultipleItems(char g[][SIZEX], const vector<Item>& itemStore);

	// Not sent as complete GSM/GOM, to keep constant array restrictions.
	setMaze(gsm.grid, gsm.maze);	// reset the empty maze configuration into grid 
	placeMultipleItems(gsm.grid, gom.holes);
	placeMultipleItems(gsm.grid, gom.pills);
	placeItem(gsm.grid, gom.spot);	// set spot in grid
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX])
{
	// reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item& item)
{
	// place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

// Item placement function
// IN: Array representing the maze, Vector holding the items.
// OUT:
// Precondition: None
// Postcondition: All items will be placed on grid

void placeMultipleItems(char g[][SIZEX], const vector<Item>& itemStore) {
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i(itemStore.size() - 1); i >= 0; --i) { // place items until max is reached
		// TODO Item renderer.. probably a good place for a visible check here.

		placeItem(g, itemStore[i]);
	}
}

// ---------------------------------------------------------------------------
// ----- move items on the grid
// ---------------------------------------------------------------------------
void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess)
{
	// move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	assert(isArrowKey(key));

	// reset message to blank
	mess = "                                         ";		// reset message to blank

	// calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

	// check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[spot.y + dy][spot.x + dx])
	{
		// ...depending on what's on the target position in grid...
	case TUNNEL:		// can move
		spot.y += dy;	// go in that Y direction
		spot.x += dx;	// go in that X direction
		break;
	case WALL:  		// hit a wall and stay there
		mess = "CANNOT GO THERE!";
		break;
	}
}
// ---------------------------------------------------------------------------
// ----- process key
// ---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy)
{
	// calculate direction indicated by key
	bool isArrowKey(const int k);
	assert(isArrowKey(key));
	switch (key)	// ...depending on the selected key...
	{
	case LEFT:  	// when LEFT arrow pressed...
		dx = -1;	// decrease the X coordinate
		dy = 0;
		break;
	case RIGHT: 	// when RIGHT arrow pressed...
		dx = +1;	// increase the X coordinate
		dy = 0;
		break;
	case UP:
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		dx = 0;
		dy = +1;
		break;
	}
}

int getKeyPress()
{
	// get key or command (in uppercase) selected by user
	// KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			// read in the selected arrow key or command letter
	while (keyPressed == 224) 		// ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;		// return it in uppercase 
}

bool isArrowKey(const int key)
{
	// check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}

bool wantsToQuit(const int key)
{
	// check if the user wants to quit (when key is 'Q' or 'q')
	return key == QUIT;
}

// ---------------------------------------------------------------------------
// ----- display info on screen
// ---------------------------------------------------------------------------

string tostring(int x)
{
	// convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}

string tostring(char x)
{
	// convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message)
{
	// display a string using specified colour at a given position 
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}

// TODO Nico
void showGameTitle(int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, x, y, "--------------------");
	showMessage(clDarkGrey, clYellow, x, y + 1, "| SPOT AND ZOMBIES |");
	showMessage(clDarkGrey, clYellow, x, y + 2, "--------------------");
}

// TODO Nico
void showGroupMembers(int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, x, y, "GROUP CS4G1A  -  2017-18");
	showMessage(clDarkGrey, clYellow, x, y + 1, "Charlie Batten  ");
	showMessage(clDarkGrey, clYellow, x, y + 2, "Matt Bellamy    ");
	showMessage(clDarkGrey, clYellow, x, y + 3, "Nico Caruana    27022205");
}

// TODO Nico
// Entry screen display
void displayEntryScreen() {
	void showGameTitle(int x, int y);
	void showGroupMembers(int x, int y);

	showGameTitle(10, 6);
	showGroupMembers(10, 10);

}

void paintGame(const GameSpaceManager& gsm, const string& mess)
{
	void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	void paintGrid(const GameSpaceManager& gsm);
	// display game title
	showMessage(clDarkGreen, clGreen, 0, 0, "___GAME___");

	displayTimeAndDate(clYellow, clBlue, 40, 0);

	showMessage(clYellow, clBlue, 40, 2, "FoP Task 1c: February 2018");

	// display menu options available
	showMessage(clRed, clGreen, 40, 8, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clRed, clGreen, 40, 9, "TO QUIT ENTER 'Q'           ");

	// print auxiliary messages if any
	showMessage(clBlack, clYellow, 40, 6, mess);	// display current message

	// display grid contents
	paintGrid(gsm);
}

void paintGrid(const GameSpaceManager& gsm)
{
	// display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
			cout << gsm.grid[row][col];	// output cell content
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(clRed, clYellow, 40, 8, "");
	system("pause");	// hold output screen until a keyboard key is hit
}

// IN: background colour, text colour, x-position, y-position
// OUT:
// Precondition:
// Postcondition: Date and time will be displayed on consecutive lines in the specified colours.
void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y) {
	void getTime(struct tm &timeLocal);

	assert(true); // TODO Assert needs sorting, although probably wont be needed, function is simple enough to run
	struct tm timeLocal; // create time structure called timeLocal
	getTime(timeLocal); //populate structure with values

	std::ostringstream date;
	date << setfill('0');
	date << "DATE: " << setw(2) << timeLocal.tm_mday << "/" << setw(2) << timeLocal.tm_mon << "/" << (timeLocal.tm_year + 1900); //timeLocal.tm_year is int, years since 1900

	std::ostringstream time;
	time << setfill('0');
	time << "TIME: " << setw(2) << timeLocal.tm_hour << ":" << setw(2) << timeLocal.tm_min << ":" << setw(2) << timeLocal.tm_sec;

	showMessage(firstColour, secondColour, x, y, date.str()); // show the date at the co-oridinates given.
	showMessage(firstColour, secondColour, x, y + 1, time.str()); // display  time on the following line directly underneath.
	return;
}

// IN: Empty struct to hold converted time data
// OUT:
// Precondition: None
// Postcondition: Supplied struct will be filled with converted time data.
void getTime(struct tm &timeLocal) { // Get raw time data from system and convert it to usable format.

	assert(true);
	time_t rawTime; // create time_t struct variable to store raw time data
	time(&rawTime); // get raw time data (in seconds from Jan 1 1970) and insert into rawtime variable.

	localtime_s(&timeLocal, &rawTime); //convert raw time into usable time structure and insert into struct timeLocal
}

// TODO Removed functions - delete at end.
/*
//UNDONE Array comparison function. Quite inefficient to use, if you can think of a better way, let us know!
//// IN: Copy of intended position variable to use as iterator, const reference of the item to verify, Array of items to check uniqueness against.
//// OUT: TRUE if Item is unique, FALSE if item already present.
//// Precondition:
//// Postcondition:
//bool isPositionUnique(int i, const Item& item, const vector<Item>& array)
//{
//	bool inArray = false;
//	while ((i - 1 >= 0) && (!inArray)) {	// Checks against the previous item(s) added to the array. Flag used to terminate loop early if found
//		if (item.x == array[i].x) {			// First property checked for equal value.
//			if (item.y == array[i].y) {		// Second property checked if the first was a match.
//				inArray = true;
//			}
//		}
//		--i;
//	}
//	return !inArray;
//}

//UNDONE void createPills(char maze[][SIZEX], vector<Item>& pills)
//{
//	void placeItem(char[][SIZEX], const Item& item);
//	for (int pillCount = 0; pillCount < MAXPILLS; pillCount++)
//	{
//		Item pill;
//		do
//		{
//			pill = { Random(SIZEX), Random(SIZEY), PILL };
//		} while (maze[pill.x][pill.y] != TUNNEL);
//		pills.push_back(pill);
//		placeItem(maze, pill);
//	}
//}
//UNDONE Hole assignment function
// IN: Array representing the maze, Amount of holes to use, Array to hold the hole items.
// OUT:
// Precondition: None
// Postcondition: All holes will be assigned an x and y coord, ready to insert into grid
//void setHoleInitialPosition(char maze[][SIZEX], int maxHoles, vector<Item>& holes) {
//	void setSpotInitialCoordinates(Item& spot, char maze[][SIZEX]);
//	//bool isPositionUnique(int i, const Item& item, const vector<Item>& array);
//	void placeItem(char g[][SIZEX], const Item& item);
//
//	for (int i = 0; i < maxHoles; ++i) {			// place holes until max is reached .
//		Item h = { 0,0, HOLE };						// Loop used here increments rather than decrements so that the
//		setSpotInitialCoordinates(h, maze);			// following Array comparision function doesnt have to take the array size variable.
//		//while (!isPositionUnique(i, h, holes))		// TODO Still having issue where spot can spawn on hole location.
//		//{											// Will probably need fixing in the spot placement function though.
//		//	setSpotInitialCoordinates(h, maze);
//		//}
//		holes.push_back(h);
//		placeItem(maze, h);
//	}
//	// Could have just set the holes into the base maze directly to render faster.
//	// Doing it this way allows us the option to move the holes ingame if we need to - additional functionality? :)
//}
*/