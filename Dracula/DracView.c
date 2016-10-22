// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "commonFunctions.h"

typedef struct _encounterData {
    LocationID location;    // location ID of the trails
    int numTraps;           // number of traps in the location
    int numVamps;           // number of vampires in the location
} encounterData;

struct dracView {
    GameView view;
    encounterData *trail[TRAIL_SIZE];    // encounter detail in Dracula's trail
};

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[]) {
    int i;
    LocationID trail[TRAIL_SIZE];

    DracView dracView = malloc(sizeof(struct dracView));
    assert(dracView != NULL);
    dracView->view = newGameView(pastPlays, messages);
    dracView->view->players[PLAYER_DRACULA]->playerCurrLocation =
        whereIs(dracView, PLAYER_DRACULA);

    for (i = 0; i < TRAIL_SIZE ; i++) {
        dracView->trail[i] = malloc(sizeof(encounterData));
        assert(dracView->trail[i] != NULL);
    }
    giveMeTheTrail(dracView, PLAYER_DRACULA, trail, FALSE);
    for (i = 0; i < TRAIL_SIZE ; i++) {
        dracView->trail[i]->location = trail[i];
        whatsThere(dracView, trail[i], &dracView->trail[i]->numTraps,
                   &dracView->trail[i]->numVamps);
    }
    return dracView;
}

// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted) {
    assert(toBeDeleted != NULL);

    int i;
    disposeGameView(toBeDeleted->view);
    for (i = 0; i < TRAIL_SIZE ; i++) {
        free(toBeDeleted->trail[i]);
        toBeDeleted->trail[i] = NULL;
    }
    free(toBeDeleted);
    toBeDeleted = NULL;
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView) {
    assert(currentView != NULL && currentView->view != NULL);
    return currentView->view->roundNumber;
}

// Get the current score
int giveMeTheScore(DracView currentView) {
    assert(currentView != NULL && currentView->view != NULL);
    return currentView->view->gameScore;
}


// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    return currentView->view->players[player]->playerHealth;
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);

    LocationID trail[TRAIL_SIZE];
    giveMeTheTrail(currentView, player, trail, FALSE);
    return trail[0];
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
              LocationID *start, LocationID *end) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);

    LocationID trail[TRAIL_SIZE];
    getHistory(currentView->view, player, trail);    // fill the trail
    *start = trail[1];
    *end = trail[0];
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                int *numTraps, int *numVamps) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(validPlace(where) || where == NOWHERE);

    int i;
    //index place of first move of Dracula in pastPlays
    int firstMove = PLAYER_DRACULA * CHARS_PER_TURN;
    int nChar = countChar(currentView->view->pastPlays);
    LocationID trail[TRAIL_SIZE];
    *numTraps = 0;        //initialise to 0 before counting
    *numVamps = 0;        //initialise to 0 before counting

    if (where == NOWHERE || idToType(where) == SEA) {
        return;
    } else {
        initialiseTrail(trail);
        //loop and keep updating trail of Dracula till the last round
        for (i = firstMove; i < nChar - 2; i += CHARS_PER_ROUND) {
            updatePlayerTrail(trail, &currentView->view->pastPlays[i],
                              PLAYER_DRACULA);
            if (trail[0] == where) {
                //update the number of encounters occurred in that place
                numEncounter(trail, currentView->view->pastPlays[i+3],
                             where, numTraps, numVamps);
                numEncounter(trail, currentView->view->pastPlays[i+4],
                             where, numTraps, numVamps);
            }
        }
    }
    return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE], int hide) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);

    int i;
    //index place of first move of player in pastPlays
    int firstMove = player * CHARS_PER_TURN;
    int nChar = countChar(currentView->view->pastPlays);

    if (!hide) {
        initialiseTrail(trail);
        //loop and keep updating trail of Dracula till the last round
        for (i = firstMove; i < nChar - 2; i += CHARS_PER_ROUND) {
            updatePlayerTrail(trail, &currentView->view->pastPlays[i],
                              PLAYER_DRACULA);
        }
    } else {
        //put original locations in the trail if the player is a hunter
        getHistory(currentView->view, player, trail);
    }
    return;
}

