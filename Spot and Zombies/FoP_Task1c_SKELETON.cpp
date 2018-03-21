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
#include <fstream> // For saving/loading files

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
const int  UP(72);				// up arrow
const int  DOWN(80); 			// down arrow
const int  RIGHT(77);			// right arrow
const int  LEFT(75);			// left arrow

// defining the other command letters
const char QUIT('Q');			// to end the game
const char FREEZE('F');			// Cheat command 1, to freeze the zombies in place.
const char EXTERMINATE('X');	// Cheat command 2, eliminate all zombies
const char EAT('E');			// Cheat command 3, eat all remaining pills.

struct Item
{
	int x, y;
	char symbol;
	bool visible = true;
};

//struct Spot : Item {
//	int lives;
//};

struct PlayerInfo {
	string playerName;
	int score = 0;
	int highscore = 0;
	bool hasCheated = false;
};

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
	void displayEntryScreen(PlayerInfo& playerData);
	void initialiseGame(GameSpaceManager& gsm, const int numberOfHoles, const int MAXPILLS, GameObjectManager& gom);
	void paintGame(const GameSpaceManager& gsm, const PlayerInfo& playerData, const string& mess);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], GameObjectManager& gom, PlayerInfo& playerData, const int key, string& mess);
	void updateGrid(GameSpaceManager& gsm, const GameObjectManager& gom);
	void saveUserData(const PlayerInfo& playerData);
	void endProgram();
	// local variable declarations

	GameSpaceManager gsm;
	GameObjectManager gom;
	PlayerInfo playerData;

	string message("LET'S START...");	// current message to player

	// TODO difficulty level related variables - will need to change location when we add 
	const int numberOfHoles(12);
	const int MAXPILLS(8);

	// action...
	Seed();												// seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");
	displayEntryScreen(playerData);
	Clrscr();											// Using included libraries, clears the game screen - sets it all grey.
	initialiseGame(gsm, numberOfHoles, MAXPILLS, gom);	// initialise grid (incl. walls and spot)
	paintGame(gsm, playerData, message);				// display game info, modified grid and messages
	int key;											// current key selected by player
	do
	{
		key = toupper(getKeyPress()); 	// read in  selected key: arrow or letter command
		if (isArrowKey(key))
		{
			updateGameData(gsm.grid, gom, playerData, key, message);			// move spot in that direction
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
				message = "INVALID KEY!";			// set 'Invalid key' message
			}
		}
		paintGame(gsm, playerData, message);		// display game info, modified grid and messages
	} while (!wantsToQuit(key));					// while user does not want to quit
	saveUserData(playerData);
	endProgram();									// display final message
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

	setInitialMazeStructure(gsm.maze);								// initialise maze
																	//TODO Place zombies first so that nothing spawns over the corners.
	setMultipleItems(HOLE, numberOfHoles, gom.holes, gsm.maze);		//Place the holes second
	setMultipleItems(PILL, MAXPILLS, gom.pills, gsm.maze);			// Place the pills
	setItemInitialCoordinates(gom.spot, SPOT, gsm.maze);			// Finally place Spot
	updateGrid(gsm, gom);											// prepare grid
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
	setMaze(gsm.grid, gsm.maze);					// reset the empty maze configuration into grid 
	placeMultipleItems(gsm.grid, gom.holes);		// Place holes on the grid
	placeMultipleItems(gsm.grid, gom.pills);		// Place pills onto the grid
	placeItem(gsm.grid, gom.spot);					// set spot in grid
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
	g[item.y][item.x] = (item.visible) ? item.symbol : ' ';
}

// Multiple item placement function
// IN: Array representing the maze, Vector holding the items.
// OUT:
// Precondition: None
// Postcondition: All items will be placed on grid
void placeMultipleItems(char g[][SIZEX], const vector<Item>& itemStore) {
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i(itemStore.size() - 1); i >= 0; --i) { // place items until max is reached
		// TODO Item renderer.. probably a good place for a visible check here.
		placeItem(g, itemStore.at(i));
	}
}

// ---------------------------------------------------------------------------
// ----- move items on the grid
// ---------------------------------------------------------------------------
void updateGameData(const char g[][SIZEX], GameObjectManager& gom, PlayerInfo& playerData, const int key, string& mess)
{
	// move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	void eatPill(GameObjectManager& gom, PlayerInfo& playerData);
	assert(isArrowKey(key));

	// reset message to blank
	mess = "                                         ";		// reset message to blank

	// calculate direction of movement for given key
	int dx(0), dy(0);
	setKeyDirection(key, dx, dy);

	// check new target position in grid and update game data (incl. spot coordinates) if move is possible
	switch (g[gom.spot.y + dy][gom.spot.x + dx])
	{
		// ...depending on what's on the target position in grid...
	case TUNNEL:		// can move
		gom.spot.y += dy;	// go in that Y direction
		gom.spot.x += dx;	// go in that X direction
		break;
	case WALL:  		// hit a wall and stay there
		mess = "CANNOT GO THERE!";
		break;
	case PILL:
		gom.spot.x += dx;
		gom.spot.y += dy;
		eatPill(gom, playerData);
	}
}

