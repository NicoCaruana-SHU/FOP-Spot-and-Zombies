// ---------------------------------------------------------------------------
// Program: Task 1c – group assignment
// Author: Charlie Batten, Matt Bellamy, Nico Caruana
// ---------------------------------------------------------------------------

#pragma region include libraries
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
#include <time.h>			// For time display
#include <vector>			// For storing item data 
#include <fstream>			// For saving/loading files

// include our own libraries
#include "RandomUtils.h"	// for Seed, Random
#include "ConsoleUtils.h"	// for Clrscr, Gotoxy, etc.

using namespace std;
#pragma endregion

#pragma region define constants
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

struct CoOrd {
	int x, y;					// grid location
};

struct Item {
	CoOrd defaultLoc;			// Mainly used for zombies so they know where to go back to
	CoOrd currentLoc;			// Current position of object on grid.	
	char symbol;				// Symbol used to represent item on grid.
	bool active = true;			// Keeps track of whether item is active (pill eaten, zombie dead etc..)
};

// Pure data struct for saving player information at the end of the game.
struct PlayerInfo {
	string playerName;
	int highscore = -1;			// High score default value set to -1 as per the specification.	
};

struct GameSpaceManager {
	char grid[SIZEY][SIZEX];	// grid for display
	char maze[SIZEY][SIZEX];	// structure of the maze
};

struct GameObjectManager {
	vector<Item> holes, pills, zombies;
	Item spot;
};

struct GameData {					// Default game variable state, can be constructed differently for difficulties later.
	int lives = 3;					// Represents lives remaining in game. Set to 3 initially to match basic version.
	int numberOfHoles = 12;
	int maxPills = 8;
	int maxNumberOfZombies = 4;
	int numberOfPillsLeft = maxPills;
	int zombiesLeft = maxNumberOfZombies;
	bool hasCheated = false;		// Flag used to determine whether score is recorded at the end of the game.
	bool zombiesFrozen = false;		// Flag to determine whether zombies can move
	bool zombiesTerminated = false;
	bool gameEnded = false;
};

// TODO Add a difficulty struct with max number of items - Advanced tasks!
#pragma endregion

#pragma region run game
// ---------------------------------------------------------------------------
// ----- run game
// ---------------------------------------------------------------------------

int main() {
	// Function declarations (prototypes)
	void displayEntryScreen(PlayerInfo& playerData);
	void initialiseGame(GameSpaceManager& gsm, GameObjectManager& gom, GameData& gameData);
	void paintGame(const GameSpaceManager& gsm, const PlayerInfo& playerData, const GameData& gameData, const string& mess, const string& endGameMessage);
	bool wantsToQuit(const int key);
	int  getKeyPress();
	bool isArrowKey(const int k);
	void updateGameData(const char g[][SIZEX], GameObjectManager& gom, GameData& gameData, const int key, string& mess, string& endGameMessage);
	void commandCheck(int key, string& message, string& endGameMessage, GameSpaceManager& gsm, GameObjectManager& gom, GameData& gameData);
	void updateGrid(GameSpaceManager& gsm, GameObjectManager& gom);
	void gameOver(const PlayerInfo& playerData, const GameData& gameData);
	void endProgram();

	// local variable declarations
	GameSpaceManager gsm;
	GameObjectManager gom;
	PlayerInfo playerData;
	GameData gameData;
	string message("LET'S START...");								// current message to player
	string endGameMessage = "";

	// Function body
	Seed();															// seed the random number generator
	SetConsoleTitle("Spot and Zombies Game - FoP 2017-18");
	displayEntryScreen(playerData);
	initialiseGame(gsm, gom, gameData);								// initialise grid (incl. walls and spot)	
	paintGame(gsm, playerData, gameData, message, endGameMessage);					// display game info, modified grid and messages
	int key;														// current key selected by player
	do {
		key = toupper(getKeyPress()); 								// read in  selected key: arrow or letter command
		if (isArrowKey(key)) {
			updateGameData(gsm.grid, gom, gameData, key, message, endGameMessage);	// move spot in that direction
		}
		else {
			commandCheck(key, message, endGameMessage, gsm, gom, gameData);
		}
		updateGrid(gsm, gom);										// Re-Update grid to apply changes
		paintGame(gsm, playerData, gameData, message, endGameMessage);				// display game info, modified grid and messages
	} while ((!wantsToQuit(key)) && (!gameData.gameEnded));			// while user does not want to quit
	gameOver(playerData, gameData);									// HACK Save highscore data on player quit, if not cheated. Doesn't make sense in game terms really... Allows quitting early to manipulate highscore.. but spec does this, so its in for now.
	endProgram();													// display final message
	return 0;
}
#pragma endregion

