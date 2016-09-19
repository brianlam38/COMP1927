////////////////////////code for gameView.c ///////////////////////////////////////
// GameView.c ... GameView ADT implementation
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h" //... if you decide to use the Map ADT
#include "Places.h" // Using Places ADT
#include <stdio.h>

typedef struct _playerInfo {
    int playerHealth;
    LocationID playerCurrLocation;
    LocationID playerTrail[TRAIL_SIZE];
} playerInfo;

struct gameView {
    char *pastPlays;
    int gameScore;
    Round roundNumber;
    PlayerID currentPlayer;
    PlayerMessage *messages;
    playerInfo *players[NUM_PLAYERS];
    
};

//max number of event encountered each turn
#define NUM_EVENT_ENCOUNTER 3
//number of chars in pastPlays in one round
#define NUM_CHAR_ONE_ROUND  40
//number of chars in pastPlays in one turn
#define NUM_CHAR_ONE_TURN   8

int inArray(int *array, int object, int size);
LocationID otherToID(char *abbrev);
int countChar(char* string);
int hunterTurnHealth(char *pastPlays, int health, LocationID prevLocation, LocationID currLocation);
void updatePlayerTrail(LocationID trail[TRAIL_SIZE], LocationID newLocation);
LocationID dracSpecialLocation(LocationID currID, LocationID trail[TRAIL_SIZE]);

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[]) {
    int i;
    int nChar = countChar(pastPlays);
    GameView currentView = malloc(sizeof(struct gameView));
    assert(currentView != NULL);
    currentView->pastPlays = malloc((nChar + 1) * sizeof(char));
    assert(currentView->pastPlays != NULL);
    strcpy(currentView->pastPlays, pastPlays);
    currentView->gameScore = getScore(currentView);
    currentView->roundNumber = getRound(currentView);
    currentView->currentPlayer = getCurrentPlayer(currentView);
    PlayerID player = currentView->currentPlayer;
    if (((nChar + 1)/NUM_CHAR_ONE_TURN) != 0) {
        currentView->messages = calloc(((nChar + 1)/NUM_CHAR_ONE_TURN) , sizeof(char[MESSAGE_SIZE]));
        assert(currentView->messages != NULL);
        for (i = 0; i < currentView->roundNumber * NUM_PLAYERS + player; i++) {
            strcpy(currentView->messages[i], messages[i]);
        }
    } else {
        currentView->messages = NULL;
    }
    for (player = PLAYER_LORD_GODALMING; player < NUM_PLAYERS; player++) {
        currentView->players[player] = malloc(sizeof(playerInfo));
        assert(currentView->players[player] != NULL);
        currentView->players[player]->playerHealth = getHealth(currentView, player);
        currentView->players[player]->playerCurrLocation = getLocation(currentView, player);
        getHistory(currentView, player, currentView->players[player]->playerTrail);
    }
    return currentView;
}

// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted) {
    if (toBeDeleted != NULL) {
        PlayerID player;
        for (player = PLAYER_LORD_GODALMING; player < NUM_PLAYERS; player++) {
            if (toBeDeleted->players[player] != NULL) {
                free(toBeDeleted->players[player]);
                toBeDeleted->players[player] = NULL;
            }
        }
        if (toBeDeleted->messages != NULL) {
            free(toBeDeleted->messages);
            toBeDeleted->messages = NULL;
        }
        if (toBeDeleted->pastPlays != NULL) {
            free(toBeDeleted->pastPlays);
            toBeDeleted->pastPlays = NULL;
        }
        free(toBeDeleted);
        toBeDeleted = NULL;
    } 
}

//// Functions to return simple information about the current state of the game
// Get the current round
Round getRound(GameView currentView) {
    assert(currentView != NULL);
    Round round;
    int nChar = countChar(currentView->pastPlays);
    if (nChar < NUM_CHAR_ONE_ROUND - 1) {
        round = 0;
    } else {
        round = ((nChar - 39)/NUM_CHAR_ONE_ROUND) + 1;
    }
    return round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView) {
    assert(currentView != NULL);
    PlayerID currentPlayer;
    int nChar = countChar(currentView->pastPlays);
    currentPlayer = ((nChar + 1)/NUM_CHAR_ONE_TURN) % NUM_PLAYERS;
    return currentPlayer;
}

