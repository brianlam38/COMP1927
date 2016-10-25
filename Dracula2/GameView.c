////////////////////////// GameView.c /////////////////////////////////////////

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
#include <time.h>

// We have put the gameView struct to commonFunction.h
//  for other files to access it

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[]) {
    //int i;
    int nChar = countChar(pastPlays);

    GameView currView = malloc(sizeof(struct gameView));
    assert(currView != NULL);
    currView->pastPlays = malloc((nChar + 1) * sizeof(char));
    assert(currView->pastPlays != NULL);
    strcpy(currView->pastPlays, pastPlays);

    currView->gameScore = getScore(currView);
    currView->roundNumber = getRound(currView);
    currView->currentPlayer = getCurrentPlayer(currView);

    PlayerID player = currView->currentPlayer;
    //if it is not before the first play (ie, messages is not empty)
//    if (((nChar + 1)/CHARS_PER_TURN) != 0) {
//        currView->messages = calloc(((nChar + 1)/CHARS_PER_TURN) ,
//                                    sizeof(char[MESSAGE_SIZE]));
//        assert(currView->messages != NULL);
//        for (i = 0; i < currView->roundNumber * NUM_PLAYERS + player; i++) {
//            strcpy(currView->messages[i], messages[i]);
//        }
//    } else {
//        //initialise messages to NULL if it's empty
//        currView->messages = NULL;
//    }

    //fill in info for each player
    for (player = PLAYER_LORD_GODALMING; player < NUM_PLAYERS; player++) {
        currView->players[player] = malloc(sizeof(playerInfo));
        assert(currView->players[player] != NULL);
        currView->players[player]->playerHealth = getHealth(currView, player);
        currView->players[player]->playerCurrLocation = getLocation(currView, player);
        getHistory(currView, player, currView->players[player]->playerTrail);
    }
    return currView;
}

// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted) {
    assert(toBeDeleted != NULL);
    PlayerID player;
    for (player = PLAYER_LORD_GODALMING; player < NUM_PLAYERS; player++) {
        if (toBeDeleted->players[player] != NULL)
        free(toBeDeleted->players[player]);
        toBeDeleted->players[player] = NULL;
    }
//    if (toBeDeleted->messages != NULL) free(toBeDeleted->messages);
//    toBeDeleted->messages = NULL;
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

    PlayerID currentPlayer;    //current player ID to be returned
    int nChar = countChar(currentView->pastPlays);

    //number of plays % number of player gives
    // the remainder equal to player ID
    currentPlayer = ((nChar + 1)/CHARS_PER_TURN) % NUM_PLAYERS;
    return currentPlayer;
}

