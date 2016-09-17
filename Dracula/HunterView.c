// HunterView.c ... HunterView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
// #include "Map.h" ... if you decide to use the Map ADT
     
struct hunterView {
    GameView view;
    PlayerMessage *messages;
};

// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    // Alloc + init hunter data
    HunterView hunterView = malloc(sizeof(struct hunterView));
    hunterView->view = newGameView(pastPlays, messages);
    // Alloc messages
    int turnNum = getRound(hunterView->view);
    hunterView->messages = malloc(sizeof(PlayerMessage) * turnNum);
    // Fill hunter messages with input messages
    int x;
    for (x=0; x<turnNum; x++) {
        strncpy(hunterView->messages[x],messages[x],MESSAGE_SIZE);
    }

    return hunterView;
}
     
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    // Dispose hunter view
    disposeGameView(toBeDeleted->view);
    free(toBeDeleted->messages);

    toBeDeleted->view = NULL;
    assert(toBeDeleted->view == NULL);
    free(toBeDeleted->view);
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(HunterView currentView)
{
    assert(currentView->view != NULL);

    return getRound(currentView->view);
}

// Get the id of current player
PlayerID whoAmI(HunterView currentView)
{
    assert(currentView->view != NULL);

    return getCurrentPlayer(currentView->view);
}

// Get the current score
int giveMeTheScore(HunterView currentView)
{
    assert(currentView->view != NULL);
    assert(getScore(currentView->view) <= GAME_START_SCORE);

    return getScore(currentView->view);
}

// Get the current health points for a given player
int howHealthyIs(HunterView currentView, PlayerID player)
{
    assert(currentView->view != NULL);
    assert(player <= NUM_PLAYERS);

    return getHealth(currentView->view, player);
}

// Get the current location id of a given player
LocationID whereIs(HunterView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return 0;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return NULL;
}
