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
#define CHARS_PER_ROUND 40

struct hunterView {
    GameView view;
    PlayerMessage *messages;
    char *history;
};


//### Function Declerations ###
static int DracLocationKnown(GameView g);
static int inSea(LocationID id);
static int revealDracLocation (HunterView h, int pos);
static int hunterInTrail(GameView g);
static int dracEncounter(HunterView h, int pos);

//#############################
// Creates a new HunterView to summarise the current state of the game
HunterView newHunterView(char *pastPlays, PlayerMessage messages[])
{
    // Alloc + init hunter data
    HunterView hView = malloc(sizeof(struct hunterView));
    hView->view = newGameView(pastPlays, messages);
    hView->history = malloc(strlen(pastPlays + 1));
    strcpy(hView->history,pastPlays);
    // Alloc messages
    int roundNum = getRound(hView->view);
    hView->messages = malloc(sizeof(PlayerMessage) * roundNum);
    // Fill hunter messages with input messages + unused with \0
    int x;
    for (x=0; x<roundNum; x++) {
        strncpy(hView->messages[x],messages[x],MESSAGE_SIZE);
    }

    return hView;
}
     
     
// Frees all memory previously allocated for the HunterView toBeDeleted
void disposeHunterView(HunterView toBeDeleted)
{
    // Dispose messages
    free(toBeDeleted->messages);
    toBeDeleted->messages = NULL;
    //Dispose PastPlays
    free(toBeDeleted->history);
    toBeDeleted->history = NULL;
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
    assert(getScore(currentView->view) >= GAME_START_SCORE);
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

  LocationID dracLocation = getLocation(currentView->view, player);
  if (DracLocationKnown(currentView->view), 0)
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
    if (player != PLAYER_DRACULA) {
      getHistory(currentView->view, player, trail);
        return;
    }
  
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
      if (!revealDracLocation(currentView, i)) {
          if (inSea(trail[i])) trail[i] = SEA_UNKNOWN;
          else trail[i] = CITY_UNKNOWN;
      }
    }
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
                             player,getRound(currentView->view),road,rail,sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea) {
  
  if (player != PLAYER_DRACULA) {
    LocationID from = whereIs(currentView,player);
    return connectedLocations(currentView->view,numLocations,from,
                             player,getRound(currentView->view),road,rail,sea);
  }
  if (DracLocationKnown(currentView->view)) {
    LocationID from = getLocation(currentView->view,player);
    return connectedLocations(currentView->view,numLocations,from,
                             player,getRound(currentView->view),road,rail,sea);
  }
    *numLocations = 0; return NULL;
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
}

static int DracLocationKnown(GameView g) {


  if (getLocation(g, PLAYER_DRACULA) == CASTLE_DRACULA)
    return TRUE;
  if (getLocation(g, PLAYER_DRACULA) == UNKNOWN_LOCATION)
    return TRUE;
  if (getLocation(g, PLAYER_DRACULA) == CITY_UNKNOWN)
    return TRUE;
  if (getLocation(g, PLAYER_DRACULA) == SEA_UNKNOWN)
    return TRUE;
  
  
  int i;
  for (i = 0; i < PLAYER_DRACULA; i++) {
    if (getLocation(g,PLAYER_DRACULA) == getLocation(g,i))
      return TRUE;
  }
  return FALSE;
}


static int inSea(LocationID id) {
  switch (id) {
    case (ADRIATIC_SEA):      return TRUE; 
    case (ATLANTIC_OCEAN):    return TRUE;
    case (BAY_OF_BISCAY):     return TRUE; 
    case (BLACK_SEA):         return TRUE; 
    case (ENGLISH_CHANNEL):   return TRUE;
    case (IONIAN_SEA):        return TRUE;
    case (IRISH_SEA):         return TRUE;
    case (MEDITERRANEAN_SEA): return TRUE;
    case (NORTH_SEA):         return TRUE;
    case (TYRRHENIAN_SEA):    return TRUE;
    case (SEA_UNKNOWN):       return TRUE;
    default:          return FALSE;
  }
}

static int revealDracLocation (HunterView h, int pos) {
  LocationID trail[TRAIL_SIZE];
  getHistory(h->view, PLAYER_DRACULA, trail);
  
  if (DracLocationKnown(h->view)) return TRUE;
    if (getLocation(h->view,PLAYER_DRACULA) == CASTLE_DRACULA) 
      return TRUE;
  
  if (pos == 5 && trail[5] == HIDE) return TRUE; 
    if (pos == 0 && trail[0] >= DOUBLE_BACK_1 && 
        trail[0] <= DOUBLE_BACK_5)    return TRUE;
  
  if (hunterInTrail(h->view) && (trail[pos] == HIDE || 
       (trail[pos] >= DOUBLE_BACK_1 && trail[pos] <= DOUBLE_BACK_5)))
        return TRUE;

    if (dracEncounter(h, pos)) return TRUE;
        
  return FALSE;
}

static int hunterInTrail(GameView g) {
  LocationID drac[TRAIL_SIZE];
    LocationID mina[TRAIL_SIZE];
  LocationID lord[TRAIL_SIZE];
  LocationID seward[TRAIL_SIZE];
    LocationID van[TRAIL_SIZE];
  
    getHistory(g, PLAYER_DRACULA, drac);
    getHistory(g, PLAYER_MINA_HARKER, mina);
  getHistory(g, PLAYER_LORD_GODALMING, lord);
    getHistory(g, PLAYER_DR_SEWARD, seward);
    getHistory(g, PLAYER_VAN_HELSING, van);
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
    int j = 0;
        for (j = 0; j < TRAIL_SIZE; j++) {
          if (drac[i] == mina[j])   return TRUE;
            if (drac[i] == lord[j])   return TRUE;
            if (drac[i] == seward[j]) return TRUE;
            if (drac[i] == van[j])    return TRUE;
        }
    }
    return FALSE;
} 
        
static int dracEncounter(HunterView h, int pos) {
  
  int limit = strlen(h->history) + 1
              - CHARS_PER_ROUND*pos;
    int start = limit - CHARS_PER_ROUND + 3;
  
  int i = 0;
    for (i = start; i < limit; i+=8) {
      if (h->history[i]   == 'D') return TRUE; 
        if (h->history[i+1] == 'D') return TRUE; 
        if (h->history[i+2] == 'D') return TRUE;
    }
   
    return FALSE;
}