void eatPill(GameObjectManager& gom, PlayerInfo& playerData)
{
	for(int i = 0; i < gom.pills.size(); i++)
	{
		if (gom.pills.at(i).x == gom.spot.x && gom.pills.at(i).y == gom.spot.y)
		{
			gom.pills.at(i).visible = false;
			playerData.score++;
		}
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

// Display the games title on sequential lines from a set starting point in defined colours.
void showGameTitle(const WORD backColour, const WORD textColour, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(backColour, textColour, x, y, "--------------------");
	showMessage(backColour, textColour, x, y + 1, "| SPOT AND ZOMBIES |");
	showMessage(backColour, textColour, x, y + 2, "--------------------");
}

// Display the project members on sequential lines from a set starting point in defined colours.
void showGroupMembers(const WORD backColour, const WORD textColour, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(backColour, textColour, x, y, "GROUP CS4G1A  -  2017-18");
	showMessage(backColour, textColour, x, y + 1, "Charlie Batten  ");
	showMessage(backColour, textColour, x, y + 2, "Matt Bellamy    ");
	showMessage(backColour, textColour, x, y + 3, "Nico Caruana    27022205");
}

// Display a request for the user to enter username from a set starting point in a defined colour.
// User Entry text is then changed to a red colour.
void displayNameRequest(const WORD backColour, const WORD textColour, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(backColour, textColour, x, y, "Enter your name: ");
	SelectTextColour(clGreen);
}

//TODO Nico - Basic function to get username, need to add validity checks and length limits.
string getUserName() {
	string playerName;
	cin >> playerName;
	return playerName;
}

void checkAndLoadUserSavedData(const string& userName, PlayerInfo& playerData) {
	ifstream fin("saves/" + userName + ".txt");		// Attempt to open the user's previous save file.
	if (fin.fail()) {						// If the file is not present, assume new user.
		playerData.playerName = userName;	// Set the username entered to be the players name for this session.
	}
	else {									// Otherwise extract the player name and high score from the save file.
		string tempUserName;
		int tempScore;
		fin >> tempUserName;
		fin >> tempScore;
		playerData.playerName = tempUserName;
		playerData.highscore = tempScore;
	}
}
//TODO Nico
void saveUserData(const PlayerInfo& playerData) {
	ofstream fout;
	fout.open("saves/" + playerData.playerName + ".txt");
	if (fout.fail()) {
		//TODO Throw an error
	}
	else {
		fout << playerData.playerName << " " << ((playerData.score > playerData.highscore) ? playerData.score : playerData.highscore);
	}
}
//TODO Nico
void displayPlayerInformation(const struct PlayerInfo& playerData) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, 40, 19, "Player Name: " + playerData.playerName);
	showMessage(clDarkGrey, clYellow, 40, 20, "Score:      " + tostring(playerData.score));
	showMessage(clDarkGrey, clYellow, 40, 21, "High Score: " + tostring(playerData.highscore));
}

// TODO Nico
// Entry screen display
void displayEntryScreen(struct PlayerInfo& playerData) {
	void showGameTitle(const WORD backColour, const WORD textColour, int x, int y);
	void showGroupMembers(const WORD backColour, const WORD textColour, int x, int y);
	void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y);
	void displayNameRequest(const WORD backColour, const WORD textColour, int x, int y);
	void checkAndLoadUserSavedData(const string& userName, struct PlayerInfo& playerData);
	string getUserName();
	void saveUserData(const PlayerInfo& playerData);

	showGameTitle(clDarkGrey, clYellow, 10, 6);
	showGroupMembers(clDarkGrey, clYellow, 10, 10);
	displayTimeAndDate(clDarkGrey, clYellow, 40, 1);
	displayNameRequest(clDarkGrey, clYellow, 10, 20);
	checkAndLoadUserSavedData(getUserName(), playerData);
	saveUserData(playerData); //HACK Just for testing save function.
}

void paintGame(const GameSpaceManager& gsm, const PlayerInfo& playerData, const string& mess)
{
	void displayPlayerInformation(const PlayerInfo& playerData);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	void paintGrid(const GameSpaceManager& gsm);
	void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y);
	// display game title
	showMessage(clDarkGreen, clGreen, 0, 0, "___SPOT AND ZOMBIES GAME___");
	showMessage(clYellow, clBlue, 40, 0, "FoP Task 1c: February 2018");
	displayTimeAndDate(clYellow, clBlue, 40, 1);

	// print auxiliary messages if any
	showMessage(clBlack, clYellow, 40, 6, mess);	// display current message
	// display menu options available
	showMessage(clRed, clGreen, 40, 8, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(clRed, clGreen, 40, 9, "TO QUIT ENTER 'Q'           ");

	displayPlayerInformation(playerData);

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
		{
			switch (gsm.grid[row][col])
			{
			case SPOT:
				SelectTextColour(clGreen);
				break;
			case PILL:
				SelectTextColour(clYellow);
				break;
			case HOLE:
				SelectTextColour(clGrey);
				break;
			case WALL:
				SelectTextColour(clWhite);
				break;
			case ZOMBIE:
				SelectTextColour(clRed);
				break;
			}
			cout << gsm.grid[row][col];	// output cell content
		}
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