//// Functions that query the map to find information about connectivity
// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanDracgo(DracView currentView, int *numLocations,
                        int road, int sea) {
    assert(currentView != NULL && currentView->view != NULL);

    LocationID currLoc = whereIs(currentView, PLAYER_DRACULA);
    LocationID *adjLoc = connectedLocations(numLocations,
                                            currLoc, PLAYER_DRACULA,
                                            giveMeTheRound(currentView),
                                            road, FALSE, sea);
    LocationID hideTrail[TRAIL_SIZE];
    LocationID trail[TRAIL_SIZE];
    int i, index;
    for(i=0;i<(*numLocations);i++) {
printf("connected[%d] = %s\n",i,idToName(adjLoc[i]));
}
    getHistory(currentView->view, PLAYER_DRACULA, hideTrail);
    giveMeTheTrail(currentView, PLAYER_DRACULA,
                   trail, FALSE);
    shiftRight(hideTrail, 0, TRAIL_SIZE - 1);
    shiftRight(trail, 0, TRAIL_SIZE - 1);
    hideTrail[0] = UNKNOWN_LOCATION;
    trail[0] = UNKNOWN_LOCATION;
    int dbOrHi = hasDBOrHI(hideTrail);
    int dbPos = posOfDb(hideTrail);
    int noShift = 0;

    if (dbOrHi == HAS_DOUBLE_BACK) {
printf("st db\n");
        if (idToType(currLoc) == SEA) {
printf("i=1\n");
            shiftLeft(adjLoc, 0, (*numLocations) - 1);
            (*numLocations)--;
            if ((*numLocations) > 0) {
                adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
                assert(adjLoc != NULL);
            } else {
                free(adjLoc);
                adjLoc = NULL;
            }
        }
        for (i = 2; i < TRAIL_SIZE; i++) {
            index = inArray(adjLoc, trail[i], *numLocations);
printf("index = %d\n", index);
            if ((i == 5) && (hideTrail[5] == HIDE)) noShift = 1;
            if (hideTrail[i] == HIDE && i == dbPos - 1) noShift = 1;
            //if (hideTrail[i] == TELEPORT) noShift = 1;
            if ((index != -1) && (i != dbPos) && (adjLoc[index] != trail[1]) && (noShift != 1)) {
                shiftLeft(adjLoc, index, (*numLocations) - 1);
                (*numLocations)--;
                if ((*numLocations) > 0) {
                    adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
                    assert(adjLoc != NULL);
                } else {
                    free(adjLoc);
                    adjLoc = NULL;
                }
            }
            noShift = 0;
        }
printf("fi db\n");
    } else if (dbOrHi == BOTH_HIDE_AND_DB) {
printf("st both\n");
printf("numLoc = %d\n", *numLocations);
for(i=0;i<(*numLocations);i++) {
printf("adjLoc[%d] = %s\n",i,idToName(adjLoc[i]));
}
        for (i = 1; i < TRAIL_SIZE; i++) {
            index = inArray(adjLoc, trail[i], *numLocations);
            if ((i == 5) && (hideTrail[5] == HIDE)) noShift = 1;
            if (hideTrail[i] == HIDE && i == dbPos - 1) noShift = 1;
            //if (hideTrail[i] == TELEPORT) noShift = 1;
            if ((index != -1) && (i != dbPos) && (noShift != 1)) {
printf("dbpos=%d\n", dbPos);
                shiftLeft(adjLoc, index, (*numLocations) - 1);
                (*numLocations)--;
                if ((*numLocations) > 0) {
printf("st realloc, numLoc = %d\n", *numLocations);
                    adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
                    assert(adjLoc != NULL);
printf("fi realloc\n");
                } else {
                    free(adjLoc);
                    adjLoc = NULL;
                }
            }
            noShift = 0;
        }
for(i=0;i<(*numLocations);i++) {
//printf("adjLoc[%d] = %s\n",i,idToName(adjLoc[i]));
}
printf("fi both\n");
    }
    return adjLoc;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           LocationID from, PlayerID player, Round round,
                           int road, int rail, int sea) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    return connectedLocations(numLocations,
                              from, player, round, road, rail, sea);
}

// check where dracula can go from a given location
LocationID *whereCanIgo(int *numLocations, LocationID trail[TRAIL_SIZE],
                        LocationID hideTrail[TRAIL_SIZE], LocationID currLoc,
                        int road, int sea) {
    LocationID *adjLoc = connectedLocations(numLocations,
                                            currLoc, PLAYER_DRACULA,
                                            1, road, FALSE, sea);
//int y;
//for (y = 0;y<*numLocations;y++) {
//printf("connection = %s\n",idToName(adjLoc[y]));}
    int i, index;
    shiftRight(hideTrail, 0, TRAIL_SIZE - 1);
    shiftRight(trail, 0, TRAIL_SIZE - 1);
    hideTrail[0] = UNKNOWN_LOCATION;
    trail[0] = UNKNOWN_LOCATION;
    int dbOrHi = hasDBOrHI(hideTrail);
    int dbPos = posOfDb(hideTrail);
    int noShift = 0;

    if (dbOrHi == HAS_DOUBLE_BACK) {

        if (idToType(currLoc) != SEA) noShift = 1;
        for (i = 1; i < TRAIL_SIZE; i++) {
            if (trail[i] == currLoc && idToType(currLoc) != SEA) noShift = 1;
            index = inArray(adjLoc, trail[i], *numLocations);
            //if ((i == 5) && (hideTrail[5] == HIDE)) noShift = 1;
            //if (hideTrail[i] == HIDE && i == dbPos - 1) noShift = 1;
            if ((index != -1) && (i != dbPos) && (noShift != 1)) {
                shiftLeft(adjLoc, index, (*numLocations) - 1);
                (*numLocations)--;
                if ((*numLocations) > 0) {
                    adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
                    assert(adjLoc != NULL);
                } else {
                    free(adjLoc);
                    adjLoc = NULL;
                }
            }
            noShift = 0;
        }
    } else if (dbOrHi == BOTH_HIDE_AND_DB) {
        for (i = 1; i < TRAIL_SIZE; i++) {
            index = inArray(adjLoc, trail[i], *numLocations);
            //if ((i == 5) && (hideTrail[5] == HIDE)) noShift = 1;
            //if (hideTrail[i] == HIDE && i == dbPos - 1) noShift = 1;
            //if (hideTrail[i] == TELEPORT) noShift = 1;
//printf("index=%d, dbPos=%d,i=%d\n",index, dbPos, i);
            if ((index != -1) && (i != dbPos) && (hideTrail[i] != HIDE)) {
//printf("del\n");
                shiftLeft(adjLoc, index, (*numLocations) - 1);
                (*numLocations)--;
                if ((*numLocations) > 0) {
                    adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
                    assert(adjLoc != NULL);
                } else {
                    free(adjLoc);
                    adjLoc = NULL;
                }
            }
        }
    }
    return adjLoc;
}
