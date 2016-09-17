// HunterView.c ... HunterView ADT implementation
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
// #include "Map.h" ... if you decide to use the Map ADT

typedef struct _hunterView *HunterView;

 struct _hunterView {
    GameView view;
    PlayerMessage *messages;
};


//### Function Declerations ###
static int DracLocationKnown(GameView g);
static int inSea(LocationID id)
//#############################
// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    // Alloc + init hunter data
    HunterView hunterView = malloc(sizeof(struct _hunterView));
    hunterView->view = newGameView(pastPlays, messages);
    // Alloc messages
    int turnNum = getRound(hunterView->view);
    hunterView->messages = malloc(sizeof(PlayerMessage) * turnNum);
    // Fill hunter messages with input messages + unused with \0
    int x;
    for (x=0; x<turnNum; x++) {
        strncpy(hunterView->messages[x],messages[x],MESSAGE_SIZE);
    }

    return hunterView;
}
     
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    // Dispose messages
    free(toBeDeleted->messages);
    toBeDeleted->messages = NULL;
    // Dispose view
    disposeGameView(toBeDeleted->view);
    toBeDeleted->view = NULL;
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
  if (player != PLAYER_DRACULA)
    return getLocation(currentView->view, player); 

  dracLocation = getLocation(currentView->view, player);
  if (DracLocationKnown(currentView->view))
    return dracLocation; 
  if (inSea(dracLocation)) return SEA_UNKNOWN;
  
  return CITY_UNKNOWN;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(HunterView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    getHistory(currentView->view, player, trail);
}

//// Functions that query the map to find information about connectivity

// What are my possible next moves (locations)  
LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION 
    int player = getCurrentPlayer(currentView->view);
    LocationID from = getLocation(currentView->view, player);
    return connectedLocations(currentView->view, numLocations, from,
                              player, road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea) {
  
  if (player != PLAYER_DRACULA) {
    LocationID from = whereIs(currentView->view,player);
    return connectedLocations(currentView->view,numLocations,from,
                             player,getRound(currentView->view),road,rail,sea);
  }
  if (DracLocationKnown(currentView->view)) {
    LocationID from = getLocation(currentView->view,player);
    return connectedLocations(currentView->view,numLocations,from,
                             player,getRound(currentView->view),road,rail,sea);
  }
    &numLocations = 0; return NULL;
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

static int DracLocationKnown(GameView g) {

  int i; 
  for (i = 0; i < PLAYER_DRACULA; i++) {
     if (getLocation(g,i) == getLocation(g,PLAYER_DRACULA))
       return 1;
  }
  return 0;
}


static int inSea(LocationID id) {
  switch (id) {
    case (ADRIATIC_SEA): return 1; 
    case (ATLANTIC_OCEAN): return 1;
    case (BAY_OF_BISCAY): return 1; 
    case (BLACK_SEA): return 1; 
    case (ENGLISH_CHANNEL): return 1;
    case (IONIAN_SEA): return 1;
    case (IRISH_SEA): return 1;
    case (MEDITERRANEAN_SEA): return 1;
    case (NORTH_SEA): return 1;
    case (TYRRHEANIAN_SEA): return 1; 
    default: return 0;
  }
}