// Get the current score
int getScore(GameView currentView) {
    assert(currentView != NULL);
    int score = GAME_START_SCORE;                  //the current score to be returned
    Round round = getRound(currentView);           //current round number
    int dracTurnCount = 0;                         //number of times Dracula finishes a turn
    int matureCount = 0;                           //number of times a Vampire matures
    int teleportCount = 0;                         //number of times a hunter is teleported to the hospital
    PlayerID player;
    int health = GAME_START_HUNTER_LIFE_POINTS;    //hunters' health in each turn
    LocationID prevLocation = UNKNOWN_LOCATION;    //hunters' previous location in each turn
    int i = 0;
    int firstMove;
    int nChar = countChar(currentView->pastPlays);        //number of chars in pastPlays
    //count the number of times a hunter is teleported to the hospital
    for (player = PLAYER_LORD_GODALMING; player <= PLAYER_MINA_HARKER; player++) {
        firstMove = player * NUM_CHAR_ONE_TURN;
        //loop each time a hunter has played
        for (i = firstMove; i < nChar - 2; i += NUM_CHAR_ONE_ROUND) {
            char currLocation[] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};           //hunters' current location in each turn
            health = hunterTurnHealth(&currentView->pastPlays[i], health, prevLocation, abbrevToID(currLocation));
            prevLocation = abbrevToID(currLocation);
            if (health <= 0) {
                health = GAME_START_HUNTER_LIFE_POINTS;
                teleportCount++;
            }
        }
    }
    //count the number of times Dracula finishes a turn
    if ((countChar(currentView->pastPlays) + 1) % NUM_CHAR_ONE_ROUND == 0) {
        dracTurnCount = round + 1;
    } else {
        dracTurnCount = round;
    }
    //count the number of times a Vampire matures
    firstMove = PLAYER_DRACULA * NUM_CHAR_ONE_TURN;
    //loop each time dracula has played
    for (i = firstMove; i < nChar; i += NUM_CHAR_ONE_ROUND) {
        if (currentView->pastPlays[i+5] == 'V') {
            matureCount++;
        }
    }
    //calculate the final score
    score -= (teleportCount * SCORE_LOSS_HUNTER_HOSPITAL + dracTurnCount * SCORE_LOSS_DRACULA_TURN + matureCount * SCORE_LOSS_VAMPIRE_MATURES);
    assert(score <= GAME_START_SCORE);
    return score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player) {
    assert(currentView != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    int health = 0;
    Round round = getRound(currentView);                  //current round number
    LocationID prevLocation = UNKNOWN_LOCATION;           //hunters' previous location in each turn
    LocationID currID;
    int i;
    int eventCounter;                                     //counter for the events encountered by each player each turn
    int firstMove = player * NUM_CHAR_ONE_TURN;
    int nChar = countChar(currentView->pastPlays);        //number of chars in pastPlays
    //health calculation for dracula
    if (player == PLAYER_DRACULA) {
        health = GAME_START_BLOOD_POINTS;
        if (round == 0) return health;
        //initialise all the trail locations
        for (i = 0; i < TRAIL_SIZE; i++) currentView->players[player]->playerTrail[i] = UNKNOWN_LOCATION;
        //loop each time dracula has played
        for (i = firstMove; i < nChar - 2; i += NUM_CHAR_ONE_ROUND) {
            int type = NONE;
            char dracLocation[] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};
            //get the trail for dracula
            updatePlayerTrail(currentView->players[player]->playerTrail, otherToID(dracLocation));
            //get the exactly location of dracula when he uses hide or double back
            currID = dracSpecialLocation(otherToID(dracLocation), currentView->players[player]->playerTrail);
            if (validPlace(currID)) type = idToType(currID);
            //check if the place is in sea
            if (currID == SEA_UNKNOWN || type == SEA) {
                health -= LIFE_LOSS_SEA;
            } else if (currID == TELEPORT) {
                health += LIFE_GAIN_CASTLE_DRACULA;
            }
        }
        //loop each time a player has played
        for (i = 3; i < nChar; i += NUM_CHAR_ONE_TURN) {
            for (eventCounter = 0; eventCounter < NUM_EVENT_ENCOUNTER; eventCounter++) {
                if (currentView->pastPlays[i + eventCounter] == 'D') {
                    health -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
            }
        }
    } else {
        //health calculation for hunters
        health = GAME_START_HUNTER_LIFE_POINTS;
        if (round == 0) return health;
        //loop each time a hunter has played
        for (i = firstMove; i < nChar; i += NUM_CHAR_ONE_ROUND) {
            char currLocation[] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};
            health = hunterTurnHealth(&currentView->pastPlays[i], health, prevLocation, abbrevToID(currLocation));
            prevLocation = abbrevToID(currLocation);
            if (health <= 0) {
                health = GAME_START_HUNTER_LIFE_POINTS;
            }
        }
        assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    }
    return health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player) {
    assert(currentView != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    Round round = getRound(currentView);                  //current round number
    if (round == 0) return NOWHERE;                       //player has not had a turn yet
    getHistory(currentView, player, currentView->players[player]->playerTrail);
    return currentView->players[player]->playerTrail[0];
}

//// Functions that return information about the history of the game
// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player, LocationID trail[TRAIL_SIZE]) {
    assert(currentView != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    int i = 0;
    int firstMove = player * NUM_CHAR_ONE_TURN;
    int nChar = countChar(currentView->pastPlays);        //number of chars in pastPlays
    //initialise all the trail locations
    for (i = 0; i < TRAIL_SIZE; i++) trail[i] = UNKNOWN_LOCATION;
    //loop till the last round
    for (i = firstMove; i < nChar - 2; i += NUM_CHAR_ONE_ROUND) {
        char currLocation[3] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};
        updatePlayerTrail(trail, otherToID(currLocation));
    }
}

