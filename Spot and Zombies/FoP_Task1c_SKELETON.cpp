// ---------------------------------------------------------------------------
// Program: Task 1c – group assignment
// Author: Charlie Batten, Matt Bellamy, Nico Caruana
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
#include <time.h>	// For time display
#include <vector>	// For storing item data 
#include <fstream>	// For saving/loading files

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
const char SPOT('@');
const char TUNNEL(' ');
const char WALL('#');
const char HOLE('0');
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
	bool alive = true;
};

struct PlayerInfo {
	string playerName;
	int score = 5;				// Represents lives remaining in game. Set to 5 initially to match basic version.
	int highscore = -1;			// High score default value set to -1 as per the specification.
	bool hasCheated = false;	// Flag used to determine whether score is recorded at the end of the game.
};

struct GameSpaceManager
{
	char grid[SIZEY][SIZEX];	// grid for display
	char maze[SIZEY][SIZEX];	// structure of the maze
};

struct GameObjectManager
{
	vector<Item> holes, pills, zombies;
	Item spot;	
};

struct GameData {
	int numberOfHoles = 12;
	int maxPills = 8;
	int maxNumberOfZombies = 4;
	int numberOfPillsLeft = maxPills;
	int zombiesLeft = maxNumberOfZombies;

};

// TODO Add a difficulty struct with max number of items - Advanced tasks!


// ---------------------------------------------------------------------------
// ----- run game
// ---------------------------------------------------------------------------

int main()
{
	// function declarations (prototypes)
	void displayEntryScreen(PlayerInfo& playerData);
	void initialiseGame(GameSpaceManager& gsm, GameObjectManager& gom, GameData& gameData);
	void paintGame(const GameSpaceManager& gsm, const PlayerInfo& playerData, const GameData& gameData, const string& mess);
	void spawnZombies(char grid[][SIZEX], vector<Item>& zombies);
	void killZombies(vector<Item>& zombies);
	bool allZombiesDead(vector<Item> zombies);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], GameObjectManager& gom, PlayerInfo& playerData, const int key, string& mess);
	void updateGrid(GameSpaceManager& gsm, GameObjectManager& gom);
	void saveUserData(const PlayerInfo& playerData);
	void endProgram();
	// local variable declarations

	GameSpaceManager gsm;
	GameObjectManager gom;
	PlayerInfo playerData;
	GameData gameData;

	string message("LET'S START...");	// current message to player

	// action...
	Seed();												// seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");
	displayEntryScreen(playerData);
	Clrscr();											// Using included libraries, clears the game screen - sets it all grey.
	initialiseGame(gsm, gom, gameData);	// initialise grid (incl. walls and spot)	
	paintGame(gsm, playerData, gameData, message);				// display game info, modified grid and messages
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
				playerData.hasCheated = true;
				//TODO STUB - Freeze the zombies in place
				break;
			case EXTERMINATE:
				playerData.hasCheated = true;
				//Exterminate all zombies on screen
				if (allZombiesDead(gom.zombies))
				{
					// TODO Need a different way of respawning the zombies. Doing it this way temporarily adds them back to screen,
					// but they are not added as 0-3 in the vector, meaning they disappear next frame.
					// Probably best to just reflag them alive and reset position manually.
					//spawnZombies(gsm.grid, gom.zombies);
				}
				//If all zombies are already dead, resets them to default spawn locations and respawns them
				else
				{
					killZombies(gom.zombies);
				}
				break;
			case EAT:
				playerData.hasCheated = true;
				for (int i = 0; i < gom.pills.size(); i++)
				{
					gom.pills.at(i).visible = false;					
				}
				gameData.numberOfPillsLeft = 0;
				break;
			case QUIT:
				break; //Maybe do something here.. put it in for now to simply get rid of the invalid key message before terminating loop on quit command.

			default:
				message = "INVALID KEY!";	// set 'Invalid key' message
			}
			updateGrid(gsm, gom); //Re-Update grid to apply changes
		}
		paintGame(gsm, playerData, gameData, message);		// display game info, modified grid and messages
	} while (!wantsToQuit(key));					// while user does not want to quit
	// TODO GameOver
	endProgram();									// display final message
	return 0;
}


// ---------------------------------------------------------------------------
// ----- initialise game state
// ---------------------------------------------------------------------------

void initialiseGame(GameSpaceManager& gsm, GameObjectManager& gom, GameData& gameData)
{
	// initialise grid and place spot in middle
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void setItemInitialCoordinates(Item& item, const char itemSymbol, char[][SIZEX]);
	void updateGrid(GameSpaceManager& gsm, GameObjectManager& gom);
	void setMultipleItems(const char itemSymbol, int maxNumOfItems, vector<Item>& itemStore, char grid[][SIZEX]);
	void spawnZombies(char grid[][SIZEX], vector<Item>& zombies);

	setInitialMazeStructure(gsm.maze);								// initialise maze
	setMaze(gsm.grid, gsm.maze);									// Create first empty game frame
	spawnZombies(gsm.grid, gom.zombies);							// Place zombies first so that nothing spawns over the corners.
	setMultipleItems(HOLE, gameData.numberOfHoles, gom.holes, gsm.grid);		// Place the holes second
	setMultipleItems(PILL, gameData.maxPills, gom.pills, gsm.grid);			// Place the pills
	setItemInitialCoordinates(gom.spot, SPOT, gsm.grid);			// Finally place Spot
}

