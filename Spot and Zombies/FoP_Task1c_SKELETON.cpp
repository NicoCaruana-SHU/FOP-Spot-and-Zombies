// TODO My tasks, code the holes and the cheats.!!!
// ---------------------------------------------------------------------------
// Program: Skeleton for Task 1c – group assignment
// Author: Pascale Vacher
// Last updated: 26 February 2018
// ---------------------------------------------------------------------------

// Go to 'View > Task List' menu to open the 'Task List' pane listing the initial amendments needed to this program

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
const char ZOMBIE('Z');
// defining the command letters to move the spot on the maze
const int  UP(72);			// up arrow
const int  DOWN(80); 		// down arrow
const int  RIGHT(77);		// right arrow
const int  LEFT(75);		// left arrow
// defining numerical constants
const int MAXPILLS(8);
// defining the other command letters
const char QUIT('Q');			// to end the game
const char FREEZE('F');			// Cheat command 1, to freeze the zombies in place.
const char EXTERMINATE('X');	// Cheat command 2, eliminate all zombies
const char EAT('E');			// Cheat command 3, eat all remaining pills.

// 

struct Item
{
	int x, y;
	char symbol;
};

struct Zombie
{
	int x, y;
	char symbol;
	bool alive;
};

// ---------------------------------------------------------------------------
// ----- run game
// ---------------------------------------------------------------------------

