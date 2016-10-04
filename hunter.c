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
#include <assert.h>
#include "Game.h"
#include "HunterView.h"
#include "Places.h"
#include "Map.h"
#include "commonFunctions.h"


//static LocationID *whereToGo(int player,int *numLocations, int from, int stationsAllowed);
//static LocationID howToGetTo(LocationID dest, LocationID from, int player, Round round);
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


void vanHelsingMove(HunterView h) {
  
  registerBestPlay("CD","Camping for days");
  
  LocationID vanTrail[TRAIL_SIZE];
  giveMeTheTrail(h,PLAYER_VAN_HELSING,vanTrail);
  int vanHealth = howHealthyIs(h,PLAYER_VAN_HELSING);
  
  if ((whereIs(h,PLAYER_DRACULA) == TELEPORT || 
      whereIs(h,PLAYER_DRACULA) == CASTLE_DRACULA) && vanHealth > 2) {
     registerBestPlay("CD","Camping for days");
  } else if (vanHealth < 6) {
     submitID(vanTrail[0]);
  } else if (vanTrail[0] != CASTLE_DRACULA) {
    registerBestPlay("CD","Camping for days");
  } else if (vanTrail[1] == KLAUSENBURG) {
    registerBestPlay("GA","GALATZ patrol"); 
  } else if (vanTrail[1] == GALATZ) {
    registerBestPlay("KL","KLAUSENBURG patrol"); 
  }
  
  LocationID dracTrail[TRAIL_SIZE];
  giveMeTheTrail(h,PLAYER_DRACULA,dracTrail);
  if (dracTrail[0] == vanTrail[0] && dracTrail[0] <= 70 && dracTrail[0] >= 0) {
    submitID(vanTrail[0]);    
    return;
  }
  
  if (dracTrail[1] == CASTLE_DRACULA && dracTrail[0] == CITY_UNKNOWN) {
    if (vanHealth > 2) { 
      if (vanTrail[0] == KLAUSENBURG) registerBestPlay("GA","Dracula's at GA");
      else registerBestPlay("KL","Dracula's at KL");
    }
  }
  
}
void lordGodalmingMove(HunterView h) {
  
  int player = whoAmI(h);
  if (whereIs(h,player) < 0 && whereIs(h,player) >= NUM_MAP_LOCATIONS) {
  registerBestPlay("ST","First Move"); 
  return;
  }

  //LocationID otherPlayers[3]; //Where are the other players
  int i = 0;


  int numPlaces;
  LocationID *placesToGo = whereCanIgo(h,&numPlaces,1,1,0);
  LocationID playerTrail[TRAIL_SIZE];
  giveMeTheTrail(h,player,playerTrail);

  if (howHealthyIs(h,player) < 2) {
    submitID(playerTrail[0]); return;
  }
  for (i = 0; i < numPlaces; i++) {
    if (placesToGo[i] != whereIs(h,player) &&
        placesToGo[i] != whereIs(h,(player+1)%4)&&
        placesToGo[i] != whereIs(h,(player+2)%4)&&
        placesToGo[i] != whereIs(h,(player+3)%4)) {
          submitID(placesToGo[i]); break;
      }
  }
  if (whereIs(h,PLAYER_DRACULA) == whereIs(h,player))
      submitID(playerTrail[0]);
  
}

static void submitID(LocationID dest) {
  char currPlace[3];
  idToAbbrev(dest);
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
/*
static LocationID howToGetTo(LocationID dest, LocationID from, int player, Round round) {

  
  LocationID seenList[NUM_MAP_LOCATIONS] = {0};
  LocationID prevList[NUM_MAP_LOCATIONS] = {0};
  LocationID stepList[NUM_MAP_LOCATIONS] = {0}; //keeps track of how many steps AI can go by rail
               //Since the number of stations allowed keeps changing
  Queue toVisit = newQueue();

  seenList[from] = 1;
  prevList[from] = -1;
  stepList[from] = (round+player)%4;
  QueueJoin(toVisit,from);
  int i;
  
  while(!QueueIsEmpty(toVisit) && !seenList[dest]) {
  
    LocationID curr = QueueLeave(toVisit);
//    printf("Curr = %d\n",curr);
        int numLocations = 0;
      LocationID *connections = whereToGo(player,&numLocations,curr,stepList[curr]); 
    
    for (i = 0; i < numLocations; i++) {
//    if (curr == from)printf("Addresses Include: %d\n",connections[i]);
          if (!seenList[connections[i]]) {
            seenList[connections[i]] = 1;
            prevList[connections[i]] = curr;
      stepList[connections[i]] = (stepList[curr] + 1)%4;  

        if (seenList[dest]) break;
        QueueJoin(toVisit,connections[i]);
          }
      }
    free(connections);
//    if (seenList[dest]) break;
  }
  dropQueue(toVisit);
  LocationID curr = dest;
  while (prevList[curr] != from) {
    curr = prevList[curr];
  }
  return curr;
  
}

static LocationID *whereToGo(int player,int *numLocations, int from, int stationsAllowed) {

    if (from == UNKNOWN_LOCATION) return NULL;
    *numLocations = 1;        //initialise the array size
    LocationID *connections = malloc((*numLocations) * sizeof(LocationID));

    connections[0] = from;    //initialise the array
    Map map = newMap();       //get the game map
    int i;
    
    //find the nearby cities of type ROAD
    connections = NearbyCities(map, from, connections, numLocations, ROAD);
    
    
    //find the nearby cities of type BOAT
    connections = NearbyCities(map, from, connections, numLocations, BOAT);
  
    if (stationsAllowed == 0) return connections;

    int nearbyStations = 0;
    LocationID *railConnections = malloc(sizeof(LocationID));
  railConnections = NearbyCities(map,from,railConnections,&nearbyStations,RAIL);

  if (stationsAllowed > 1) {
    int priStationsFound = nearbyStations;
      for (i = 0; i < priStationsFound; i++) {
        railConnections = NearbyCities(map,railConnections[i],railConnections,&nearbyStations,RAIL);
      
      }

      if (stationsAllowed == 3) {
          int triStationsFound = nearbyStations;
          for (i = priStationsFound; i < triStationsFound; i++) {
          railConnections = NearbyCities(map,railConnections[i],railConnections,&nearbyStations,RAIL);              
          }
      }
    }

    for (i = 0; i < nearbyStations; i++) {

    if(!inArray(connections, railConnections[i], *numLocations)) {

            (*numLocations)++;
            connections = realloc(connections,(*numLocations)*sizeof(LocationID));
        connections[*numLocations-1] = railConnections[i];

          }
    }

  free(railConnections);
    return connections;
}
*/