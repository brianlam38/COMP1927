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
    for (i = 0; i < TRAIL_SIZE ; i++) {
        dracView->trail[i] = malloc(sizeof(encounterData));
        assert(dracView->trail[i] != NULL);
    }
    giveMeTheTrail(dracView, PLAYER_DRACULA, trail);
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
    return currentView->view->players[player]->playerCurrLocation;
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
    assert(validPlace(where) || where == NOWHERE || where == TELEPORT);
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
                    LocationID trail[TRAIL_SIZE]) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    int i;
    //index place of first move of player in pastPlays
    int firstMove = player * CHARS_PER_TURN;
    int nChar = countChar(currentView->view->pastPlays);

    if (player == PLAYER_DRACULA) {
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

void giveMeTheTrail2(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE]) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    //index place of first move of player in pastPlays
    getHistory(currentView->view, player, trail);
    return;
}

//// Functions that query the map to find information about connectivity
// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road,
                        int sea) {
    assert(currentView != NULL && currentView->view != NULL);
    LocationID *adjLoc = connectedLocations(currentView->view, numLocations,
                              whereIs(currentView, PLAYER_DRACULA),
                              PLAYER_DRACULA, giveMeTheRound(currentView),
                              road, FALSE, sea);

    LocationID trail[TRAIL_SIZE];
    LocationID trail2[TRAIL_SIZE];
    int i, index;
    giveMeTheTrail(currentView, PLAYER_DRACULA, trail);
    giveMeTheTrail2(currentView, PLAYER_DRACULA, trail2);
    shiftRight(trail, 0, TRAIL_SIZE - 1);
    trail[0] = UNKNOWN_LOCATION;
    if (trail[1] == NORTH_SEA || trail[1] == ENGLISH_CHANNEL ||
        trail[1] == IRISH_SEA || trail[1] == ATLANTIC_OCEAN ||
        trail[1] == ATLANTIC_OCEAN || trail[1] == BAY_OF_BISCAY ||
        trail[1] == MEDITERRANEAN_SEA || trail[1] == TYRRHENIAN_SEA ||
        trail[1] == 	IONIAN_SEA || trail[1] == ADRIATIC_SEA ||
        trail[1] == 	BLACK_SEA) {
        shiftLeft(adjLoc, 0, *numLocations - 1);
        (*numLocations)--;
        adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
        assert(adjLoc != NULL);
    }
    if (hasDBOrHI(trail2, DRAC_VIEW) == HAS_HIDE) {
        shiftLeft(adjLoc, 0, *numLocations - 1);
        (*numLocations)--;
        adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
        assert(adjLoc != NULL);
    } else if (hasDBOrHI(trail2, DRAC_VIEW) == HAS_DOUBLE_BACK) {
        for (i = 2; i < TRAIL_SIZE; i++) {
            index = inArrayForDrac(adjLoc, trail[i], *numLocations);
            if ((index != -1) && (adjLoc[index] != trail[1])) {
                shiftLeft(adjLoc, index, *numLocations - 1);
                (*numLocations)--;
                adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
                assert(adjLoc != NULL);
            }
        }
    } else if (hasDBOrHI(trail2, DRAC_VIEW) == BOTH_HIDE_AND_DB) {
        for (i = 0; i < TRAIL_SIZE; i++) {
            index = inArrayForDrac(adjLoc, trail[i], *numLocations);
            if (index != -1) {
                shiftLeft(adjLoc, index, *numLocations - 1);
                (*numLocations)--;
                adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
                assert(adjLoc != NULL);
            }
        }
    }
    return adjLoc;
}
// What are my (Dracula's) possible next moves (locations)
/*LocationID *whereCanIgo(DracView currentView, int *numLocations,
                        int road, int sea) {

    assert(currentView != NULL && currentView->view != NULL);
    printf("where is dracula %d\n", whereIs(currentView, PLAYER_DRACULA));
    LocationID *adjLocations = connectedLocations(currentView->view, numLocations,
                              whereIs(currentView, PLAYER_DRACULA),
                              PLAYER_DRACULA, giveMeTheRound(currentView),
                              road, FALSE, sea);
                              int x;
                                for (x = 0 ; x < *numLocations ; x++) {
                                    printf("%d\n",adjLocations[x]);
                                }*/
    //
    // LocationID trail[TRAIL_SIZE];
    // giveMeTheTrail(currentView, PLAYER_DRACULA, trail);
    // if (hasDBOrHI(trail, -1)) {
    //   int overLap = 0;
    //   int x;
    //   int y;
    //   for (x = 0 ; x < *numLocations ; x++) {
    //     for (y = 0 ; y < TRAIL_SIZE ; y++) {
    //       if (trail[y] == adjLocations[x]) overLap++;
    //     }
    //   }
    //   *numLocations -= overLap;
    //   int repeatedLocations = 0;
    //   for (x = 0; x < TRAIL_SIZE - 2; x++) {
    //       if (trail[x] == trail[x + 1] && trail[x] != -1) {
    //         repeatedLocations++;
    //       }
    //   }
    //   *numLocations += repeatedLocations;
    //   LocationID *newAdjLocations = malloc((*numLocations) *sizeof(LocationID));
    //   int z = 0;
    //   for (x = 0 ; x < *numLocations ; x++) {
    //     int check = 0;
    //     for (y = 0 ; y < TRAIL_SIZE ; y++) {
    //       if (trail[y] == adjLocations[x]) check++;
    //       if (check == 0 ) newAdjLocations[z++] = adjLocations[x];
    //
    //     }
    //   }
    //   return newAdjLocations;
    // } else {
    // }
//}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    return connectedLocations(currentView->view, numLocations,
                              whereIs(currentView, player), player,
                              giveMeTheRound(currentView), road, rail, sea);
}
