// hunter.c
// Implementation of your "Fury of Dracula" hunter AI
//HunterView functions: 
/*
Round giveMeTheRound(HunterView currentView);
PlayerID whoAmI(HunterView currentView);
int giveMeTheScore(HunterView currentView);
int howHealthyIs(HunterView currentView, PlayerID player);
LocationID whereIs(HunterView currentView, PlayerID player);
void giveMeTheTrail(HunterView currentView, PlayerID player,
                        LocationID trail[TRAIL_SIZE]);

LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea);
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea);

*/

// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include "Places.h"

#define G_START 60 // Godalming = Stratsbourg  (centre)
#define S_START 37 // Seward = Lisbon          (left)
#define V_START 66 // Van Helsing = Varna      (right)
#define M_START 27 // Mina Harker = Galway     (top)

static void submitID(LocationID dest);
//void lordGodalmingMove(HunterView h);
//void DrSewardMove(HunterView h);
//void vanHelsingMove(HunterView h);
//void minaHarkerMove(HunterView h);

/*
    Current Strategy:
        (1) Godalming starts in the centre of the map. Remaining players spread out LEFT, RIGHT, TOP.
        (2) Remaining players converge on Godalming, the leader.
        (3) If trail is picked up, all hunters converge on Drac
        (4) If 6 rounds pass without picking up
*/

void decideHunterMove(HunterView gameState)
{  
    int player = whoAmI(gameState);         // store curr player
    int round = giveMeTheRound(gameState);  // store curr round

    LocationID dTrail[TRAIL_SIZE];                     // gets drac trail
    giveMeTheTrail(gameState,PLAYER_DRACULA,dracTrail);

    LocationID hTrail[TRAIL_SIZE];          // get current hunter trail
    giveMeTheTrail(h,player,hunterTrail);

    int locID;
    char *move;

    printf("The current round is: %d\n", round);
    if (round == 0) {
        if (player == PLAYER_LORD_GOLDAMING) { locID = G_START; }
        else if (player == PLAYER_DR_SEWARD) { locID = S_START; }
        else if (player == PLAYER_VAN_HELSING) { locID = V_START; }
        else if (player == PLAYER_MINA_HARKER) { locID = M_START; }
        move = idToAbbrev(locID);
        registerBestPlay(move,"ROUND 0 PLACEMENT");
    } else if (round == 1 && dTrail[0] == hTrail[0]) {    // In round 1, if Drac = Hunter Loc, rest
        locID = hTrail[0];
        move = idToAbbrev(locID);
        registerBestPlay(move,"BLAH");
        //convergeOnDrac(h);                // remaining hunters converge
    } else {   
        // registerBestPlay moves should be ordered from most important --> least important
    }
}

void convergeOnDrac(HunterView h) {
    int player = whoAmI(h);         // store curr player

    if (player)
}

// Players know when DRAC goes to see or lands in city
// 

// Takes in ID, copies ID Abbrev into array, then declares Abbrev AI move
static void submitID(LocationID dest) {
	char currPlace[3];
  idToAbbrev(dest,currPlace);
  registerBestPlay(currPlace,"Hello");
}

//Returns the next move in order to reach the 'dest'
//LocationID bestWayToGo(LocationID dest,player) {
  //Graph traversal needed
  //probably just make a new function in Map.c
//}

/*
void minaHarkerMove(HunterView h) {
  
}
void DrSewardMove(HunterView h) {
  
}
*/

// Take in ID and places return place Abbrev
// Need to change above functions to take input p
/*
char LocationIDToAbbrev(Place p, LocationID loc) {
  	int i;
  	for (i = 0; i < NUM_MAP_LOCATIONS; i++) {	// Search places array for loc ID.
      	if (p[i]->id == loc) {								// If matched, return abbrev
        	return p[i]->*abbrev;
        }
    }
  	return "NONE";
}

*/

