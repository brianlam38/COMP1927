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
                           playerID player, int road, int rail, int sea);

*/

// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include "Places.h"
#include "commonFunctions.h"

#define G_START 60 // Godalming = Stratsbourg  (centre)
#define S_START 37 // Seward = Lisbon          (left)
#define V_START 66 // Van Helsing = Varna      (right)
#define M_START 27 // Mina Harker = Galway     (top)

static void submitID(LocationID dest);
static LocationID *whereToGo(int player,int *numLocations, int from, int stationsAllowed);
static LocationID howToGetTo(LocationID dest, LocationID from, int player, Round round, int *pathLength);
static int convergeOnDrac(HunterView h);
static int convergeOnLeader(HunterView h);
static int researchedBefore(HunterView h, LocationID *htrail);

//static int leader = PLAYER_LORD_GODALMING; // Global leader variable

/*
    Current Strategy:
        (1) Godalming starts in the centre of the map. Remaining players spread out LEFT, RIGHT, TOP.
        (2) Remaining players converge on Godalming, the leader for the first 6 turns.
        (3) If trail is picked up, all hunters converge on Drac
        (4) If 6 rounds pass and no trail is picked up, all hunters research.
        (5) Converge on Drac.
        (6) If dracTrail is lost, all hunters research immediately and converge on Drac.
        (7) Repeat step #6
*/

void decideHunterMove(HunterView gameState)
{  
    int player = whoAmI(gameState);         // store curr player
    int round = giveMeTheRound(gameState);  // store curr round

    LocationID dTrail[TRAIL_SIZE];          // get drac trail
    giveMeTheTrail(gameState,PLAYER_DRACULA,dTrail);

    LocationID hTrail[TRAIL_SIZE];          // get current hunter trail
    giveMeTheTrail(gameState,player,hTrail);

    /* CURRENT DRYRUN LOGS
    
    (1) Round 0 placement = successful
    (2) Round 1 = Researching (incorrect logic)
    (3) Round 2 = Converge on Godalming (fn not implemented)

    */

    printf("The current round is: %d\n", round);
    // Round 0 placement
    if (round == 0) {
        switch(player) {
            case PLAYER_LORD_GODALMING:
                submitID(G_START);
                break;
            case PLAYER_DR_SEWARD:
                submitID(S_START);
                break;
            case PLAYER_VAN_HELSING:
                submitID(V_START);
                break;
            default:
                submitID(M_START);
        }
    // First 6 turns, converge on Godalming
    } else if (round < 6 && (dTrail[0] == CITY_UNKNOWN) || (dTrail[0] == SEA_UNKNOWN)) {
        submitID(convergeOnLeader(gameState));
    // After initial 6 turns      
    } else if (round >= 6 && dTrail[0] != CITY_UNKNOWN && dTrail[0] != SEA_UNKNOWN) {
        if (dTrail[0] == hTrail[0]) {            // Stay in city if drac is here
            submitID(hTrail[0]);
        } else {                                 // Converge on drac
            submitID(convergeOnDrac(gameState));
        }
    } else if (round >= 6 && dTrail[5] != CITY_UNKNOWN && dTrail[5] != SEA_UNKNOWN) {
      submitID(convergeOnDrac(gameState));      
    // Dracula trail not found, all hunters research
    } else {
        submitID(hTrail[0]);
    }                  
}

// Returns LocationID of whereToGoNext to hunt drac
int convergeOnDrac(HunterView h) {

    // Use pathLength() to determine #turns away Dracula is
    // Use BFS to determine which cities he may currently be in (within the #turns/degrees
    // Close him off?

    return 0;
}

// Returns LocationID of whereToGoNext for initial 5 turns
int convergeOnLeader(HunterView h) {
  
    // For the first 5 turns OR until drac trail is found,
    // converge towards Godalming.
    // Make Goldaming move around Centre Zone meanwhile

    return 0;
}

               
LocationID searchForDrac(HunterView h) {

  //Once the trail is researched, and hunters are nearby
  //make a searching function to explore the nearby area to find dracula.
  
  return 0;
}
// BFS to find length of shortest path (# of turns to dest)
int pathLength(LocationID src, LocationID dest) {

    Map map = newMap();

    LocationID *path = malloc(map->nV * sizeof(LocationID));
    LocationID *visited = calloc(map->nV, sizeof(LocationID));
    Queue q = newQueue();
    QueueJoin(q,src);
    int isFound = 0;

    while (!QueueIsEmpty(q) && !isFound) {
        LocationID x = QueueLeave(q);
        visited[x] = 1;
        VList curr;
        LocationID y;
        for (curr = map->connections[x]; curr != NULL; curr = curr->next) {
            y = curr->v;
            path[y] = x;
            if (y == dest)
                isFound = 1;
                break;
            if (!visited[y])
                QueueJoin(q,y);
                visited[y] = 1;
        }
    }
    int length = 0;
    if (isFound) {
        LocationID v;
        for (v = dest; v != src; v = path[v])
            length++;
    }
    return length;
}

/*              
static int researchedBefore(HunterView h, LocationID *htrail) {
  int i = 0;
  for (i=0; i < TRAIL_SIZE; i++) {
    
  }
                 
}
*/
static void submitID(LocationID dest) {
  char currPlace[3];
  idToAbbrev(dest,currPlace);
  registerBestPlay(currPlace,"Hello");
}

static LocationID howToGetTo(LocationID dest, LocationID from, int player, Round round, int *pathLength) {

  
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