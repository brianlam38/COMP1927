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
    // Free game struct data
    free(toBeDeleted->pastPlays);
    free(toBeDeleted->messages);
    free(toBeDeleted->map);
    // Free player struct
    int x;
    for (x = PLAYER_LORD_GODALMING; x <= PLAYER_DRACULA; x++) {
        free(toBeDeleted->players[x]);
        toBeDeleted->players[x] = NULL;
        assert(toBeDeleted->players[x] == NULL);
    }
    // Free struct ptr + set to NULL + check
    free(toBeDeleted);
    toBeDeleted = NULL;
    assert(toBeDeleted == NULL);
}

// ################
// SETTER FUNCTIONS
// ################

// ################
// GETTER FUNCTIONS
// ################

// Get the current round
Round getRound(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    int round = currentView->roundNumber;
    assert(round >= 0);

    return round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    int player = currentView->currentPlayer;
    assert(player < NUM_PLAYERS);

    return player;
}

// Get the current score
int getScore(GameView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    int score = currentView->gameScore;
    assert(score <= GAME_START_SCORE);

    return score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    int health = currentView->players[currentView->currentPlayer]->playerHealth;
    if (currentView->currentPlayer == PLAYER_DRACULA) {
        assert(health <= 40 && health > 0);
    } else {
        assert(health <= 9);
    }
    return health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    return 0;
}

// #################
// GET HISTORIC DATA
// #################

void getHistory(GameView currentView, PlayerID player,
                 LocationID trail[TRAIL_SIZE]) {
    
}

// ###################
// MAP QUERY FUNCTIONS
// ###################

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


// ##################
// IN HOUSE FUNCTIONS
// ##################

// given a special abbreviation (2 char), return its ID number
static int specialToID(char *abbrev) {

    if (strcmp(abbrev, "C?") == 0) {
        return CITY_UNKNOWN;
    } else if (strcmp(abbrev,"S?") == 0) {
        return SEA_UNKNOWN;
    } else if (strcmp(abbrev,"HI") == 0) {
        return HIDE;
    } else if (strcmp(abbrev,"D1") == 0) {      // Investigate if D1 D2 D3 exists
        return DOUBLE_BACK_1;
    } else {
        return abbrevToID(abbrev);
    }
}

// PARSING CHECKLIST
    // char *pastPlays
// 1. Take in location ABBREV from trail -> Give AI number      DONE (given by default places.c?)
// 2. Take in special ABBREV (actions etc) -> Give AI number    IN PROGRESS (gameView.c)
// 3. Take in AI move -> return location ABBREV                 NOT STARTED
// 4. Take in AI action -> return special ABBREV                NOT STARTED
    // PlayerMessage messages[]
// 1. Take in AI message -> Store in messages                   NOT STARTED
// 2. Take messages from storage -> Translate for AI            NOT STARTED



