// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h" //... if you decide to use the Map ADT

typedef struct _encounterData {
    LocationID loc;
    int encounterType;
} encounterData;

  
struct dracView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView view;
    PlayerMessage *messages;
    encounterData encounters[TRAIL_SIZE + 1];
};

static int DracLocationKnown(GameView g);
static int inSea(LocationID id);
static void addToEncounters(encounterData ecounters, DracView currentView);

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->view = newGameView(pastPlays, messages[]);

    int turnNum = getRound(dracView->view);
    dracView->messages = malloc(sizeof(PlayerMessage) * turnNum);
    // Fill hunter messages with input messages + unused with \0
    int x;
    for (x=0; x<turnNum; x++) {
        strncpy(hunterView->messages[x],messages[x],MESSAGE_SIZE);
    }
    return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    //COMPLETE THIS IMPLEMENTATION
    free( toBeDeleted->messages );
    toBeDeleted->messages = NULL;
    free(toBeDeleted->view);
    toBeDeleted->view = NULL;
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView->view != NULL);
    return currentView->view->roundNumber;
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView->view != NULL);
    return currentView->view->gameScore;
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView->view != NULL);
    return currentView->view->players[player]->playerHealth;
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
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

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    start = currentView->view-players[player]->playerTrail[0];
    end = currentView->view-players[player]->playerLocation;
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION



    return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    getHistory(currentView->view, player, trail);

}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    LocationID from = getLocation(currentView->view,player);
    return connectedLocations(currentView->view,numLocations,from,
                         player,getRound(currentView->view),road,rail,sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    if (player == PLAYER_DRACULA) return whereCanIgo(currentView, numLocations, road, sea);
    Round round = getRound(currentView->view);
    LocationID loc = getLocation(currentView, player);
    return connectedLocations(currentView->view,numLocations, loc,
                         player,round,road,rail,sea);

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

static void addToEncounters(encounterData encounters, DracView currentView) {
    int x = 0;
  for (;x <= TRAIL_SIZE + 1; x++) {
    if (((currentView->view->roundNumber - x) % 13) == 0) {
        encounters[x]->enCounterType = VAMPIRE;
    }
    
  }
}


