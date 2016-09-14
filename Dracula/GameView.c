// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h" //... if you decide to use the Map ADT
     
typedef struct _playerInfo {
  int playerHealth;
  LocationID playerLocation;    
  LocationID playerTrail[5];
} playerInfo;

struct gameView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int gameScore;
    Round roundNumber;
    char *pastPlays;
    PlayerMessage* messages;
    Map map;
    PlayerID currentPlayer;
    playerInfo *players[NUM_PLAYERS];
};
     

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[]) //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->gameScore = GAME_START_SCORE;
    gameView->roundNumber = 0;
    gameView->pastPlays = pastPlays;
    gameView->messages = messages;
    gameView->map = newMap();
    gameView->currentPlayer = PLAYER_LORD_GODALMING;
    int x;
    for (x = PLAYER_LORD_GODALMING; x <= PLAYER_MINA_HARKER ; x++) {
        gameView->players[x]->playerHealth = GAME_START_HUNTER_LIFE_POINTS;
        gameView->players[x]->playerLocation = ST_JOSEPH_AND_ST_MARYS;
        int y;
        for (y = 0; y < TRAIL_SIZE ; y++) {
            gameView->players[x]->playerTrail[y] = UNKNOWN_LOCATION;
        }
    }
    gameView->players[PLAYER_DRACULA]->playerHealth = GAME_START_BLOOD_POINTS;
    gameView->players[PLAYER_DRACULA]->playerLocation = CASTLE_DRACULA;
    int y;
    for (y = 0; y < TRAIL_SIZE ; y++) {
        gameView->players[PLAYER_DRACULA]->playerTrail[y] = UNKNOWN_LOCATION;
    }
    return gameView;
}     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{ //COMPLETE THIS IMPLEMENTATION
    
    // Check game data exists
    assert(toBeDeleted != NULL);         
    // Free general data
    free(toBeDeleted->gameScore);
    free(toBeDeleted->roundNumber);
    free(toBeDeleted->pastPlays);
    free(toBeDeleted->messages;
    free(toBeDeleted->map);
    free(toBeDeleted->currentPlayer);
    // Free hunter data
    int x;
    for (x = PLAYER_LORD_GODALMING; x <= PLAYER_MINA_HARKER; x++) {
        free(toBeDeleted->players[x]->playerHealth);
        free(toBeDeleted->players[x]->playerLocation);
        int y;
        for (y = 0; y < TRAIL_SIZE; y++) {
            free(toBeDeleted->players[x]->playerTrail[y]);
        }       
    }
    // Free dracula data
    free(toBeDeleted->players[PLAYER_DRACULA]->playerHealth);
    free(toBeDeleted->players[PLAYER_DRACULA]->playerLocation);
    int y;
    for (y = 0; y < TRAIL_SIZE; y++) {
        free(toBeDeleted->players[PLAYER_DRACULA]->playerTrail[y]);
    }
    // Free struct ptr + set to NULL + check
    free(toBeDeleted);
    toBeDeleted = NULL;
    assert(toBeDeleted == NULL);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int x;
    currentView->
    return 0;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

// Get the current score
int getScore(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    currentView->
    return 0;
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int size = *numLocations;
    LocationID* array = (LocationID *)malloc(sizeof(LocationID)*size);
    array[0] = from;
    // int x=1;
    // VList curr = currentView->map->connections[x];
    // while ( x < size || curr!= NULL) {
    //   array[x++] = curr->v;
    //   int unrepeated = 1;
    //   while (unrepeated) {
    //     curr = curr->next;
    //     int y;
    //     for (y=0 ; y <= x; y++) {
    //       if (curr->v == array[y]) unrepeated = 0;
    //     }
    //   }
    // }

    return array;
}