#pragma region initialise game state
// ---------------------------------------------------------------------------
// ----- initialise game state
// ---------------------------------------------------------------------------

// Set up initial game grid and place items at their respective positions
// IN: Structs representing GameSpaceManager, GameObjectManager and GameData
// OUT:
// Precondition: GSM, GOM and GameData exist
// Postcondition: Grid is set up with the maze layout and all objects placed
void initialiseGame(GameSpaceManager& gsm, GameObjectManager& gom, GameData& gameData) {// initialise grid and place spot in middle
	// Function declarations (prototypes)
	void setInitialMazeStructure(char maze[][SIZEX]);
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void setItemInitialCoordinates(const char itemSymbol, Item& item, char[][SIZEX]);
	void updateGrid(GameSpaceManager& gsm, GameObjectManager& gom);
	void setMultipleItems(const char itemSymbol, int maxNumOfItems, vector<Item>& itemStore, char grid[][SIZEX]);
	void setZombies(char grid[][SIZEX], vector<Item>& zombies, GameData data);

	assert(true);

	// Function body
	setInitialMazeStructure(gsm.maze);										// initialise maze
	setMaze(gsm.grid, gsm.maze);											// Create first empty game frame
	setZombies(gsm.grid, gom.zombies, gameData);							// Place zombies first so that nothing spawns over the corners.
	setMultipleItems(HOLE, gameData.numberOfHoles, gom.holes, gsm.grid);	// Place the holes second
	setMultipleItems(PILL, gameData.maxPills, gom.pills, gsm.grid);			// Place the pills
	setItemInitialCoordinates(SPOT, gom.spot, gsm.grid);					// Finally place Spot
}

// Generate random co-ordinates within the grid bounds and place object there.
// IN: Char to use as the symbol to represent the item ingame, the item struct, the game grid array.
// OUT:
// Precondition: SIZEX and SIZEY must be greater than 2.
// Postcondition: Item is placed on grid in a random position, but not on the edge.
void setItemInitialCoordinates(const char itemSymbol, Item& item, char grid[][SIZEX]) {
	// Function declarations (prototypes)
	void placeItem(char g[][SIZEX], const Item& item);

	assert((SIZEX > 2) && (SIZEY > 2));

	// Function body
	do {										// NICO - Replaced the constant recalling itself with a do-while loop - more efficient!
		item.currentLoc.x = Random(SIZEX - 2);
		item.currentLoc.y = Random(SIZEY - 2);
	} while (grid[item.currentLoc.y][item.currentLoc.x] != TUNNEL);
	item.symbol = itemSymbol;
	placeItem(grid, item);
}

// Generate a vector of items and assign them random co-ordinates within the grid bounds and place them.
// IN: Char to use as the symbol to represent the item ingame, the maximum number of items to generate, vector to store the items, the game grid array.
// OUT:
// Precondition: maxNumOfItems must be greater than zero.
// Postcondition: Items are placed on grid in random positions, but not on the edge. Items stored in vector.
void setMultipleItems(const char itemSymbol, int maxNumOfItems, vector<Item>& itemStore, char grid[][SIZEX]) {
	// function declarations (prototypes)
	void setItemInitialCoordinates(const char itemSymbol, Item& item, char grid[][SIZEX]);
	void placeItem(char g[][SIZEX], const Item& item);

	assert(maxNumOfItems > 0);

	// Function body
	for (int i = maxNumOfItems; i > 0; --i) {
		Item i1;
		setItemInitialCoordinates(itemSymbol, i1, grid);
		itemStore.push_back(i1);
		placeItem(grid, i1);
	}
}

