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
	int defaultX, defaultY;		// Mainly used for zombies so they know where to go back to
	char symbol;
	bool visible = true;
	bool alive = true;
};

struct PlayerInfo {
	string playerName;
	int highscore = -1;			// High score default value set to -1 as per the specification.	
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

struct GameData {				// Default game variable state, can be constructed differently for difficulties later.
	int lives = 5;				// Represents lives remaining in game. Set to 5 initially to match basic version.
	int numberOfHoles = 12;
	int maxPills = 8;
	int maxNumberOfZombies = 4;
	int numberOfPillsLeft = maxPills;
	int zombiesLeft = maxNumberOfZombies;
	bool hasCheated = false;	// Flag used to determine whether score is recorded at the end of the game.
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
	void killZombies(vector<Item>& zombies);
	void spawnZombies(char grid[][SIZEX], vector<Item>& zombieStore, GameData& data);
	bool allZombiesDead(vector<Item> zombies);
	void moveZombies(const char grid[][SIZEX], GameObjectManager& gom, GameData& data);
	bool wantsToQuit(const int key);
	bool isArrowKey(const int k);
	int  getKeyPress();
	void updateGameData(const char g[][SIZEX], GameObjectManager& gom, GameData& gameData, const int key, string& mess);
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
			updateGameData(gsm.grid, gom, gameData, key, message);			// move spot in that direction
			updateGrid(gsm, gom);									// update grid information
		}
		else {
			switch (key) {
			case FREEZE:
				gameData.hasCheated = true;
				//TODO STUB - Freeze the zombies in place
				break;
			case EXTERMINATE:
				gameData.hasCheated = true;
				//If all zombies are already dead, resets them to default spawn locations and respawns them
				if (allZombiesDead(gom.zombies))
				{
					// NOTE: spawnZombies was changed to use existing data for purposes such as this; replaced with setZombies
					spawnZombies(gsm.grid, gom.zombies, gameData);
				}
				//Exterminate all zombies on screen
				else
				{
					killZombies(gom.zombies);
					gameData.zombiesLeft = 0;
				}
				break;
			case EAT:
				gameData.hasCheated = true;
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
	void setItemInitialCoordinates(const char itemSymbol, Item& item, char[][SIZEX]);
	void updateGrid(GameSpaceManager& gsm, GameObjectManager& gom);
	void setMultipleItems(const char itemSymbol, int maxNumOfItems, vector<Item>& itemStore, char grid[][SIZEX]);
	void setZombies(char grid[][SIZEX], vector<Item>& zombies, GameData data);

	setInitialMazeStructure(gsm.maze);								// initialise maze
	setMaze(gsm.grid, gsm.maze);									// Create first empty game frame
	setZombies(gsm.grid, gom.zombies, gameData);							// Place zombies first so that nothing spawns over the corners.
	setMultipleItems(HOLE, gameData.numberOfHoles, gom.holes, gsm.grid);		// Place the holes second
	setMultipleItems(PILL, gameData.maxPills, gom.pills, gsm.grid);			// Place the pills
	setItemInitialCoordinates(SPOT, gom.spot, gsm.grid);			// Finally place Spot
}

void setItemInitialCoordinates(const char itemSymbol, Item& item, char grid[][SIZEX])
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
	void setItemInitialCoordinates(const char itemSymbol, Item& item, char grid[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i = maxNumOfItems; i > 0; --i) {
		Item i1;
		setItemInitialCoordinates(itemSymbol,i1, grid);
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

// Zombies default init function
// IN: Array representing the maze, vector representing (and referencing) all zombies
// OUT: Zombie coordinates set to defaults, zombies set to be alive
// Precondition: None
// Postcondition: All zombies placed in their initial corners and alive
void setZombies(char grid[][SIZEX], vector<Item>& zombieStore, GameData data)
{
	void spawnZombies(char grid[][SIZEX], vector<Item>& zombieStore, GameData& data);

	//Position defaults
	int xDef[4] = { 1, 1, SIZEX - 2, SIZEX - 2 };
	int yDef[4] = { 1, SIZEY - 2, SIZEY - 2, 1 };

	for (int i = 0; i < 4; i++)
	{
		Item z1;
		z1.defaultX = xDef[i];
		z1.defaultY = yDef[i];
		z1.alive = true;
		z1.symbol = ZOMBIE;
		z1.visible = 1;
		zombieStore.push_back(z1);
	}
	spawnZombies(grid, zombieStore, data);
}

// Zombies spawning function
// IN: Array representing the maze, vector representing (and referencing) all zombies
// OUT: Zombie coordinates set to defaults, zombies set to be alive
// Precondition: None
// Postcondition: All zombies placed in their initial corners and alive
void spawnZombies(char grid[][SIZEX], vector<Item>& zombieStore, GameData& data)
{
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i = 0; i < 4; i++)
	{
		zombieStore.at(i).x = zombieStore.at(i).defaultX;
		zombieStore.at(i).y = zombieStore.at(i).defaultY;
		zombieStore.at(i).alive = true;
		placeItem(grid, zombieStore.at(i));
	}
	data.zombiesLeft = 4;
}

// Zombies movement function
// IN: Array representing the maze, GameObjectManager reference, GameData reference
// OUT:
// Precondition:
// Postcondition: Zombie(s) moved towards Spot, and determined to have fallen, bumped or hit Spot and taken necessary action
void moveZombies(const char grid[][SIZEX], GameObjectManager& gom, GameData& data)
{
	void zombiesHitSpot(GameObjectManager& go, GameData& gd);
	void zombiesBumped(vector<Item>& zombieStore);
	void zombiesFell(GameObjectManager& go, GameData& gd);

	for (int i = 0; i < 4; i++)
	{
		// Only moves if alive
		if (gom.zombies.at(i).alive)
		{
			// Only does all checks if coordinates are not equal
			if (gom.zombies.at(i).x != gom.spot.x || gom.zombies.at(i).y != gom.spot.y)
			{
				// X MOVEMENT
				// Move towards Spot if Spot X is to lower
				if (gom.zombies.at(i).x > gom.spot.x)
				{
					// 1/4 chance to not move
					if ((rand() % 4) != 1)
					{
						gom.zombies.at(i).x--;
					}
				}
				// Move towards Spot if Spot X is higher
				else if (gom.zombies.at(i).x < gom.spot.x)
				{
					// 1/4 chance to not move
					if ((rand() % 4) != 1)
					{
						gom.zombies.at(i).x++;
					}
				}

				// Y MOVEMENT
				// Move towards Spot if Spot Y is lower
				if (gom.zombies.at(i).y > gom.spot.y)
				{
					// 1/4 chance to not move
					if ((rand() % 4) != 1)
					{
						gom.zombies.at(i).y--;
					}
				}
				// Move towards Spot if Spot Y is higher
				else if (gom.zombies.at(i).y < gom.spot.y)
				{
					// 1/4 chance to not move
					if ((rand() % 4) != 1)
					{
						gom.zombies.at(i).y++;
					}
				}
			}
		}
		//Checks if zombies have fallen into holes
		zombiesFell(gom, data);

		// Checks if zombies have run into spot and causes damage+resets zombie if applicable
		zombiesHitSpot(gom, data);

		// Checks to see if zombies have bumped into one another through movement and resets their coordinates if necessary
		zombiesBumped(gom.zombies);
	}
}


// Zombies falling listener+handler
// IN: GameObjectManager reference, GameData reference
// OUT:
// Precondition:
// Postcondition: Zombies that have walked onto holes are killed and removed from the grid
void zombiesFell(GameObjectManager& gom, GameData& data)
{
	for (int i = 0; i < 4; i++)
	{
		if (gom.zombies.at(i).alive)
		{
			bool fallen = false;
			int j = 0;
			while (!fallen && j < data.numberOfHoles)
			{
				if (gom.zombies.at(i).x == gom.holes.at(j).x && gom.zombies.at(i).y == gom.holes.at(j).y)
				{
					fallen = true;
				}
				j++;
			}
			if (fallen)
			{
				gom.zombies.at(i).alive = false;
				data.zombiesLeft--;
			}
		}
	}
}

// Zombies hitting Spot listener+handler
// IN: GameObjectManager reference, GameData reference
// OUT:
// Precondition:
// Postcondition: Zombies that have caught Spot remove 1 life from data and reset to default coordinates
void zombiesHitSpot(GameObjectManager& gom, GameData& data)
{
	void resetZombieCoordinates(Item& zombieS);

	for (int i = 0; i < 4; i++)
	{
		if (gom.zombies.at(i).alive)
		{
			if (gom.zombies.at(i).x == gom.spot.x && gom.zombies.at(i).y == gom.spot.y)
			{
				data.lives--;
				resetZombieCoordinates(gom.zombies.at(i));
			}
		}
	}
}

// Zombies bumping listener+handler
// IN: Item vector representing Zombies
// OUT:
// Precondition:
// Postcondition: Zombies that have walked into each other are sent back to default coordinates
void zombiesBumped(vector<Item>& zombieStore)
{
	void resetZombieCoordinates(Item& zombieS);

	for (int i = 0; i < 4; i++)
	{
		// Only checks zombies that are alive
		if (zombieStore.at(i).alive)
		{
			// Loop to identify second zombies
			for (int j = 0; j < 4; j++)
			{
				// Does not check a zombie against itself
				if (i != j)
				{
					// Only compares with live zombies
					if (zombieStore.at(j).alive)
					{
						// If zombie i and zombie j are now on the same square, they are sent back home
						if ((zombieStore.at(i).x == zombieStore.at(j).x) && (zombieStore.at(i).y == zombieStore.at(j).y))
						{
							resetZombieCoordinates(zombieStore.at(i));
							resetZombieCoordinates(zombieStore.at(j));
						}
					}
				}
			}
		}
	}
}

// Zombies coordinate reset function
// IN: Item representing the zombie to reset coordinates of
// OUT:
// Precondition:
// Postcondition: Zombie given in parameter is reset to its default coordinates
void resetZombieCoordinates(Item& zombieStore)
{
	zombieStore.x = zombieStore.defaultX;
	zombieStore.y = zombieStore.defaultY;
}

// Zombies placement function
// IN: Array representing the grid, Vector representing all zombies
// OUT: Relevant zombies placed on relevant coordinates on the grid
// Precondition: None
// Postcondition: All zombies that are alive are placed on the grid at coordinates to match their own coordinate values
void placeZombies(char grid[][SIZEX], vector<Item> zombieStore)
{
	void placeItem(char g[][SIZEX], const Item& item);

	for (int i = 0; i < 4; i++)
	{
		if (zombieStore.at(i).alive)
		{
			placeItem(grid, zombieStore[i]);
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
	placeMultipleItems(gsm.grid, gom.holes);		// Place holes on the grid
	placeMultipleItems(gsm.grid, gom.pills);		// Place pills onto the grid
	placeZombies(gsm.grid, gom.zombies);			// place the zombies - last so always visible since they're a moving hazard
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
void updateGameData(const char g[][SIZEX], GameObjectManager& gom, GameData& gameData, const int key, string& mess)
{
	// move spot in required direction
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	void eatPill(GameObjectManager& gom, GameData& gameData);
	void moveZombies(const char grid[][SIZEX], GameObjectManager& gom, GameData& data);
	assert(isArrowKey(key));

	// reset message to blank
	mess = "                                         ";		// reset message to blank
	
	// Bool to tell whether or not to move zombies
	bool zombiesShouldMove = true;

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
		zombiesShouldMove = false;
		break;
	case PILL:
		gom.spot.x += dx;
		gom.spot.y += dy;
		eatPill(gom, gameData);
		break;
	case HOLE:
		gom.spot.x += dx;
		gom.spot.y += dy;
		gameData.lives--;
	}
	if (zombiesShouldMove)
	{
		moveZombies(g, gom, gameData);
	}

	// Moved this to the end so it isn't only called when Spot falls down a hole, to account for zombies draining lives - Matt
	if (gameData.lives <= 0) {
		//TODO STUB GameOverLose
	}
}

void eatPill(GameObjectManager& gom, GameData& gameData)
{
	for (int i = 0; i < gom.pills.size(); i++)
	{
		if (gom.pills.at(i).x == gom.spot.x && gom.pills.at(i).y == gom.spot.y)
		{
			gom.pills.at(i).visible = false;
			gameData.lives++;
			gameData.numberOfPillsLeft--;
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

// Records the playername and a new high score into a text file.
// IN: PlayerInfo struct containing playerName and previous Highscore, newHighScore integer
// OUT:
void saveUserData(const PlayerInfo& playerData, const int newHighScore) {
	ofstream fout;
	fout.open("saves/" + playerData.playerName + ".txt");
	if (fout.fail()) {
		//TODO Throw an error
	}
	else {
		fout << playerData.playerName << " " << newHighScore;
	}
}

void displayPlayerInformation(const PlayerInfo& playerData, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, x, y, "Player Name: " + playerData.playerName);
	showMessage(clDarkGrey, clYellow, x, y + 1, "High Score: " + tostring(playerData.highscore));
}

// Entry screen display
void displayEntryScreen(struct PlayerInfo& playerData) {
	void showGameTitle(const WORD backColour, const WORD textColour, int x, int y);
	void showGroupMembers(const WORD backColour, const WORD textColour, int x, int y);
	void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y);
	void displayNameRequest(const WORD backColour, const WORD textColour, int x, int y);
	string getUserName();
	void saveUserData(const PlayerInfo& playerData);

	showGameTitle(clDarkGrey, clYellow, 10, 6);
	showGroupMembers(clDarkGrey, clYellow, 10, 10);
	displayTimeAndDate(clDarkGrey, clYellow, 40, 1);
	displayNameRequest(clDarkGrey, clYellow, 10, 20);
	checkAndLoadUserSavedData(getUserName(), playerData);
}

void displayGameInformation(const GameData& gameData, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, x, y, "REMAINING LIVES: " + tostring(gameData.lives));
	showMessage(clDarkGrey, clYellow, x, y + 1, "ZOMBIES LEFT: " + tostring(gameData.zombiesLeft));
	showMessage(clDarkGrey, clYellow, x, y + 2, "NUMBER OF PILLS REMAINING: " + tostring(gameData.numberOfPillsLeft));

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

	displayPlayerInformation(playerData, 40, 15);
	displayGameInformation(gameData, 40, 20);

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

void gameOver(const PlayerInfo& playerData, const GameData& gameData) {
	void saveUserData(const PlayerInfo& playerData, const int newHighScore);
	if ((!gameData.hasCheated) && (gameData.lives > playerData.highscore)) {
		saveUserData(playerData, gameData.lives);
	}
}


// IN: background colour, text colour, x-position, y-position
// OUT:
// Precondition:
// Postcondition: Date and time will be displayed on consecutive lines in the specified colours.
void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y) {
	void getTime(struct tm &timeLocal);

	assert(true);
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