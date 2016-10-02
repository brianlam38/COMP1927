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
static void submitID(LocationID dest);
void vanHelsingMove(HunterView h); //our camper
void lordGodalmingMove(HunterView h); //generic player we can specialise him later
//void minaHarkerMove(HunterView h);
//void DrSewardMove(HunterView h);

void decideHunterMove(HunterView gameState)
{
    int player = whoAmI(gameState);
  	if (player == PLAYER_VAN_HELSING) vanHelsingMove(gameState);
  	else lordGodalmingMove(gameState);
}

// #################
// VAN HELSING MOVES
// #################
void vanHelsingMove(HunterView h) {
  // Default VAN move
  registerBestPlay("CD","HELSING - Camping for days");
  
  LocationID vanTrail[TRAIL_SIZE];
  giveMeTheTrail(h,PLAYER_VAN_HELSING,vanTrail);
  int vanHealth = howHealthyIs(h,PLAYER_VAN_HELSING);
  
  if ((whereIs(h,PLAYER_DRACULA) == TELEPORT || 
      whereIs(h,PLAYER_DRACULA) == CASTLE_DRACULA) && vanHealth > 2) {
     registerBestPlay("CD","HELSING - Camping for days");
  } else if (vanHealth < 6) {
     submitID(vanTrail[0]);
  } else if (vanTrail[0] != CASTLE_DRACULA) {
    registerBestPlay("CD","HELSING - Camping for days");
  } else if (vanTrail[1] == KLAUSENBURG) {
    registerBestPlay("GA","HELSING - GALATZ patrol"); 
  } else if (vanTrail[1] == GALATZ) {
   	registerBestPlay("KL","HELSING - KLAUSENBURG patrol"); 
  }
  // If drac = van at most recent move, stay in same location
  LocationID dracTrail[TRAIL_SIZE];
  giveMeTheTrail(h,PLAYER_DRACULA,dracTrail);
  if (dracTrail[0] == vanTrail[0] && dracTrail[0] <= 70 && dracTrail[0] >= 0) {
    submitID(vanTrail[0]);
  	return;
  }
  // If drac is currently at C?, previously CD and VAN at KL/GA --> Go GA/KL
  if (dracTrail[1] == CASTLE_DRACULA && dracTrail[0] == CITY_UNKNOWN) {
    if (vanHealth > 2) { 
      if (vanTrail[0] == KLAUSENBURG) registerBestPlay("GA","HELSING - Dracula's at GA");
      else registerBestPlay("KL","HELSING - Dracula's at KL");
    }
  }
  
}
// ####################
// LORD GODALMING MOVES
// ####################
void lordGodalmingMove(HunterView h) {
  
  int player = whoAmI(h);
  if (whereIs(h,player) == UNKNOWN_LOCATION) {
	registerBestPlay("ST","First Move"); 
	return;
  }

  LocationID otherPlayers[3]; //Where are the other players
  int i = 0;
  for (i = 0;i < 3; i++) {
    otherPlayers[i] = whereIs(h,((player+i)%4));
  }


  int numPlaces;
  LocationID *placesToGo = whereCanIgo(h,&numPlaces,1,1,0);
  LocationID playerTrail[TRAIL_SIZE];
  giveMeTheTrail(h,player,playerTrail);

  if (howHealthyIs(h,player) < 2) {
    submitID(playerTrail[0]); return;
  }
  for (i = 0; i < numPlaces; i++) {
   	if (placesToGo[i] != whereIs(h,player) &&
        placesToGo[i] != otherPlayers[0]   &&
        placesToGo[i] != otherPlayers[1]   &&
        placesToGo[i] != otherPlayers[2]) {
      		submitID(placesToGo[i]); break;
    	}
  }    
  if (whereIs(h,PLAYER_DRACULA) == whereIs(h,player))
    	submitID(playerTrail[0]);
  
}
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

