// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
// #include "Map.h" ... if you decide to use the Map ADT
    
//max number of event encountered each turn
#define NUM_EVENT_ENCOUNTER 3
//number of chars in pastPlays in one round
#define NUM_CHAR_ONE_ROUND  40
//number of chars in pastPlays in one turn
#define NUM_CHAR_ONE_TURN   8

typedef struct _encounterData {
    LocationID location;
    int numTraps;
    int numVamps;
} encounterData;

struct dracView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView view;
    encounterData *encounters[TRAIL_SIZE];
};

int countChar2(char* string);

// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    DracView dracView = malloc(sizeof(struct dracView));
    dracView->view = newGameView(pastPlays, messages);

    int i;
    for (i=0; i < TRAIL_SIZE ;i++) {
      dracView->encounters[i] = malloc(sizeof(encounterData));
      dracView->encounters[i]->numTraps = 0;
      dracView->encounters[i]->numVamps = 0;
    }

    LocationID trail[TRAIL_SIZE];
    getHistory(dracView->view, PLAYER_DRACULA, trail);

    for (i=0; i < TRAIL_SIZE ; i++) {
      dracView->encounters[i]->location = trail[i];
    }

    if ( countChar2(pastPlays) > 32 ) {
      int maxTrail = 1;
      int x = 33;
      while (x < countChar2(pastPlays)) {
        x += NUM_CHAR_ONE_ROUND;
        maxTrail++;
      }
      if (maxTrail > TRAIL_SIZE) maxTrail = TRAIL_SIZE;

      int lastroundPlayers = 0;
      int lastDrac = countChar2(pastPlays) - 7;
      while (pastPlays[lastDrac] != 'D') {
        lastDrac -= 8;
        lastroundPlayers++;
      }

      for(i= 0; i < maxTrail; i++) {
        lastDrac = countChar2(pastPlays) - 7;
        while (pastPlays[lastDrac] != 'D') {
          lastDrac -= 8;
        }
        int prevRounds = i*NUM_CHAR_ONE_ROUND;
        if (pastPlays[lastDrac + 3] == 'T' - prevRounds) dracView->encounters[i]->numTraps++;
        if (pastPlays[lastDrac + 4] == 'V' - prevRounds) dracView->encounters[i]->numVamps++;
        int y;
        if (i == 0) {
          for(y= 0; y < lastroundPlayers; y++) {
            int jump2 = y*NUM_CHAR_ONE_TURN;
            if (getLocation(dracView->view,y) == dracView->encounters[i]->location) {
              if (pastPlays[countChar2(pastPlays)-5-jump2] == 'T' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numTraps--;
              }
              if (pastPlays[countChar2(pastPlays)-4-jump2] == 'T' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numTraps--;
              }
              if (pastPlays[countChar2(pastPlays)-3-jump2] == 'T' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numTraps--;
              }

              if (pastPlays[countChar2(pastPlays)-5-jump2] == 'V' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numVamps--;
              }
              if (pastPlays[countChar2(pastPlays)-4-jump2] == 'V' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numVamps--;
              }
              if (pastPlays[countChar2(pastPlays)-3-jump2] == 'V' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numVamps--;
              }
              if (pastPlays[countChar2(pastPlays)-2-jump2] == 'V' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numVamps--;
              }
            }
          }
        } else {
          for(y= 0; y < NUM_PLAYERS; y++) {
            int jump1 = y*NUM_PLAYERS;
            int jump2 = lastroundPlayers*NUM_CHAR_ONE_TURN;
            int jump3 = (i>0) ? (i-1)*NUM_CHAR_ONE_ROUND : 0 ;
            if (getLocation(dracView->view,y) == dracView->encounters[i]->location) {
              if (pastPlays[countChar2(pastPlays)-5- jump3 - jump2-jump1] == 'T' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numTraps--;
              }
              if (pastPlays[countChar2(pastPlays) -4- jump3 - jump2-jump1] == 'T' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numTraps--;
              }
              if (pastPlays[countChar2(pastPlays) -3- jump3 - jump2-jump1] == 'T' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numTraps--;
              }

              if (pastPlays[countChar2(pastPlays) -5- jump3 - jump2-jump1] == 'V' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numVamps--;
              }
              if (pastPlays[countChar2(pastPlays) -4- jump3 - jump2-jump1] == 'V' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numVamps--;
              }
              if (pastPlays[countChar2(pastPlays) -3- jump3 - jump2-jump1] == 'V' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numVamps--;
              }
              if (pastPlays[countChar2(pastPlays) -2- jump3 - jump2-jump1] == 'V' && pastPlays[countChar2(pastPlays)-7-jump2] != 'D') {
                dracView->encounters[i]->numVamps--;
              }
            }
          }
        }
      }

    }
    return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    free(toBeDeleted->view);
    toBeDeleted->view = NULL;
    free(toBeDeleted);
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL && currentView->view != NULL);
    return getRound(currentView->view);
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL && currentView->view != NULL);
    return getScore(currentView->view);
}


// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL && currentView->view != NULL);
    return getHealth(currentView->view, player);
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    return getLocation(currentView->view, player);
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player, LocationID *start, LocationID *end)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL && currentView->view != NULL);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    LocationID trail[TRAIL_SIZE];
    getHistory(currentView->view, player, trail);
    *start = trail[1];
    *end = trail[0];
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    int i;
    for (i=0; i < TRAIL_SIZE; i++) {
      if (where == currentView->encounters[i]->location) {
        *numTraps = currentView->encounters[i]->numTraps;
        *numVamps = currentView->encounters[i]->numVamps;
        return;
      }
    }

    *numTraps = 0;
    *numVamps = 0;

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
    return connectedLocations(currentView->view,numLocations, getLocation(currentView->view,PLAYER_DRACULA),
                       PLAYER_DRACULA, getRound(currentView->view),road,FALSE,sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return connectedLocations(currentView->view,numLocations, getLocation(currentView->view,player),
                         player, getRound(currentView->view),road,rail,sea);
}

int countChar2(char* string) {
    char *p;
    int nChar = 0;
    for (p = string; *p != '\0'; p++) {
        nChar++;
    }
    return nChar;
}