void setInitialMazeStructure(char maze[][SIZEX]) {
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

// Zombies default init function
// IN: Array representing the maze, vector representing (and referencing) all zombies
// OUT: Zombie coordinates set to defaults, zombies set to be alive
// Precondition: None
// Postcondition: All zombies placed in their initial corners and alive
void setZombies(char grid[][SIZEX], vector<Item>& zombieStore, GameData data) {
	// function declarations (prototypes)
	void resetZombieCoordinates(Item& zombieStore);
	void placeItem(char g[][SIZEX], const Item& item);

	assert(true);

	// Local variables
	int xDef[4] = { 1, 1, SIZEX - 2, SIZEX - 2 }; // Position defaults x co-ord
	int yDef[4] = { 1, SIZEY - 2, SIZEY - 2, 1 }; // Position defaults y co-ord

	// Function body
	for (int i = 0; i < 4; i++) {
		Item z1;
		z1.defaultLoc.x = xDef[i];
		z1.defaultLoc.y = yDef[i];
		z1.symbol = ZOMBIE;
		resetZombieCoordinates(z1);		
		zombieStore.push_back(z1);
		placeItem(grid, z1);
	}
}

// Zombies spawning function
// IN: Array representing the maze, vector representing (and referencing) all zombies
// OUT: Zombie coordinates set to defaults, zombies set to be alive
// Precondition: None
// Postcondition: All zombies placed in their initial corners and alive
void spawnZombies(char grid[][SIZEX], vector<Item>& zombieStore, GameData& data) {
	// Function declarations (prototypes)
	void resetZombieCoordinates(Item& zombieStore);

	assert(true);

	// Function body
	for (int i = 0; i < 4; i++) {
		resetZombieCoordinates(zombieStore.at(i));
	}
	data.zombiesLeft = 4;
}
#pragma endregion

#pragma region update grid state
// ---------------------------------------------------------------------------
// ----- update grid state
// ---------------------------------------------------------------------------

// Update grid configuration after each move
// IN: GameSpaceManager and GameObjectManager
// OUT:
// Precondition: None
// Postcondition: All objects placed in their respective positions after movement.
void updateGrid(GameSpaceManager& gsm, GameObjectManager& gom) {
	// Function declarations (prototypes)
	void setMaze(char g[][SIZEX], const char b[][SIZEX]);
	void placeMultipleItems(char g[][SIZEX], const vector<Item>& itemStore);
	void placeItem(char g[][SIZEX], const Item& spot);

	assert(true);

	// Function body
	// Not sent as complete GSM/GOM, to keep constant array restrictions.
	setMaze(gsm.grid, gsm.maze);					// reset the empty maze configuration into grid 
	placeMultipleItems(gsm.grid, gom.holes);		// Place holes on the grid
	placeMultipleItems(gsm.grid, gom.pills);		// Place pills onto the grid
	placeMultipleItems(gsm.grid, gom.zombies);		// place the zombies - last so always visible since they're a moving hazard
	placeItem(gsm.grid, gom.spot);					// set spot in grid
}

// Reset the empty/fixed maze configuration into grid
// IN: Grid array, maze layout array
// OUT:
// Precondition: None
// Postcondition: Maze layout copied into grid array.
void setMaze(char grid[][SIZEX], const char maze[][SIZEX]) {
	assert(true);

	// Function body
	for (int row(0); row < SIZEY; ++row)
		for (int col(0); col < SIZEX; ++col)
			grid[row][col] = maze[row][col];
}

// Single item placement function
// IN: Array representing the grid, item to place.
// OUT:
// Precondition: None
// Postcondition: Item will be placed on grid at its x, y co-ordinates.
void placeItem(char g[][SIZEX], const Item& item) {
	assert(true);

	// Function body
	if (item.active) { // check for visible flag before rendering
		g[item.currentLoc.y][item.currentLoc.x] = item.symbol;
	}
}

// Multiple item placement function
// IN: Array representing the grid, Vector holding the items.
// OUT:
// Precondition: None
// Postcondition: All items will be placed on grid
void placeMultipleItems(char g[][SIZEX], const vector<Item>& itemStore) {
	// Function declarations (prototypes)
	void placeItem(char g[][SIZEX], const Item& item);

	assert(true);

	// Function body
	for (int i(itemStore.size() - 1); i >= 0; --i) { // place items until max is reached
		placeItem(g, itemStore.at(i));
	}
}

#pragma endregion

#pragma region move items on the grid
// ---------------------------------------------------------------------------
// ----- move items on the grid
// ---------------------------------------------------------------------------
void updateGameData(const char g[][SIZEX], GameObjectManager& gom, GameData& gameData, const int key, string& mess, string& endGameMessage) {
	// Function declarations (prototypes)
	bool isArrowKey(const int k);
	void setKeyDirection(int k, int& dx, int& dy);
	void eatPill(GameObjectManager& gom, GameData& gameData);
	void moveZombies(const char grid[][SIZEX], GameObjectManager& gom, GameData& data, string& endGameMessage);
	void gameLost(GameData& gameData, string& endGameMessage);
	void gameWon(GameData& gameData, string& endGameMessage);

	assert(isArrowKey(key));

	// Local variables
	mess = "                                         ";		// reset message to blank	
	bool playerMoved = true;								// Bool to confirm whether or not player moved, so zombies can be moved.	
	int dx(0), dy(0);										// X and Y values for applying movement to objects.

	// Function body
	setKeyDirection(key, dx, dy);							// calculate direction of movement for given key
	switch (g[gom.spot.currentLoc.y + dy][gom.spot.currentLoc.x + dx]) {			// check new target position in grid and update game data (incl. spot coordinates) if move is possible
		// ...depending on what's on the target position in grid...
	case TUNNEL:		// can move
		gom.spot.currentLoc.y += dy;	// go in that Y direction
		gom.spot.currentLoc.x += dx;	// go in that X direction
		break;
	case WALL:  		// hit a wall and stay there
		mess = "CANNOT GO THERE!";
		playerMoved = false;
		break;
	case ZOMBIE: // Allow movement onto zombie square, but must also check for pill underlying too.
	case PILL:
		gom.spot.currentLoc.x += dx;
		gom.spot.currentLoc.y += dy;
		eatPill(gom, gameData);
		if ((gameData.numberOfPillsLeft == 0) && (gameData.zombiesLeft == 0)) { // If there are no zombies alive when there are no pills remaining, game is won!;
			gameWon(gameData, endGameMessage);
		}
		break;
	case HOLE:
		gom.spot.currentLoc.x += dx;
		gom.spot.currentLoc.y += dy;
		gameData.lives--;
		if (gameData.lives <= 0) { // Check to see if player has any lives remaining after they enter a hole.
			gameLost(gameData, endGameMessage);
		}
	}
	if ((!gameData.gameEnded) && (!gameData.zombiesFrozen) && (playerMoved)) {
		moveZombies(g, gom, gameData, endGameMessage);
	}

}

// Zombies movement function
// IN: Array representing the maze, GameObjectManager reference, GameData reference
// OUT:
// Precondition:
// Postcondition: Zombie(s) moved towards Spot, and determined to have fallen, bumped or hit Spot and taken necessary action
void moveZombies(const char grid[][SIZEX], GameObjectManager& gom, GameData& data, string& endGameMessage) {
	// Function declarations (prototypes)
	void zombiesHitSpot(GameObjectManager& go, GameData& gd, string& endGameMessage);
	void zombiesBumped(vector<Item>& zombieStore);
	void zombiesFell(GameObjectManager& go, GameData& gd, string& endGameMessage);

	assert(true);

	// Function body
	for (int i = 0; i < 4; i++) {
		if (gom.zombies.at(i).active) { // Only moves if alive			
			if (gom.zombies.at(i).currentLoc.x != gom.spot.currentLoc.x || gom.zombies.at(i).currentLoc.y != gom.spot.currentLoc.y) { // Only does all checks if coordinates are not equal
																						  // X MOVEMENT				
				if (gom.zombies.at(i).currentLoc.x > gom.spot.currentLoc.x) { // Move towards Spot if Spot X is to lower
														// 1/4 chance to not move
														//if ((rand() % 4) != 1) {
					gom.zombies.at(i).currentLoc.x--;
					//}
				}
				else if (gom.zombies.at(i).currentLoc.x < gom.spot.currentLoc.x) { // Move towards Spot if Spot X is higher
															 // 1/4 chance to not move
															 //if ((rand() % 4) != 1) {
					gom.zombies.at(i).currentLoc.x++;
					//}
				}
				// Y MOVEMENT

				if (gom.zombies.at(i).currentLoc.y > gom.spot.currentLoc.y) { // Move towards Spot if Spot Y is lower
														// 1/4 chance to not move
														//if ((rand() % 4) != 1) {
					gom.zombies.at(i).currentLoc.y--;
					//}
				}
				else if (gom.zombies.at(i).currentLoc.y < gom.spot.currentLoc.y) { // Move towards Spot if Spot Y is higher
															 // 1/4 chance to not move
															 //if ((rand() % 4) != 1) {
					gom.zombies.at(i).currentLoc.y++;
					//}
				}
			}
		}
		zombiesFell(gom, data, endGameMessage);					//Checks if zombies have fallen into holes		
		zombiesHitSpot(gom, data, endGameMessage);				// Checks if zombies have run into spot and causes damage+resets zombie if applicable		
		zombiesBumped(gom.zombies);				// Checks to see if zombies have bumped into one another through movement and resets their coordinates if necessary
	}
}

// Zombies coordinate reset function
// IN: Item representing the zombie to reset coordinates of
// OUT:
// Precondition:
// Postcondition: Zombie given in parameter is reset to its default coordinates and set active.
void resetZombieCoordinates(Item& zombieStore) {
	assert(true);

	// Function body
	zombieStore.currentLoc = zombieStore.defaultLoc;
	zombieStore.active = true;
}
#pragma endregion

#pragma region collision behaviour
// ---------------------------------------------------------------------------
// ----- collision behaviour
// ---------------------------------------------------------------------------

// Consume pill that spot has collided with.
// IN: GameObjectManager reference, GameData reference
// OUT:
// Precondition:
// Postcondition: Encountered pill will be removed from the game rendering, player will be granted a life, number of pills on the ui will be decremented.
void eatPill(GameObjectManager& gom, GameData& gameData) {
	assert(true);

	// Function body
	for (int i = 0; i < gom.pills.size(); i++) {
		if (gom.pills.at(i).currentLoc.x == gom.spot.currentLoc.x && gom.pills.at(i).currentLoc.y == gom.spot.currentLoc.y) {
			gom.pills.at(i).active = false;
			gameData.lives++;
			gameData.numberOfPillsLeft--;
		}
	}
}

// Zombies falling listener+handler
// IN: GameObjectManager reference, GameData reference
// OUT:
// Precondition:
// Postcondition: Zombies that have walked onto holes are killed and removed from the grid
void zombiesFell(GameObjectManager& gom, GameData& gameData, string& gameEndMessage) {
	// Function declarations (prototypes)
	void gameWon(GameData& gameData, string& gameEndMessage);

	assert(true);

	// Function body
	for (int i = 0; i < 4; i++) {
		if (gom.zombies.at(i).active) {
			bool fallen = false;
			int j = 0;
			while (!fallen && j < gameData.numberOfHoles) {
				if (gom.zombies.at(i).currentLoc.x == gom.holes.at(j).currentLoc.x && gom.zombies.at(i).currentLoc.y == gom.holes.at(j).currentLoc.y) {
					fallen = true;
				}
				j++;
			}
			if (fallen) {
				gom.zombies.at(i).active = false;
				gameData.zombiesLeft--;
				if ((gameData.zombiesLeft == 0) && (gameData.numberOfPillsLeft == 0)) { // If there are no pills remaining when the last zombie falls into a hole, game is won!;
					gameWon(gameData, gameEndMessage);
				}
			}
		}
	}
}

// Zombies hitting Spot listener+handler
// IN: GameObjectManager reference, GameData reference
// OUT:
// Precondition:
// Postcondition: Zombies that have caught Spot remove 1 life from data and reset to default coordinates
void zombiesHitSpot(GameObjectManager& gom, GameData& gameData, string& endGameMessage) {
	// Function declarations (prototypes)
	void resetZombieCoordinates(Item& zombieS);
	void gameLost(GameData& gameData, string& endGameMessage);

	assert(true);

	// Function body
	for (int i = 0; i < 4; i++) {
		if (gom.zombies.at(i).active) {
			if (gom.zombies.at(i).currentLoc.x == gom.spot.currentLoc.x && gom.zombies.at(i).currentLoc.y == gom.spot.currentLoc.y) {
				gameData.lives--;
				resetZombieCoordinates(gom.zombies.at(i));
				if (gameData.lives <= 0) { // Check to see if player has any lives remaining after spot gets hit by zombie.
					gameLost(gameData, endGameMessage);
				}
			}
		}
	}
}

// Zombies bumping listener+handler
// IN: Item vector representing Zombies
// OUT:
// Precondition:
// Postcondition: Zombies that have walked into each other are sent back to default coordinates
void zombiesBumped(vector<Item>& zombieStore) {
	// Function declarations (prototypes)
	void resetZombieCoordinates(Item& zombieS);

	assert(true);

	// Function body
	for (int i = 0; i < 4; i++) {
		if (zombieStore.at(i).active) {									// Only checks zombies that are alive			
			for (int j = 0; j < 4; j++) {								// Loop to identify second zombies				
				if (i != j) {											// Does not check a zombie against itself					
					if (zombieStore.at(j).active) {						// Only compares with live zombies
																		// If zombie i and zombie j are now on the same square, they are sent back home
						if ((zombieStore.at(i).currentLoc.x == zombieStore.at(j).currentLoc.x) && (zombieStore.at(i).currentLoc.y == zombieStore.at(j).currentLoc.y)) {
							resetZombieCoordinates(zombieStore.at(i));
							resetZombieCoordinates(zombieStore.at(j));
						}
					}
				}
			}
		}
	}
}
#pragma endregion

#pragma region process inputs
// ---------------------------------------------------------------------------
// ----- process inputs
// ---------------------------------------------------------------------------
void setKeyDirection(const int key, int& dx, int& dy) { // calculate direction indicated by key
	// Function declarations (prototypes)
	bool isArrowKey(const int k);

	assert(isArrowKey(key));

	// Function body
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

int getKeyPress() {
	// get key or command (in uppercase) selected by user
	// KEEP THIS FUNCTION AS GIVEN
	int keyPressed;
	keyPressed = _getch();			// read in the selected arrow key or command letter
	while (keyPressed == 224) 		// ignore symbol following cursor key
		keyPressed = _getch();
	return keyPressed;		// return it in uppercase 
}

bool isArrowKey(const int key) {
	// check if the key pressed is an arrow key (also accept 'K', 'M', 'H' and 'P')
	// TODO Why are we being told to accept KMHP here, what are they for? Can't see anything in spec about them.
	return (key == LEFT) || (key == RIGHT) || (key == UP) || (key == DOWN);
}

bool wantsToQuit(const int key) {
	// check if the user wants to quit (when key is 'Q' or 'q')
	return key == QUIT;
}

// Get a valid string from user input. 
// IN: String reference to store the user information in
// OUT:
// Precondition:
// Postcondition: String will be filled with a valid username, or empty string if invalid characters entered.
void getUserName(string& name) {
	assert(true);

	// Local variables
	int maxChars = 20;										// Maximum length of permitted name
	bool valid = true;										// Flag for name validity, assumed true initially

	// Function body
	cin >> setw(maxChars) >> name;							// Get input from user, using cin to avoid whitespace characters, setting max length with setw.
	int i = 0;
	while (valid && (i < name.length())) {					// Loop while the name is still considered valid, and end of string hasn't been reached.
		if (!(name.at(i) >= 'A') && (name.at(i) <= 'z')) {	// Check if character is not a valid letter a-z including capitals.
			valid = false;									// Flag name invalid if invalid character found
		}
		++i;												// Increment counter to continue iterating through string.
	}
	if (!valid) {											// If an invalid character has been found, reset the string to empty. 
		name = "";
	}
}

void commandCheck(int key, string& message, string& endGameMessage, GameSpaceManager& gsm, GameObjectManager& gom, GameData& gameData) {
	void freezeCheat(string& message, GameData& gameData);
	void exterminateCheat(string& message, string& endGameMessage, GameData& gameData, GameObjectManager& gom, GameSpaceManager& gsm);
	void eatPillCheat(string& message, string& endGameMessage, GameData& gameData, vector<Item>& pillList, GameSpaceManager& gsm);

	switch (key) {
	case FREEZE:
		freezeCheat(message, gameData);
		break;
	case EXTERMINATE:				
		exterminateCheat(message,endGameMessage, gameData, gom, gsm);
		break;
	case EAT:
		eatPillCheat(message, endGameMessage, gameData, gom.pills, gsm);
		break;
	case QUIT:
		message = "GAME STOPPED";
		break;
	default:
		message = "INVALID KEY!";					// set 'Invalid key' message if keypress not recognised as valid input.
	}
}
#pragma endregion

#pragma region cheat actions
// ---------------------------------------------------------------------------
// ----- cheat actions
// ---------------------------------------------------------------------------

void deactivateAll(vector<Item>& list) {
	for (int i = 0; i < list.size(); i++)
	{
		list.at(i).active = false;
	}
}

void freezeCheat(string& message, GameData& gameData) {
	gameData.hasCheated = true;							// Flag the gamestate as having cheated to prevent high score saving later.
	gameData.zombiesFrozen = !gameData.zombiesFrozen;	// Toggle value on each keypress. Boolean later checked to allow zombies to move.
	if (gameData.zombiesFrozen) {
		message = "CHEAT: ZOMBIES FROZEN!";
	}
	else {
		message = "CHEAT: ZOMBIES UNFROZEN!";
	}
}

void exterminateCheat(string& message, string& endGameMessage, GameData& gameData, GameObjectManager& gom, GameSpaceManager& gsm) {
	void deactivateAll(vector<Item>& list);
	void gameWon(GameData& gameData, string& endGameMessage);

	gameData.hasCheated = true;							// Flag the gamestate as having cheated to prevent high score saving later.
	if (gameData.zombiesTerminated) {					// If used the cheat to kill last time, respawns zombies					
		spawnZombies(gsm.grid, gom.zombies, gameData);	// NOTE: spawnZombies was changed to use existing data for purposes such as this; replaced with setZombies
		gameData.zombiesTerminated = false;
		message = "CHEAT: ZOMBIES SPAWNED!";
	}
	else {												// Exterminate all zombies on screen
		deactivateAll(gom.zombies);
		gameData.zombiesLeft = 0;
		gameData.zombiesTerminated = true;
		message = "CHEAT: ZOMBIES KILLED!";
		if (gameData.numberOfPillsLeft == 0)
		{
			gameWon(gameData, endGameMessage);
		}
	}
}

void eatPillCheat(string& message, string& endGameMessage, GameData& gameData, vector<Item>& pillList, GameSpaceManager& gsm) {
	void deactivateAll(vector<Item>& list);
	void gameWon(GameData& gameData, string& endGameMessage);

	gameData.hasCheated = true;							// Flag the gamestate as having cheated to prevent high score saving later.
	deactivateAll(pillList);							// Set all pills visible flag to false so that they no longer get rendered in the game display
	gameData.numberOfPillsLeft = 0;						// Player is not rewarded for pills eaten this way, so set pills to zero without changing lives left.
	message = "CHEAT: EAT!";
	if (gameData.zombiesLeft == 0)
	{
		gameWon(gameData, endGameMessage);
	}
}
#pragma endregion

#pragma region display info on screen
// ---------------------------------------------------------------------------
// ----- display info on screen
// ---------------------------------------------------------------------------

string tostring(int x) {
	// convert an integer to a string
	std::ostringstream os;
	os << x;
	return os.str();
}

string tostring(char x) {
	// convert a char to a string
	std::ostringstream os;
	os << x;
	return os.str();
}

void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message) {
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
	showMessage(backColour, textColour, x, y + 1, "Charlie Batten  27012619");
	showMessage(backColour, textColour, x, y + 2, "Matt Bellamy    26012012");
	showMessage(backColour, textColour, x, y + 3, "Nico Caruana    27022205");
}

// Display a request for the user to enter username from a set starting point in a defined colour.
// User Entry text is then changed to a red colour.
void displayNameRequest(const WORD backColour, const WORD textColour, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(backColour, textColour, x, y, "Enter your name: ");
	SelectTextColour(clGreen);
}

void displayPlayerInformation(const PlayerInfo& playerData, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, x, y, "Player Name: " + playerData.playerName);
	showMessage(clDarkGrey, clYellow, x, y + 1, "High Score: " + tostring(playerData.highscore));
}