int main()
{
	// function declarations (prototypes)
	void displayEntryScreen();
	void initialiseGame(char g[][SIZEX], char m[][SIZEX], Item& spot, int numberOfHoles, Item holes[], vector<Item> &pills, vector<Zombie> &zombies);
	void paintGame(const char g[][SIZEX], string mess);
	void spawnZombies(char maze[][SIZEX], vector<Zombie> &zombies);
	void killZombies(vector<Zombie> &zombies);
	bool allZombiesDead(vector<Zombie> zombies);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], Item& spot, const int key, string& mess);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], const Item spot, int maxHoles, Item holes[], vector<Zombie> zombies);
	void endProgram();
	void getTime(struct tm &timeLocal);


	// local variable declarations 
	char grid[SIZEY][SIZEX];			// grid for display
	char maze[SIZEY][SIZEX];			// structure of the maze
	vector<Item> pills;
	Item spot = { 0, 0, SPOT }; 		// spot's position and symbol
	string message("LET'S START...");	// current message to player

	// Hole placement related variables - will need to change when we add difficulty levels
	const int numberOfHoles(12);
	Item holes[numberOfHoles];

	// Declaration of Zombies - initial placement is done in 'spawnZombies' function
	vector<Zombie> zombies = { {0, 0, ZOMBIE, true}, {0, 0, ZOMBIE, true}, {0, 0, ZOMBIE, true}, {0, 0, ZOMBIE, true} };


	// action...
	Seed();								// seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");
	displayEntryScreen();
	cin.get(); // HACK temporarily hold display here to view entry screen.
	initialiseGame(grid, maze, spot, numberOfHoles, holes, pills, zombies);	// initialise grid (incl. walls and spot)
	spawnZombies(grid, zombies);		// Initial spawning of zombies
	paintGame(grid, message);			// display game info, modified grid and messages
	int key;							// current key selected by player

	int xz = 0;
	do
	{
		key = toupper(getKeyPress()); 	// read in  selected key: arrow or letter command
		if (isArrowKey(key))
		{
			updateGameData(grid, spot, key, message);		// move spot in that direction
			updateGrid(grid, maze, spot, numberOfHoles, holes, zombies);					// update grid information
		}
		else {
			switch (key) {
			case FREEZE:
				//TODO STUB - Freeze the zombies in place
				break;
			case EXTERMINATE:
				//Exterminate all zombies on screen
				if (allZombiesDead(zombies))
				{
					spawnZombies(grid, zombies);
				}
				//If all zombies are already dead, resets them to default spawn locations and respawns them
				else
				{
					killZombies(zombies);
					updateGrid(grid, maze, spot, numberOfHoles, holes, zombies); //Re-Update grid to apply dead zombies
				}
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
		paintGame(grid, message);		// display game info, modified grid and messages
	} while (!wantsToQuit(key));		// while user does not want to quit
	endProgram();						// display final message
	return 0;
}


// ---------------------------------------------------------------------------
// ----- initialise game state
// ---------------------------------------------------------------------------

void initialiseGame(char grid[][SIZEX], char maze[][SIZEX], Item& spot, int numberOfHoles, Item holes[], vector<Item> &pills, vector<Zombie> &zombies)
{
	// initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setSpotInitialCoordinates(Item& spot, char[][SIZEX]);
	void updateGrid(char g[][SIZEX], const char m[][SIZEX], Item b, int maxHoles, Item holes[], vector<Zombie> zombies);
	void setHoleInitialPosition(char maze[][SIZEX], int maxHoles, Item holes[]);
	void createPills(char[][SIZEX], vector<Item> &pills);

	setInitialMazeStructure(maze);		// initialise maze
	setHoleInitialPosition(maze, numberOfHoles, holes);
	setSpotInitialCoordinates(spot, maze);
	createPills(maze, pills);
	updateGrid(grid, maze, spot, numberOfHoles, holes, zombies);		// prepare grid
}

void setSpotInitialCoordinates(Item& spot, char maze[][SIZEX])
{
	// set spot coordinates inside the grid at random at beginning of game
	spot.y = Random(SIZEY - 2);      // vertical coordinate in range [1..(SIZEY - 2)]
	spot.x = Random(SIZEX - 2);      // horizontal coordinate in range [1..(SIZEX - 2)]
	if (maze[spot.x][spot.y] != TUNNEL)
		setSpotInitialCoordinates(spot, maze);
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

//TODO Hole assignment function
// IN: Array representing the maze, Amount of holes to use, Array to hold the hole items.
// OUT:
// Precondition: None
// Postcondition: All holes will be assigned an x and y coord, ready to insert into grid
void setHoleInitialPosition(char maze[][SIZEX], int maxHoles, Item holes[]) {
	void setSpotInitialCoordinates(Item& spot, char maze[][SIZEX]);
	bool isPositionUnique(int i, const Item& item, Item array[]);

	for (int i = 0; i < maxHoles; ++i) {			// place holes until max is reached .
		Item h = { 0,0, HOLE };						// Loop used here increments rather than decrements so that the 
		setSpotInitialCoordinates(h, maze);			// following Array comparision function doesnt have to take the array size variable.
		while (!isPositionUnique(i, h, holes))		// TODO Still having issue where spot can spawn on hole location.
		{											// Will probably need fixing in the spot placement function though.
			setSpotInitialCoordinates(h, maze);
		}
		holes[i] = h;
	}
	// Could have just set the holes into the base maze directly to render faster.
	// Doing it this way allows us the option to move the holes ingame if we need to - additional functionality? :)
}

// TODO Array comparison function. Quite inefficient to use, if you can think of a better way, let us know!
// IN: Copy of intended position variable to use as iterator, const reference of the item to verify, Array of items to check uniqueness against.
// OUT: TRUE if Item is unique, FALSE if item already present.
// Precondition: 
// Postcondition:
bool isPositionUnique(int i, const Item& item, Item array[])
{
	bool inArray = false;
	while ((i - 1 >= 0) && (!inArray)) {	// Checks against the previous item(s) added to the array. Flag used to terminate loop early if found
		if (item.x == array[i].x) {			// First property checked for equal value.
			if (item.y == array[i].y) {		// Second property checked if the first was a match.
				inArray = true;
			}
		}
		--i;
	}
	return !inArray;
}

void createPills(char maze[][SIZEX], vector<Item> &pills)
{
	void placeItem(char[][SIZEX], Item);
	for (int pillCount = 0; pillCount < MAXPILLS; pillCount++)
	{
		Item pill;
		do
		{
			pill = { Random(SIZEX), Random(SIZEY), PILL };
		} while (maze[pill.x][pill.y] != TUNNEL);
		pills.push_back(pill);
		placeItem(maze, pill);
	}
}

// Zombies alive checking function
// IN: Vector representing all zombies
// OUT: Boolean dictating if all zombies are alive. False even if even a single zombie is still alive
// Precondition: None
// Postcondition: Identified whether all zombies are dead
bool allZombiesDead(vector<Zombie> zombies)
{
	bool returnVal = true;
	int i = 0;
	while (returnVal == true && i < 4)
	{
		returnVal = !zombies.at(i).alive;	// Sets returnVal to true if zombie is dead, false and breaks loop if alive
		i++;
	}
	return returnVal;
}

// Zombies killing cheat function
// IN: Vector representing all zombies
// OUT: New form of zombies vector with false .alive values
// Precondition: None
// Postcondition: All zombies referenced in function call are killed
void killZombies(vector<Zombie> &zombies)
{
	for (int i = 0; i < 4; i++)
	{
		zombies.at(i).alive = false;
	}
}

// Zombies default spawning function
// IN: Array representing the maze, vector representing (and referencing) all zombies
// OUT: Zombie coordinates reset to defaults, zombies set to be alive
// Precondition: None
// Postcondition: All zombies placed in their corners and alive
void spawnZombies(char maze[][SIZEX], vector<Zombie> &zombies)
{
	void placeZombies(char[][SIZEX], vector<Zombie>);
	//Position defaults
	int xDef[4] = { 1, 1, 23, 23 };
	int yDef[4] = { 1, 18, 18, 1 };

	for (int i = 0; i < 4; i++)
	{
		zombies.at(i).x = xDef[i];
		zombies.at(i).y = yDef[i];
		zombies.at(i).alive = true;
	}
	placeZombies(maze, zombies);
}

// Zombies placement function
// IN: Array representing the maze, Vector representing all zombies
// OUT: Relevant zombies placed on relevant coordinates on the grid
// Precondition: None
// Postcondition: All zombies that are alive are placed on the grid at coordinates to match their own coordinate values
void placeZombies(char maze[][SIZEX], vector<Zombie> zombies)
{
	void placeItem(char[][SIZEX], Item);

	for (int i = 0; i < 4; i++)
	{
		if (zombies.at(i).alive)
		{
			Item zombie = { zombies.at(i).x, zombies.at(i).y, zombies.at(i).symbol };
			placeItem(maze, zombie);
		}
	}
}


// ---------------------------------------------------------------------------
// ----- update grid state
// ---------------------------------------------------------------------------

void updateGrid(char grid[][SIZEX], const char maze[][SIZEX], const Item spot, int maxHoles, Item holes[], vector<Zombie> zombies)
{
	// update grid configuration after each move
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item spot);
	void placeHoles(char g[][SIZEX], int maxHoles, Item holes[]);
	void placeZombies(char g[][SIZEX], vector<Zombie> zombies);

	setMaze(grid, maze);	// reset the empty maze configuration into grid
	placeHoles(grid, maxHoles, holes);
	placeZombies(grid, zombies);
	placeItem(grid, spot);	// set spot in grid
}

void setMaze(char grid[][SIZEX], const char maze[][SIZEX])
{
	// reset the empty/fixed maze configuration into grid
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

void placeItem(char g[][SIZEX], const Item item)
{
	// place item at its new position in grid
	g[item.y][item.x] = item.symbol;
}

// Hole placement function
// IN: Array representing the maze, Amount of holes to use, Array holding the hole items.
// OUT:
// Precondition: None
// Postcondition: All holes will be placed on grid

// TODO Could probably wrap numberofholes and array of holes together in a struct for ease of passing data.
void placeHoles(char g[][SIZEX], int maxHoles, Item holes[]) {
	void placeItem(char g[][SIZEX], const Item item);

	for (int i(maxHoles - 1); i >= 0; --i) { // place holes until max is reached
		placeItem(g, holes[i]);
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

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message)
{
	// display a string using specified colour at a given position 
	Gotoxy(x, y);
	SelectBackColour(backColour);
	SelectTextColour(textColour);
	cout << message;
}

// TODO Nico
void showGameTitle(int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);

	showMessage(clDarkGrey, clYellow, x, y, "--------------------");
	showMessage(clDarkGrey, clYellow, x, y + 1, "| SPOT AND ZOMBIES |");
	showMessage(clDarkGrey, clYellow, x, y + 2, "--------------------");
}

// TODO Nico
void showGroupMembers(int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);

	showMessage(clDarkGrey, clYellow, x, y, "GROUP CS4G1A  -  2017-18");
	showMessage(clDarkGrey, clYellow, x, y + 1, "Charlie Batten  ");
	showMessage(clDarkGrey, clYellow, x, y + 2, "Matt Bellamy    26012012");
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

// TODO Nico
// Clear screen
void clearScreen() {

}






void paintGame(const char g[][SIZEX], string mess)
{
	void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y);


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
	void paintGrid(const char g[][SIZEX]);
	// display game title
	showMessage(clDarkGreen, clGreen, 0, 0, "___GAME___");

	displayTimeAndDate(clYellow, clBlue, 40, 0);

	showMessage(clYellow, clBlue, 40, 2, "FoP Task 1c: February 2018");

	// display menu options available
	showMessage(clRed, clGreen, 40, 8, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clRed, clGreen, 40, 9, "TO QUIT ENTER 'Q'           ");

	// print auxiliary messages if any
	showMessage(clBlack, clYellow, 40, 8, mess);	// display current message



	// display grid contents
	paintGrid(g);
}

void paintGrid(const char g[][SIZEX])
{
	// display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row)
	{
		for (int col(0); col < SIZEX; ++col)
			cout << g[row][col];	// output cell content
		cout << endl;
	}
}

void endProgram()
{
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string message);
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