// GameView.c ... GameView ADT implementation
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"       //use the Map ADT
#include "Places.h"    //use the Places ADT
#include "commonFunctions.h"    //Our ADT

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
    if (((nChar + 1)/CHARS_PER_TURN) != 0) {    //if it is not before the first play (ie, messages is not empty)
        currentView->messages = calloc(((nChar + 1)/CHARS_PER_TURN) , sizeof(char[MESSAGE_SIZE]));
        assert(currentView->messages != NULL);
        for (i = 0; i < currentView->roundNumber * NUM_PLAYERS + player; i++) {
            strcpy(currentView->messages[i], messages[i]);    //copy each element in messages
        }
    } else {
        currentView->messages = NULL;    //initialise messages to NULL if it's empty
    }
    //fill in info for each player
    for (player = PLAYER_LORD_GODALMING; player < NUM_PLAYERS; player++) {
        currentView->players[player] = malloc(sizeof(playerInfo));
        assert(currentView->players[player] != NULL);
        currentView->players[player]->playerHealth = getHealth(currentView, player);
        currentView->players[player]->playerCurrLocation = getLocation(currentView, player);
        getHistory(currentView, player, currentView->players[player]->playerTrail);    //fill in the trail
    }
    return currentView;
}

// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted) {
    assert(toBeDeleted != NULL);
    PlayerID player;
    for (player = PLAYER_LORD_GODALMING; player < NUM_PLAYERS; player++) {
        if (toBeDeleted->players[player] != NULL) free(toBeDeleted->players[player]);
        toBeDeleted->players[player] = NULL;
    }
    if (toBeDeleted->messages != NULL) free(toBeDeleted->messages);
    toBeDeleted->messages = NULL;
    if (toBeDeleted->pastPlays != NULL) free(toBeDeleted->pastPlays);
    toBeDeleted->pastPlays = NULL;
    free(toBeDeleted);
    toBeDeleted = NULL;
}