void displayGameInformation(const GameData& gameData, int x, int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clDarkGrey, clYellow, x, y, "REMAINING LIVES: " + tostring(gameData.lives));
	showMessage(clDarkGrey, clYellow, x, y + 1, "ZOMBIES LEFT: " + tostring(gameData.zombiesLeft));
	showMessage(clDarkGrey, clYellow, x, y + 2, "NUMBER OF PILLS REMAINING: " + tostring(gameData.numberOfPillsLeft));

}

void displayControlsMenu(const WORD firstColour, const WORD secondColour, const int x, const int y) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(firstColour, secondColour, x, y, "TO MOVE USE KEYBOARD ARROWS ");
	showMessage(firstColour, secondColour, x, y + 1, "TO QUIT ENTER 'Q'           ");
	showMessage(firstColour, secondColour, x, y + 3, "CHEATS                        ");
	showMessage(firstColour, secondColour, x, y + 4, "TO FREEZE ZOMBIES PRESS '" + tostring(FREEZE) + "'   ");
	showMessage(firstColour, secondColour, x, y + 5, "TO KILL ALL ZOMBIES PRESS '" + tostring(EXTERMINATE) + "' ");
	showMessage(firstColour, secondColour, x, y + 6, "TO EAT ALL PILLS PRESS '" + tostring(EAT) + "'    ");
}

