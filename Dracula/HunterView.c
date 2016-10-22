// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "commonFunctions.h"

struct hunterView {
    GameView view;
};

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[]) {
    HunterView hunterView = malloc(sizeof(struct hunterView));
    assert(hunterView != NULL);
    hunterView->view = newGameView(pastPlays, messages);
    return hunterView;
}

// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted) {
    assert(toBeDeleted != NULL);
    disposeGameView(toBeDeleted->view);
    free(toBeDeleted);
    toBeDeleted = NULL;
}

//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView) {
    assert(currentView != NULL && currentView->view != NULL);
    return currentView->view->roundNumber;
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView) {
    assert(currentView != NULL && currentView->view != NULL);
    return currentView->view->currentPlayer;
}

// Get the current score
int giveMeTheScore(HunterView currentView) {
    assert(currentView != NULL && currentView->view != NULL);
    return currentView->view->gameScore;
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    return currentView->view->players[player]->playerHealth;
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    return currentView->view->players[player]->playerCurrLocation;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE]) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    getHistory(currentView->view, player, trail);
    return;
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea) {
    assert(currentView != NULL && currentView->view != NULL);
    PlayerID player = whoAmI(currentView);    // get the current player    
    return connectedLocations(numLocations, whereIs(currentView, player), 
                              player, giveMeTheRound(currentView), 
                              road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea) {
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    return connectedLocations(numLocations, whereIs(currentView, player), 
                              player, giveMeTheRound(currentView), 
                              road, rail, sea);
}