//// Functions that query the map to find information about connectivity
// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    assert(currentView != NULL);
    assert(*numLocations >= 0 && *numLocations <= NUM_MAP_LOCATIONS);
    assert(validPlace(from));
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    assert(round >= 0);
    *numLocations = 1;        //initialise the array size
    LocationID *connections = malloc((*numLocations) * sizeof(LocationID));
    assert(connections != NULL);
    connections[0] = from;    //initialise the array
    Map map = newMap();
    int i, j;
    //find the nearby cities of type ROAD
    if (road) {
        connections = NearbyCities(map, from, connections, numLocations, ROAD);
        //case: Dracula can't move to hospital
        if (player == PLAYER_DRACULA && inArray(connections, ST_JOSEPH_AND_ST_MARYS, *numLocations)) {
            for (i = 0; i < *numLocations; i++) {
                if (connections[i] == ST_JOSEPH_AND_ST_MARYS) {
                    for (j = i; j + 1 < *numLocations; j++) {
                        connections[j] = connections[j + 1];
                    }
                    i = *numLocations;
                }
            }
            (*numLocations)--;
            connections = realloc(connections, *numLocations * sizeof(LocationID));
            assert(connections != NULL);
        }
    }
    //find the nearby cities of type BOAT
    if (sea) {
        connections = NearbyCities(map, from, connections, numLocations, BOAT);
    }
    //find the nearby cities of type RAIL
    if (rail) {
        int stationsAllowed = (player + round) % 4;
        int priNumStations = 0;
        int secNumStations = 0;
        int numStations = 1;
        LocationID *railConnections = malloc(numStations * sizeof(LocationID));
        assert(railConnections != NULL);
        railConnections[0] = from;
        //no rail allowed for Dracula or stations allowed is zero
        if (player == PLAYER_DRACULA || stationsAllowed == 0) return connections;
        //get the primary rail connections from the given location
        connections = NearbyCities(map, from, connections, numLocations, RAIL);
        if (stationsAllowed == 1) return connections;
        //get the primary rail connections and store it into the railConnections array
        railConnections = NearbyCities(map, from, railConnections, &numStations, RAIL);
        priNumStations = numStations;
        //loop through the primary rail stations and find their nearby rail stations
        for (i = 0; i < priNumStations; i++) {
            railConnections = NearbyCities(map, railConnections[i], railConnections, &numStations, RAIL);
        }
        //loop through the secondary rail stations and find their nearby rail stations
        if (stationsAllowed == 3) {
            secNumStations = numStations;
            for (i = priNumStations - 1; i < secNumStations; i++) {
                railConnections = NearbyCities(map, railConnections[i], railConnections, &numStations, RAIL);
            }
        }
        //put the nearby cities of type RAIL to the connections array
        for (i = 0; i < numStations; i++) {
            for (j = 0; j < *numLocations; j++) {
                if (connections[j] == railConnections[i]) j = *numLocations + 1;
            }
            if (j != *numLocations + 1) {
                (*numLocations)++;
                connections = realloc(connections, *numLocations * sizeof(LocationID));
                assert(connections != NULL);
                connections[*numLocations - 1] = railConnections[i];
            }
        }
    }
    return connections;
}