// Entry screen display
void displayEntryScreen(PlayerInfo& playerData) {
	void showGameTitle(const WORD backColour, const WORD textColour, int x, int y);
	void showGroupMembers(const WORD backColour, const WORD textColour, int x, int y);
	void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y);
	void displayNameRequest(const WORD backColour, const WORD textColour, int x, int y);
	void getUserName(string& name);
	void checkAndLoadUserSavedData(const string& userName, PlayerInfo& playerData);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	assert(true);

	// Local variables
	string name = "";

	// Function body
	showGameTitle(clDarkGrey, clYellow, 10, 6);
	showGroupMembers(clDarkGrey, clYellow, 10, 10);
	displayTimeAndDate(clDarkGrey, clYellow, 40, 1);
	displayNameRequest(clDarkGrey, clYellow, 10, 20);
	getUserName(name);
	while (name == "") {
		SelectBackColour(clBlack); // Doing a full screen refresh here, setting the background colour to black
		Clrscr();
		showGameTitle(clDarkGrey, clYellow, 10, 6);
		showGroupMembers(clDarkGrey, clYellow, 10, 10);
		displayTimeAndDate(clDarkGrey, clYellow, 40, 1);
		displayNameRequest(clDarkGrey, clYellow, 10, 20);
		showMessage(clDarkGrey, clRed, 10, 22, "Invalid entry, name must contain letters only!");
		displayNameRequest(clDarkGrey, clYellow, 10, 20);
		getUserName(name);
	}
	checkAndLoadUserSavedData(name, playerData);
}