//// Functions to return simple information about the current state of the game
// Get the current round
Round getRound(GameView currentView) {
    assert(currentView != NULL);
    Round round;        //current round to be returned
    int nChar = countChar(currentView->pastPlays);
    if (nChar < CHARS_PER_ROUND - 1) {
        round = 0;
    } else {
        round = ((nChar - 39)/CHARS_PER_ROUND) + 1;
    }
    return round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView) {
    assert(currentView != NULL);
    PlayerID currentPlayer;        //current player ID to be returned
    int nChar = countChar(currentView->pastPlays);
    //number of plays % number of player gives the remainder equal to player ID
    currentPlayer = ((nChar + 1)/CHARS_PER_TURN) % NUM_PLAYERS;
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
    int firstMove;                                 //index place of first move of player in pastPlays
    int nChar = countChar(currentView->pastPlays); //number of chars in pastPlays
    //count the number of times a hunter is teleported to the hospital
    for (player = PLAYER_LORD_GODALMING; player <= PLAYER_MINA_HARKER; player++) {
        firstMove = player * CHARS_PER_TURN;
        //loop each time a hunter has played
        for (i = firstMove; i < nChar - 2; i += CHARS_PER_ROUND) {
            //hunters' current location in each turn
            char currLocation[] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};
            health = hunterTurnHealth(&currentView->pastPlays[i], health, prevLocation, abbrevToID(currLocation));
            prevLocation = abbrevToID(currLocation);
            if (health <= 0) {
                //reset the life points to max when the hunter had died
                health = GAME_START_HUNTER_LIFE_POINTS;
                teleportCount++;
            }
        }
    }
    //count the number of times Dracula finishes a turn
    if ((countChar(currentView->pastPlays) + 1) % CHARS_PER_ROUND == 0) {
        dracTurnCount = round + 1;    //if the round has just finished (ie, Dracula just finished his move)
    } else {
        dracTurnCount = round;        //the round is still running (ie, Dracula hasn't finished his move in that round yet)
    }
    //count the number of times a Vampire matures
    firstMove = PLAYER_DRACULA * CHARS_PER_TURN;
    //loop each time dracula has played
    for (i = firstMove; i < nChar; i += CHARS_PER_ROUND) {
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
    LocationID currID;                                    //current location
    int i;
    int eventCounter;                                     //counter for the events encountered by each player each turn
    int firstMove = player * CHARS_PER_TURN;              //index place of first move of player in pastPlays
    int nChar = countChar(currentView->pastPlays);        //number of chars in pastPlays
    //health calculation for dracula
    if (player == PLAYER_DRACULA) {
        health = GAME_START_BLOOD_POINTS;
        if (round == 0) return health;
        initialiseTrail(currentView->players[player]->playerTrail);
        //loop each time dracula has played
        for (i = firstMove; i < nChar - 2; i += CHARS_PER_ROUND) {
            int type = NONE;
            char dracLocation[] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};
            //get the trail for dracula
            updatePlayerTrail(currentView->players[player]->playerTrail, otherToID(dracLocation));
            //get the exact location of dracula when he uses hide or double back
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
        for (i = 3; i < nChar; i += CHARS_PER_TURN) {
            //loop through the events a hunter encountered in each play
            for (eventCounter = 0; eventCounter < NUM_EVENT_ENCOUNTER; eventCounter++) {
                if (currentView->pastPlays[i + eventCounter] == 'D') {
                    //Dracula lost life points if he met a hunter
                    health -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
            }
        }
    } else {
        //health calculation for hunters
        health = GAME_START_HUNTER_LIFE_POINTS;
        if (round == 0) return health;
        //loop each time a hunter has played
        for (i = firstMove; i < nChar; i += CHARS_PER_ROUND) {
            char currLocation[] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};
            health = hunterTurnHealth(&currentView->pastPlays[i], health, prevLocation, abbrevToID(currLocation));    //get health for each turn
            prevLocation = abbrevToID(currLocation);
            if (health <= 0) {
                //reset hunters' health if he has died
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
    int nChar = countChar(currentView->pastPlays);           //number of chars in pastPlays
    if (nChar <= player * CHARS_PER_TURN) return NOWHERE;    //player has not had a turn yet
    getHistory(currentView, player, currentView->players[player]->playerTrail);
    return currentView->players[player]->playerTrail[0];
}

//// Functions that return information about the history of the game
// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player, LocationID trail[TRAIL_SIZE]) {
    assert(currentView != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    int i = 0;
    int firstMove = player * CHARS_PER_TURN;              //index of first move
    int nChar = countChar(currentView->pastPlays);        //number of chars in pastPlays
    initialiseTrail(trail);                    //initialise all the trail locations
    //loop till the last round
    for (i = firstMove; i < nChar - 2; i += CHARS_PER_ROUND) {
        //get current location of each round and update it to the trail
        char currLocation[] = {currentView->pastPlays[i+1], currentView->pastPlays[i+2], '\0'};
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
    assert(from >= MIN_MAP_LOCATION && from <= MAX_MAP_LOCATION);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    assert(round >= 0);
    *numLocations = 1;        //initialise the array size
    LocationID *connections = malloc((*numLocations) * sizeof(LocationID));
    assert(connections != NULL);
    connections[0] = from;    //initialise the array
    Map map = newMap();       //get the game map
    int i, j;
    //find the nearby cities of type ROAD
    if (road) {
        connections = NearbyCities(map, from, connections, numLocations, ROAD);
        //case: Dracula can't move to hospital
        //delete the location of hospital if it is in the connections array
        if (player == PLAYER_DRACULA && inArray(connections, ST_JOSEPH_AND_ST_MARYS, *numLocations)) {
            for (i = 0; i < *numLocations; i++) {
                if (connections[i] == ST_JOSEPH_AND_ST_MARYS) {
                    for (j = i; j + 1 < *numLocations; j++) {
                        connections[j] = connections[j + 1];    //shift the array elements
                    }
                    i = *numLocations;
                }
            }
            //reset the size of array
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
        int stationsAllowed = (player + round) % 4;    //number of stations that is allowed to pass through
        int priNumStations = 0;    //number of next stations
        int secNumStations = 0;    //number of stations next to the "next next stations"
        int numStations = 1;       //number elements(stations) in railConnections array
        LocationID *railConnections = malloc(numStations * sizeof(LocationID));
        assert(railConnections != NULL);
        railConnections[0] = from;    //another array to store just the locations with rail connections
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
            //store all the secondary rail stations to railConnections array
            railConnections = NearbyCities(map, railConnections[i], railConnections, &numStations, RAIL);
        }
        if (stationsAllowed == 3) {
            secNumStations = numStations;
            //loop through only the secondary rail stations and find their nearby rail stations
            for (i = priNumStations - 1; i < secNumStations; i++) {
                //store all the tertiary rail stations to railConnections array
                railConnections = NearbyCities(map, railConnections[i], railConnections, &numStations, RAIL);
            }
        }
        //put the nearby cities of type RAIL to the connections array without duplicates
        for (i = 0; i < numStations; i++) {
            for (j = 0; j < *numLocations; j++) {
                //check if the location exist in connections array
                if (connections[j] == railConnections[i]) j = *numLocations + 1;
            }
            //only the "new" locations are added to the connections array
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
