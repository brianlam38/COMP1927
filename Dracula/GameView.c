// Pairsing Function
    // Read [The Data]
// Play --> Move
// Encounters
    // Health
    // GameScore
    // Possibly others etc.
// Update Trails
// Maps
    // Write an interface function inside the gameView.c file
  // Every we need neighbours, call connected neighbours
    // Think about special cases, such as rail connections etc.

// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h" //... if you decide to use the Map ADT
#include "Places.h" // Using Places ADT
     
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

// #####################
// FUNCTION DECLARATIONS
// #####################

LocationID otherToID(char *abbrev);
     

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
// GET FUNCTIONS
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

    int numNearby = *numLocations;
    LocationID *connections = malloc(numNearby*sizeof(LocationID));
    int counter = 0;

    if (road) {
        LocationID nearby[71];
        int numNeighbours = 0;
        numNeighbours = NearbyCities(currentView->map, from, nearby, ROAD);
        int i = 0;

    while (counter < numNeighbours) {
            connections[counter] = nearby[i];
            i++; counter++;
        }
  }
    
    if (sea) {
        LocationID nearby[71];
        int numNeighbours = 0;
        numNeighbours = NearbyCities(currentView->map, from, nearby, ROAD);

    int i = 0;
        while (numNeighbours > 0) {
            if(!inArray(connections,nearby[i], counter)) {
                connections[counter] = nearby[i];
                counter++;
        }
            numNeighbours--; i++;
        }
    }

    if (rail) {
        LocationID stations[71];
        int numStations = getStations(currentView->map, from, stations, player, round);
        if (numStations != 0) {
            while(numStations > 0) {
                if (!inArray(connections,stations[numStations],counter)) {
                    connections[counter] = stations[numStations];
                    counter++;
                }
                numStations--;
            }
        }
    }
    return connections;
}


int getStations(Map map,LocationID from, LocationID *stations, PlayerID player, Round round) {

    int stationsAllowed = (player+round)%4;
    if (player == PLAYER_DRACULA || stationsAllowed == 0) return 0;

    int counter = NearbyCities(map, from, stations, RAIL);
    if (stationsAllowed == 1) return counter;

    int numNearby = counter;

  int i;
    for(i = counter; i > 0; i--) {
        LocationID secondaryStations[71];
        int j = NearbyCities(map,stations[i],secondaryStations,RAIL);

    while (j > 0) {
            if (!inArray(stations,secondaryStations[j],counter)) {
                stations[counter] = secondaryStations[j];
                counter++;
            }
            j--;
        }
    }

    if (stationsAllowed == 2) return counter;

    int k;
  for(k = counter; k > numNearby; k--) {
  
    LocationID tertiaryStations[71];
    int j = NearbyCities(map,stations[k],tertiaryStations,RAIL);
    
    while (j > 0) {
        if (!inArray(stations,tertiaryStations[j],counter)) {
        stations[counter] = tertiaryStations[j];
        counter++;
      }
      j--;
        }
 }
  
    return counter;
}

int inArray(int *array,int object, int size) {

    int i;
    for(i = 0; i < size; i++) {
        if (array[i] == object) return 0;
    }
    return 1;
}

// #################
// PARSING FUNCTIONS
// #################

// given an "other" location abbreviation, return its ID number
LocationID otherToID(char *abbrev) {
    if (strcmp(abbrev, "C?") == 0) {
        return CITY_UNKNOWN;
    } else if (strcmp(abbrev,"S?") == 0) {
        return SEA_UNKNOWN;
    } else if (strcmp(abbrev,"HI") == 0) {
        return HIDE;
    } else if (strcmp(abbrev,"D1") == 0) {
        return DOUBLE_BACK_1;
    } else if (strcmp(abbrev,"D2") == 0) {
        return DOUBLE_BACK_2;
    } else if (strcmp(abbrev,"D3") == 0) {
        return DOUBLE_BACK_3;
    } else if (strcmp(abbrev,"D4") == 0) {
        return DOUBLE_BACK_4;
    } else if (strcmp(abbrev,"D5") == 0) {
        return DOUBLE_BACK_5;
    } else if (strcmp(abbrev,"TP") == 0) {
        return TELEPORT;
    } else {
        return abbrevToID(abbrev);
    }
}

// PARSING CHECKLIST
    // char *pastPlays
// 1. Take in location ABBREV from trail -> Give AI number      DONE (given by default places.c?)
// 2. Take in special ABBREV (actions etc) -> Give AI number    DONE (gameView.c)
// 3. Take in AI move -> return location ABBREV                 NOT STARTED
// 4. Take in AI action -> return special ABBREV                NOT STARTED
    // PlayerMessage messages[]
// 1. Take in AI message -> Store in messages                   NOT STARTED
// 2. Take messages from storage -> Translate for AI            NOT STARTED