void displayEndGameMessages(const string& endGameMessage) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	showMessage(clGrey, clRed, 40, 25, endGameMessage);
}

void paintGame(const GameSpaceManager& gsm, const PlayerInfo& playerData, const GameData& gameData, const string& mess, const string& endGameMessage)
{
	// Function declarations (prototypes)
	void displayPlayerInformation(const PlayerInfo& playerData, int x, int y);
	void displayGameInformation(const GameData& gameData, int x, int y);
	void displayEndGameMessages(const string& endGameMessage);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// display game title, messages, maze, spot and other items on screen
	string tostring(char x);
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	void paintGrid(const GameSpaceManager& gsm);
	void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y);

	// Function body

	SelectBackColour(clGrey);
	Clrscr();														// Using included libraries, clears the game screen - sets it all grey.

	// display game title
	showMessage(clDarkGreen, clGreen, 0, 0, "___SPOT AND ZOMBIES GAME___");
	showMessage(clYellow, clBlue, 40, 0, "FoP Task 1c: February 2018");
	displayTimeAndDate(clYellow, clBlue, 40, 1);

	// print auxiliary messages if any
	showMessage(clBlack, clYellow, 40, 6, mess);	// display current message

	displayControlsMenu(clRed, clGreen, 40, 8);		// display menu options available
	displayPlayerInformation(playerData, 40, 16);
	displayGameInformation(gameData, 40, 19);
	if (gameData.gameEnded)							// If the game has finished, display the win/loss messages
	{
		displayEndGameMessages(endGameMessage);
	}
	paintGrid(gsm);									// display grid contents
}