// Get the current score
int getScore(GameView currentView) {
    assert(currentView != NULL);

    int score = GAME_START_SCORE;    //the current score to be returned
    //number of times Dracula finishes a turn
    int dracTurnCount = 0;
    //number of times a Vampire matures
    int matureCount = 0;
    //number of times a hunter is teleported to the hospital
    int teleportCount = 0;
    //hunters' health in each round
    int health = GAME_START_HUNTER_LIFE_POINTS;
    //hunters' previous location in each round
    LocationID prevLocation = UNKNOWN_LOCATION;
    //index place of first move of player in pastPlays
    int firstMove;
    int i = 0;
    PlayerID player;
    Round round = getRound(currentView);
    int nChar = countChar(currentView->pastPlays);

    //count the number of times a hunter is teleported to the hospital
    for (player = PLAYER_LORD_GODALMING; player <= PLAYER_MINA_HARKER; player++) {
        firstMove = player * CHARS_PER_TURN;
        //loop each time a hunter has played
        for (i = firstMove; i < nChar - 2; i += CHARS_PER_ROUND) {
            //hunters' current location in each turn
            char currLocation[3];
            strToAbbrev(&currentView->pastPlays[i], currLocation);
            health = hunterTurnHealth(&currentView->pastPlays[i],
                                      health, prevLocation,
                                      abbrevToID(currLocation));
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
        //if the round has just finished (ie, Dracula just finished his move)
        dracTurnCount = round + 1;
    } else {
        //the round is still running
        // (ie, Dracula hasn't finished his move in that round yet)
        dracTurnCount = round;
    }

    //count the number of times a Vampire matures
    firstMove = PLAYER_DRACULA * CHARS_PER_TURN;
    for (i = firstMove; i < nChar; i += CHARS_PER_ROUND) {
        if (currentView->pastPlays[i+5] == 'V') {
            matureCount++;
        }
    }

    //calculate the final score
    score -= (teleportCount * SCORE_LOSS_HUNTER_HOSPITAL +
              dracTurnCount * SCORE_LOSS_DRACULA_TURN +
              matureCount * SCORE_LOSS_VAMPIRE_MATURES);
    assert(score <= GAME_START_SCORE);
    return score;
}

int getHealth(GameView currentView, PlayerID player) {
    assert(currentView != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);

    //hunters' previous location in each turn
    LocationID prevLocation = UNKNOWN_LOCATION;
    //index place of first move of player in pastPlays
    int firstMove = player * CHARS_PER_TURN;
    LocationID currID;                //current location
    int health = 0;
    Round round = getRound(currentView);
    int nChar = countChar(currentView->pastPlays);
    int i, entCount;

    //health calculation for dracula
    if (player == PLAYER_DRACULA) {
        health = GAME_START_BLOOD_POINTS;
        if (round == 0) return health;

        initialiseTrail(currentView->players[player]->playerTrail);
        //loop each time dracula has played
        for (i = firstMove; i < nChar - 2; i += CHARS_PER_ROUND) {
            int type = NONE;
            updatePlayerTrail(currentView->players[player]->playerTrail,
                              &currentView->pastPlays[i], PLAYER_DRACULA);
            currID = currentView->players[player]->playerTrail[0];
            if (validPlace(currID))
            type = idToType(currID);
            if (currID == SEA_UNKNOWN || type == SEA) {
                health -= LIFE_LOSS_SEA;
            } else if (currID == TELEPORT || currID == CASTLE_DRACULA) {
                health += LIFE_GAIN_CASTLE_DRACULA;
            }
        }
        //loop each time a player has played
        for (i = 3; i < nChar; i += CHARS_PER_TURN) {
            //loop through the events a hunter encountered in each play
            for (entCount = 0; entCount < NUM_EVENT_ENCOUNTER; entCount++) {
                if (currentView->pastPlays[i + entCount] == 'D') {
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
            char currLocation[3];
            strToAbbrev(&currentView->pastPlays[i], currLocation);
            health = hunterTurnHealth(&currentView->pastPlays[i],
                                      health, prevLocation,
                                      abbrevToID(currLocation));
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

    int nChar = countChar(currentView->pastPlays);
    //player has not had a turn yet
    if (nChar <= player * CHARS_PER_TURN) return NOWHERE;

    getHistory(currentView, player,
               currentView->players[player]->playerTrail);

	if (getHealth(currentView,player) == 9 && getRound(currentView) > 1 && player != PLAYER_DRACULA) {
    	LocationID curr = getCurrentPlayer(currentView);
    	int offset = 0;

    	if (curr > player)
			offset = -(curr-player)*CHARS_PER_TURN;
    	else
			offset = -(5-player+curr)*CHARS_PER_TURN;

    	offset += nChar+1;
    	int health = hunterTurnHealth(&currentView->pastPlays[offset],1,
		currentView->players[player]->playerTrail[1],currentView->players[player]->playerTrail[0]);
		if (health <= 0) return ST_JOSEPH_AND_ST_MARYS;
	}
    return currentView->players[player]->playerTrail[0];
}

//// Functions that return information about the history of the game
// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                LocationID trail[TRAIL_SIZE]) {
    assert(currentView != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);

    int i = 0;
    int firstMove = player * CHARS_PER_TURN;    //index of first move
    int nChar = countChar(currentView->pastPlays);

    initialiseTrail(trail);
    //don't give the actual location of Dracula,
    // so never pass in Dracula ID to updatePlayerTrail
    if (player == PLAYER_DRACULA) player --;
    //loop till the last round
    for (i = firstMove; i < nChar - 2; i += CHARS_PER_ROUND) {
        updatePlayerTrail(trail, &currentView->pastPlays[i], player);
    }
}

//// Functions that query the map to find information about connectivity
// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    assert(from >= MIN_MAP_LOCATION && from <= MAX_MAP_LOCATION);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    assert(round >= 0);

    *numLocations = 1;        //initialise the array size
    LocationID *connections = malloc((*numLocations) * sizeof(LocationID));
    assert(connections != NULL);
    connections[0] = from;    //initialise the array
    Map map = newGraph();       //get the game map
    int i, j;

    //find the nearby cities of type ROAD
    if (road) {
        connections = NearbyCities(map, from, connections, numLocations, ROAD);
        //case: Dracula can't move to hospital
        // delete the location of hospital if it is in the connections array
        if (player == PLAYER_DRACULA &&
            inArray(connections, ST_JOSEPH_AND_ST_MARYS, *numLocations) != -1) {
            for (i = 0; i < *numLocations; i++) {
                if (connections[i] == ST_JOSEPH_AND_ST_MARYS) {
                    for (j = i; j + 1 < *numLocations; j++) {
                        connections[j] = connections[j + 1];
                    }
                    i = *numLocations;
                }
            }
            //reset the size of array
            (*numLocations)--;
            if ((*numLocations) > 0) {
                connections = realloc(connections, (*numLocations) * sizeof(LocationID));
                assert(connections != NULL);
            } else {
                connections = NULL;
            }
        }
    }

    //find the nearby cities of type BOAT
    if (sea) {
        connections = NearbyCities(map, from, connections, numLocations, BOAT);
    }

    //find the nearby cities of type RAIL
    if (rail) {
        //number of stations that is allowed to pass through
        int stationsAllowed = (player + round) % 4;
        //number of next stations
        int priNumStations = 0;
        //number of stations next to the "next next stations"
        int secNumStations = 0;
        //number elements(stations) in railConnections array
        int numStations = 1;
        //array of connected rail stations
        LocationID *railConnections = malloc(numStations * sizeof(LocationID));
        assert(railConnections != NULL);
        railConnections[0] = from;

        if (player == PLAYER_DRACULA || stationsAllowed == 0)
            return connections;

        //get the primary rail connections from the given location
        connections = NearbyCities(map, from, connections, numLocations, RAIL);
        if (stationsAllowed == 1) return connections;

        //get the primary rail connections and
        // store it into the railConnections array
        railConnections = NearbyCities(map, from, railConnections,
                                       &numStations, RAIL);
        priNumStations = numStations;
        //loop through the primary rail stations and find their nearby rail stations
        for (i = 0; i < priNumStations; i++) {
            railConnections = NearbyCities(map, railConnections[i],
                                           railConnections, &numStations, RAIL);
        }

        if (stationsAllowed == 3) {
            secNumStations = numStations;
            //loop through only the secondary rail stations and
            // find their nearby rail stations
            for (i = priNumStations - 1; i < secNumStations; i++) {
                railConnections = NearbyCities(map, railConnections[i],
                                               railConnections, &numStations, RAIL);
            }
        }

        //put the nearby cities of type RAIL to
        // the connections array without duplicates
        for (i = 0; i < numStations; i++) {
            for (j = 0; j < *numLocations; j++) {
                if (connections[j] == railConnections[i]) j = *numLocations;
            }
            //only the "new" locations are added to the connections array
            if (j != (*numLocations) + 1) {
                (*numLocations)++;
                connections = realloc(connections,
                                      *numLocations * sizeof(LocationID));
                assert(connections != NULL);
                connections[*numLocations - 1] = railConnections[i];
            }
        }
        free(railConnections);
    }
    return connections;
}
