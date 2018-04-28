Data is stored in the GameData struct
Data is reset in game initialisation
Data is updated in the paintGame function
Replay is displayed in the paintGame function
Replay is terminated in the paintGame function

Known issue: During the replay, if the user provides input it will be taken in and be kept in the buffer.
             When control is passed back to the player, all input in the buffer will come out at once.
             Attempted flushing, such as with cin.ignore, has proven fruitless and even broken the game's input