void paintGrid(const GameSpaceManager& gsm) {
	// display grid content on screen
	SelectBackColour(clBlack);
	SelectTextColour(clWhite);
	Gotoxy(0, 2);
	for (int row(0); row < SIZEY; ++row) {
		for (int col(0); col < SIZEX; ++col) {
			switch (gsm.grid[row][col]) {
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
#pragma endregion

#pragma region game end
// ---------------------------------------------------------------------------
// ----- game end
// ---------------------------------------------------------------------------

void endProgram() {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);
	showMessage(clRed, clYellow, 40, 27, "");
	system("pause");	// hold output screen until a keyboard key is hit
}

void gameOver(const PlayerInfo& playerData, const GameData& gameData) {
	void saveUserData(const PlayerInfo& playerData, const int newHighScore);

	if ((!gameData.hasCheated) && (gameData.lives > playerData.highscore)) {
		saveUserData(playerData, gameData.lives);
		showMessage(clGrey, clRed, 40, 26, "CONGRATULATIONS, A NEW HIGH SCORE!");
	}
}

void gameLost(GameData& gameData, string& endGameMessage) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	// Function body
	gameData.gameEnded = true;
	endGameMessage = "NO MORE LIVES, YOU LOSE!";
}

void gameWon(GameData& gameData, string& endGameMessage) {
	void showMessage(const WORD backColour, const WORD textColour, int x, int y, const string& message);

	// Function body
	gameData.gameEnded = true;
	endGameMessage = "YOU WON WITH " + tostring(gameData.lives) + " LIVES LEFT";
}
#pragma endregion

#pragma region data management
// ---------------------------------------------------------------------------
// ----- data management
// ---------------------------------------------------------------------------

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
	if (!fout.fail()) {
		fout << playerData.playerName << " " << newHighScore;
	}
	else {
		//HACK Throw an error- Simple cout for now, ideas for a better way appreciated.
		cout << "Error saving data! Highscore not recorded!";
	}
}
#pragma endregion

#pragma region time functions
// ---------------------------------------------------------------------------
// ----- time functions
// ---------------------------------------------------------------------------

// IN: background colour, text colour, x-position, y-position
// OUT:
// Precondition:
// Postcondition: Date and time will be displayed on consecutive lines in the specified colours.
void displayTimeAndDate(const WORD firstColour, const WORD secondColour, const int x, const int y) {
	void getTime(tm &timeLocal);

	struct tm timeLocal; // create time structure called timeLocal
	getTime(timeLocal); //populate structure with values

	assert(true);
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

// Get raw time data from system and convert it to usable format.
// IN: Empty struct to hold converted time data
// OUT:
// Precondition: None
// Postcondition: Supplied struct will be filled with converted time data.
void getTime(tm &timeLocal) {
	assert(true);

	// Function body
	time_t rawTime; // create time_t struct variable to store raw time data
	time(&rawTime); // get raw time data (in seconds from Jan 1 1970) and insert into rawtime variable.
	localtime_s(&timeLocal, &rawTime); //convert raw time into usable time structure and insert into struct timeLocal
}
#pragma endregion