//check if an element is in the given array
int inArray(int *array, int object, int size) {
    int i;
    for(i = 0; i < size; i++) {
        if (array[i] == object) return 1;
    }
    return 0;
}

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

//To count the number of chars in a string
int countChar(char* string) {
    char *p;
    int nChar = 0;
    for (p = string; *p != '\0'; p++) {
        nChar++;
    }
    return nChar;
}

//get the health of hunter in each turn
int hunterTurnHealth(char *pastPlays, int health, LocationID prevLocation, LocationID currLocation) {
    assert(pastPlays != NULL);
    assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    assert(validPlace(prevLocation) || prevLocation == UNKNOWN_LOCATION);
    assert(validPlace(currLocation) || currLocation == UNKNOWN_LOCATION);
    char *p = pastPlays;
    int i;
    //check if the previous and the current location is the same
    if (currLocation == prevLocation) {
        health += LIFE_GAIN_REST;
        if (health > GAME_START_HUNTER_LIFE_POINTS) {
            health = GAME_START_HUNTER_LIFE_POINTS;
        }
    }
    p += 3;
    //loop each time a hunter has encountered an event
    for (i = 0; i < NUM_EVENT_ENCOUNTER; i++) {
        switch(*p) {
            case 'T' :
                health -= LIFE_LOSS_TRAP_ENCOUNTER;
                break;
            case 'D' :
                health -= LIFE_LOSS_DRACULA_ENCOUNTER;
                break;
            default :
                break;
        }
        p++;
    }
    assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    return health;
}

//updates player's trail by inserting newLocation at the starts
void updatePlayerTrail(LocationID trail[TRAIL_SIZE], LocationID newLocation) {
    int i;
    for (i = TRAIL_SIZE - 1; i > 0; i--)
        trail[i] = trail[i - 1];    //shift location in the array
    trail[i] = newLocation;         //add current location to the array
}
            
LocationID dracSpecialLocation(LocationID currID, LocationID trail[TRAIL_SIZE]) {
    switch (currID) {
        case HIDE:
            currID = trail[0];
            break;
        case DOUBLE_BACK_1:
            currID = trail[1];
            break;
        case DOUBLE_BACK_2:
            currID = trail[2];
            break;
        case DOUBLE_BACK_3:
            currID = trail[3];
            break;
        case DOUBLE_BACK_4:
            currID = trail[4];
            break;
        case DOUBLE_BACK_5:
            currID = trail[5];
            break;
        default:
            break;
    }
    return currID;
}