void setItemInitialCoordinates(Item& item, const char itemSymbol, char grid[][SIZEX])
{												// NICO - Replaced the constant recalling itself with a do-while loop - more efficient!
	void placeItem(char g[][SIZEX], const Item& item);
	do {
		item.x = Random(SIZEX - 2);
		item.y = Random(SIZEY - 2);
	} while (grid[item.y][item.x] != TUNNEL);
	item.symbol = itemSymbol;
	placeItem(grid, item);
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

void setMultipleItems(const char itemSymbol, int maxNumOfItems, vector<Item>& itemStore, char grid[][SIZEX])
{
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i = maxNumOfItems; i > 0; --i) {
		Item i1;
		setItemInitialCoordinates(i1, itemSymbol, grid);
		itemStore.push_back(i1);
		placeItem(grid, i1);
	}
}

// Zombies alive checking function
// IN: Vector representing all zombies
// OUT: Boolean dictating if all zombies are alive. False even if even a single zombie is still alive
// Precondition: None
// Postcondition: Identified whether all zombies are dead
bool allZombiesDead(vector<Item> zombies)
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
void killZombies(vector<Item>& zombies)
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
void spawnZombies(char grid[][SIZEX], vector<Item>& zombieStore)
{
	void placeItem(char g[][SIZEX], const Item& item);

	//Position defaults
	int xDef[4] = { 1, 1, SIZEX - 2, SIZEX - 2 };
	int yDef[4] = { 1, SIZEY - 2, SIZEY - 2, 1 };

	for (int i = 0; i < 4; i++)
	{
		Item z1;
		z1.x = xDef[i];
		z1.y = yDef[i];
		z1.alive = true;
		z1.symbol = ZOMBIE;
		z1.visible = 1;
		zombieStore.push_back(z1);
		placeItem(grid, z1);
	}
}

// Zombies placement function
// IN: Array representing the grid, Vector representing all zombies
// OUT: Relevant zombies placed on relevant coordinates on the grid
// Precondition: None
// Postcondition: All zombies that are alive are placed on the grid at coordinates to match their own coordinate values
void placeZombies(char grid[][SIZEX], vector<Item> zombies)
{
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i = 0; i < 4; i++)
	{
		if (zombies.at(i).alive)
		{
			placeItem(grid, zombies[i]);
		}
	}
}


// ---------------------------------------------------------------------------
// ----- update grid state
// ---------------------------------------------------------------------------

void updateGrid(GameSpaceManager& gsm, GameObjectManager& gom)
{
	// update grid configuration after each move
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item& spot);
	void placeMultipleItems(char g[][SIZEX], const vector<Item>& itemStore);
	void placeZombies(char g[][SIZEX], vector<Item> zombies);

	// Not sent as complete GSM/GOM, to keep constant array restrictions.
	setMaze(gsm.grid, gsm.maze);					// reset the empty maze configuration into grid 
	placeZombies(gsm.grid, gom.zombies);			// place the zombies
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
	if (item.visible) { // check for visible flag before rendering
		g[item.y][item.x] = item.symbol;
	}
}

// Multiple item placement function
// IN: Array representing the grid, Vector holding the items.
// OUT:
// Precondition: None
// Postcondition: All items will be placed on grid
void placeMultipleItems(char g[][SIZEX], const vector<Item>& itemStore) {
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i(itemStore.size() - 1); i >= 0; --i) { // place items until max is reached
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
		break;
	case HOLE:
		gom.spot.x += dx;
		gom.spot.y += dy;
		playerData.score--;
		if (playerData.score <= 0) {
			//TODO STUB GameOver
		}
	}
}

void eatPill(GameObjectManager& gom, PlayerInfo& playerData)
{
	for (int i = 0; i < gom.pills.size(); i++)
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

// TODO Nico
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
void displayPlayerInformation(const PlayerInfo& playerData, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, x, y, "Player Name: " + playerData.playerName);
	showMessage(clDarkGrey, clYellow, x, y+1, "Lives:      " + tostring(playerData.score));
	showMessage(clDarkGrey, clYellow, x, y+2, "High Score: " + tostring(playerData.highscore));
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

void displayGameInformation(const GameData& gameData, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, x, y, "NUMBER OF PILLS REMAINING: " + tostring(gameData.numberOfPillsLeft));

}

void paintGame(const GameSpaceManager& gsm, const PlayerInfo& playerData, const GameData& gameData, const string& mess)
{
	void displayPlayerInformation(const PlayerInfo& playerData, int x, int y);
	void displayGameInformation(const GameData& gameData, int x, int y);

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

	displayPlayerInformation(playerData, 40, 19);
	displayGameInformation(gameData, 40, 25);

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

void gameOver(const PlayerInfo& playerData) {
	void saveUserData(const PlayerInfo& playerData);
	if (!playerData.hasCheated) { // TODO Should probably pull this out of here and make a generic game over function that can be used after death/win too.
		saveUserData(playerData);
	}
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