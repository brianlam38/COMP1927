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

void returnLastMessage(HunterView h, int player, char *message);

*/

// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Game.h"
#include "HunterView.h"
#include "Places.h"
#include "commonFunctions.h"

#include <time.h>

#define G_START 60 // Godalming = Stratsbourg  (centre)
#define S_START 37 // Seward = Lisbon          (left)
#define V_START 66 // Van Helsing = Varna      (right)
#define M_START 27 // Mina Harker = Galway     (top)

static void submitID(LocationID dest, char *message);
static LocationID *whereToGo(int player,int *numLocations, int from,int sea, int stationsAllowed);
//static LocationID howToGetTo(HunterView h, LocationID dest, LocationID from, int player, int *findPathLength, int sea,int rail);
static int convergeOnDrac(HunterView h);
static int convergeOnLeader(HunterView h);
//static int researchedBefore(HunterView h, LocationID *htrail);
static int visitedDest(HunterView h, LocationID place, int pos);
static LocationID searchNearby(HunterView h, int player);
LocationID *whereDracWent(HunterView h, int *numLocations, int *offset);
//static LocationID hideAndSeek(HunterView h);
//static LocationID DBSeek(HunterView h);

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
    /* Grab gameState data */
    srand(time(NULL));
    int player = whoAmI(gameState);
    int round = giveMeTheRound(gameState); 

    LocationID dTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState,PLAYER_DRACULA,dTrail);

    LocationID hTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState,player,hTrail);

    int temp = 0;

    /* Round 0 placement */
    if (round == 0) {
        switch(player) {
            case PLAYER_LORD_GODALMING:
                submitID(G_START, "GODALMING START");
                break;
            case PLAYER_DR_SEWARD:
                submitID(S_START, "SEWARD START");
                break;
            case PLAYER_VAN_HELSING:
                submitID(V_START, "HELSING START");
                break;
            default:
                submitID(M_START, "MINA START");
        }

    /* Send player to hospital */
    } else if (whereIs(gameState,player) == ST_JOSEPH_AND_ST_MARYS && hTrail[0] != ST_JOSEPH_AND_ST_MARYS) {
        submitID (howToGetTo(hTrail[0],ST_JOSEPH_AND_ST_MARYS,round,player,&temp,0,1), "The comeback is real");
        return;

    /* First 5 rounds, converge on Godalming */
    } else if (round < 6) {
        if (dTrail[0] == hTrail[0]) {
            submitID(hTrail[0], "Dracula is here, I am staying!");
        } else {
            int i;
            int isFound = 0;
            for (i = 0; i < (TRAIL_SIZE); i++) {
                if (dTrail[i] > MAX_MAP_LOCATION) // No location found
                    continue;
                else                              // Location is found, stop loop                                       
                    isFound = 1;                  
                    break;           
            }
            if (isFound == 0)                                     
                submitID(convergeOnLeader(gameState), "Converging on LEADER");
            else                                              
                submitID(convergeOnDrac(gameState), "Converging on DRAC");   
        }

    /* After 5 rounds, research or converge */
    } else if (round >= 6) {
        if (dTrail[0] == hTrail[0]) {                                       // Drac is here
            submitID(hTrail[0], "Dracula is here, I am staying!");
        } else {
            int i;
            int isFound = 0;
            for (i = 0; i < (TRAIL_SIZE); i++) {
                if (dTrail[i] == CITY_UNKNOWN || dTrail[i] == SEA_UNKNOWN)  // Drac is unknown
                    continue;
                else
                    isFound = 1;                                            // Drac is known or HI/DB
                    break;                                                
            }
            if (isFound == 0)
                submitID(hTrail[0], "Researching!");
            else {
                switch(dTrail[i]) {
                    case HIDE:
                        submitID(convergeOnDrac(gameState), "Dracula is HIDING");
                        break;
                    case DOUBLE_BACK_1:
                        submitID(convergeOnDrac(gameState), "Dracula did a DB1");
                        break;
                    case DOUBLE_BACK_2:
                        submitID(convergeOnDrac(gameState), "Dracula did a DB2");
                        break;
                    case DOUBLE_BACK_3:
                        submitID(convergeOnDrac(gameState), "Dracula did a DB3");
                        break;
                    case DOUBLE_BACK_4:
                        submitID(convergeOnDrac(gameState), "Dracula did a DB4");
                        break;
                    case DOUBLE_BACK_5:
                        submitID(convergeOnDrac(gameState), "Dracula did a DB5");
                        break;
                default:
                    submitID(convergeOnDrac(gameState), "Converging on DRAC");
                }
            }
        }
    }               
}
/* ### LOGIC FOR HIDE ### */
// Returns location of where drac may be from a HIDE

//LocationID hideAndSeek(HunterView h) {

    // (1) # of degrees away = trail# - 1
    // (2) search for all possible locations X degrees away
    // (3) go to the locations

    /* Grabs dracula trail */
//    LocationID dTrail[TRAIL_SIZE];
//    giveMeTheTrail(h,PLAYER_DRACULA,dTrail);

    /* Grabs position of location before hide (The true location) */
//    int i;
//    for (i = 0; i < TRAIL_SIZE; i++) {
//        if (dTrail[i] == HIDE) {
//            break;
//        }         
//    }
    // HI move = stay in same location.
    // Hunter lands on city where dTrail[3] == HI
    // This means that dTrail[4] location will reveal

    /* Determines what to do */
//    int player = whoAmI(h);   
//    LocationID trueLoc;
//    trueLoc = howToGetTo(dTrail[i+1],whereIs(h,player),giveMeTheRound(h),player,&i,1,1); 

//    return trueLoc;
//}

/* ### LOGIC FOR DB ### */ 
// Returns location of where drac may be from a DOUBLE BACK
/*
LocationID DBSeek(HunterView h) {
    insert code here
}
*/

// Returns LocationID of whereToGoNext to hunt drac
LocationID convergeOnDrac(HunterView h) {

    clock_t start = clock();
    printf("Converge on Dracula at %d function! start = %lu\n",whereIs(h,PLAYER_DRACULA),start);


    int player = whoAmI(h); // get player
    //Round round = giveMeTheRound(h);                      // unused var ATM
    LocationID dTrail[TRAIL_SIZE];                          // get drac trail
    giveMeTheTrail(h,PLAYER_DRACULA,dTrail);
    int i;
  
    for (i = 0; i < TRAIL_SIZE; i++) {                      // Iterate through trail #
        if (dTrail[i] >= 0 && dTrail[i] <= MAX_MAP_LOCATION)    // Trail found, exit loop
            break;      
    }

    if (dTrail[i] >= 0 && dTrail[i] <= MAX_MAP_LOCATION) {
        int myNums = 0;

        LocationID dest = howToGetTo(dTrail[i],whereIs(h,player),giveMeTheRound(h),player,&myNums,1,1); //using temp, since we don't actually need the length

    printf("The howToGetTo took %lu time\n",clock()-start);
        if (i == 0) {
             return dest; //We found Dracula!

/*       } else if (dest == dTrail[i]) {                 //If dracula was here (place already visited)                            

                int dracNums = 0;
                LocationID *myChoices = whereCanIgo(h,&myNums,1,1,0); //assumes drac doesn't travel by sea
                LocationID *dracsChoices = whereToGo(PLAYER_DRACULA,&dracNums,dTrail[i],0,0);
                int foundPlace = 0; 
              
                int max = (myNums <= dracNums) ? myNums : dracNums; //max is the lowest of the two nums

                for (i = 6; i > 2; i--) { //check visited until trail[0,1,2]
                    for (j = 0; j < max; j++) {
                        if (dracsChoices[j] == dest) continue; //not wasting time on something I know
                        if (inArray(myChoices,dracsChoices[j],myNums) && !visitedDest(h, dracsChoices[j], i)) {
                            foundPlace = 1; break;
                        }
                    }
                    if (foundPlace) break;
                }

                foundPlace = (foundPlace) ? dracsChoices[j] : dest;
                //If a place is found return that, else return dest
                free (dracsChoices);
                free(myChoices);
                return foundPlace;
*/
        } else {
//          printf("Finding dracs choices...\n");
            int numLocations = 0;       
            int offset = 0;
            LocationID *dracsChoices = whereDracWent(h,&numLocations,&offset);
//          printf("found his choices\n");
            dest = findMostCommon(dracsChoices,numLocations);
            if (dest == -1) dest = dracsChoices[numLocations-1];            
            free (dracsChoices);

        return howToGetTo(dest,whereIs(h,player),giveMeTheRound(h),player,&i,0,1);
    }
      
      
    }

    //This function will return the same place the hunter is, if the hunter is already there
    //To actually find Drac, add stuff to the function, so the hunters search around 'dest'
    
    // Use pathLength() to determine #turns away Dracula is
    // Use BFS to determine which cities he may currently be in (within the #turns/degrees
    // Close him off?

    int res = searchNearby(h, player);
    if (res == -1) return whereIs(h,player); //just in case this function was called in the wrong
    return res;                              //situation, it calls searchNearby()
}

// Returns LocationID of whereToGoNext for initial 5 turns
LocationID convergeOnLeader(HunterView h) {
    assert(giveMeTheRound(h) < 6);

    clock_t start = clock();

    printf("Converge on leader function! start = %lu\n",start);
    int i = 0;
    //int j = 0;     // unused variable
    int player = whoAmI(h);                          // get player ID
    LocationID curr = whereIs(h,player);             // get location of player
    Round round = giveMeTheRound(h);
  
    if (player == PLAYER_VAN_HELSING) {              // Helsing's First five rounds
        switch (round) {                             // Go through Castle Drac
            case (1): return CONSTANTA;      break;
            case (2): return GALATZ;         break;
            case (3): return CASTLE_DRACULA; break;
            case (4): return KLAUSENBURG;    break;
            case (5): return BUDAPEST;       break;
        }
    }

    if (player != PLAYER_LORD_GODALMING) {
        LocationID dest = howToGetTo(whereIs(h,PLAYER_LORD_GODALMING),curr,
                        giveMeTheRound(h),player,&i,1,1);
    printf("Time taken for howToGetTo %d is %lu\n",whereIs(h,PLAYER_LORD_GODALMING),clock()-start);    // find how to reach Godall
        if (!visitedDest(h,dest,5)) return dest;                            // if location not visited, return dest
    }

    LocationID dest = searchNearby(h,player);       /* FOR GODALMING */
    if (dest == -1) return curr;                    // if dest is unknown, stay in same location
    else return dest;                               // else return nearby location

    //returns -1 if unable to find somewhere to go
    //Random trawling to places unvisited, if there is nothing to do        
}
  
// Search nearby for unvisited locations (ADJ TO STRASBOURG), returns -1 if nowhere to go         
LocationID searchNearby(HunterView h, int player) {

    clock_t start = clock();     
        printf("Search Nearby function! start = %lu\n",start);
    int numPlaces = 0;
    int i = 0; //int j = 0;   // uninitialised
    Round round = giveMeTheRound(h);
    //Don't want to wander around the sea
    LocationID *placesToGo = whereCanIgo(h,&numPlaces,1,0,1);  // stores next possible location to go
    LocationID inCase = -1;                                    // value for no location found

  
    for (i = 0; i < numPlaces; i++) {                          // loop through possible places
        int visited = visitedDest(h,placesToGo[i],4);
        if (!visited && player != PLAYER_LORD_GODALMING) {       // if not visited and !Godalming
 
        return placesToGo[i];
    }                                   // return location
        else if (!visited) {                                   // if not visited and am Godalming    
            inCase = placesToGo[i];                                 // patrol around STRASBOURG
            int size = 0;
            LocationID *placesNearDest = whereToGo(player,&size,placesToGo[i],0,(round+1)%4);
            if (inArray(placesNearDest,STRASBOURG,size) != -1) return placesToGo[i];
            free(placesNearDest);
        }            
    }
    //Random trawling to places unvisited, if there is nothing to do         
    //Once the trail is researched, and hunters are nearby
    //make a searching function to explore the nearby area to find dracula.
  
    //returns -1 if unable to find anywhere unvisited
    //returns an unvisited place not next to STRATSBOURG otherwise
    printf("Time taken in Case = %lu\n",(clock()-start));
    return inCase;
}

// Determines if location has been visited in the last 'pos' turns
//e.g pos = 3, checks if place has been visited by anyone in the last
//3 rounds
int visitedDest(HunterView h, LocationID place, int pos) {
  
    LocationID h1Trail[TRAIL_SIZE];
    LocationID h2Trail[TRAIL_SIZE];
    LocationID h3Trail[TRAIL_SIZE];
    LocationID h4Trail[TRAIL_SIZE];
  
    int player = whoAmI(h);
    giveMeTheTrail(h,player,h1Trail);
    giveMeTheTrail(h,(player+1)%4,h2Trail);
    giveMeTheTrail(h,(player+2)%4,h3Trail);
    giveMeTheTrail(h,(player+3)%4,h4Trail);
  
    int i;

    for (i = 0; i < pos; i++) { //return if in last 'pos' locations
        if (place == h1Trail[i]) return 1;
        if (place == h2Trail[i]) return 1;
        if (place == h3Trail[i]) return 1;
        if (place == h4Trail[i]) return 1;
    }
    return 0;      
}

/*              
static int researchedBefore(HunterView h, LocationID *htrail) {
  int i = 0;
  for (i=0; i < TRAIL_SIZE; i++) {
    
  }
                 
}
*/

// ID to Abbrev translator + makes best play
static void submitID(LocationID dest, char *message) {
    char currPlace[3];
    idToAbbrev(dest,currPlace);
    registerBestPlay(currPlace,message);
}

// Which adjacent cities can the Hunter go next?
static LocationID *whereToGo(int player,int *numLocations, int from, int sea, int stationsAllowed) {

    if (from == UNKNOWN_LOCATION) return NULL;
    *numLocations = 1;        //initialise the array size
    LocationID *connections = malloc((*numLocations) * sizeof(LocationID));

    connections[0] = from;    //initialise the array
    Map map;
    if (stationsAllowed > 0) map = newRailMap();
    else map = newBasicMap();       //get the game map
    int i;
    
    //find the nearby cities of type ROAD
    connections = NearbyCities(map, from, connections, numLocations, ROAD);
    
    
    //find the nearby cities of type BOAT
    if (sea)
    connections = NearbyCities(map, from, connections, numLocations, BOAT);

    //find rail connections
    if (stationsAllowed == 0) return connections;

        int nearbyStations = 0;
        LocationID *railConnections = malloc(sizeof(LocationID));
        railConnections = NearbyCities(map,from,railConnections,&nearbyStations,RAIL);

        int priStationsFound = nearbyStations;
        if (stationsAllowed > 1) {
            for (i = 0; i < priStationsFound; i++) {
                railConnections = NearbyCities(map,railConnections[i],railConnections,&nearbyStations,RAIL);
            }
        }
        if (stationsAllowed == 3) {
            int triStationsFound = nearbyStations;
            for (i = priStationsFound; i < triStationsFound; i++) {
                railConnections = NearbyCities(map,railConnections[i],railConnections,&nearbyStations,RAIL);              
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

//DO NOT call this function if a hunter is at drac's location or drac is at
//CD, cause its pointless and it'll probs break the function
//Also don't call it if there is nothing in drac's trail or drac was at sea after
//being at the destination he was last seen at
LocationID *whereDracWent(HunterView h, int *numLocations, int *offset) {

    int i,j,k;
    LocationID h1[TRAIL_SIZE], h2[TRAIL_SIZE], h3[TRAIL_SIZE], h4[TRAIL_SIZE];
    LocationID dTrail[TRAIL_SIZE];

    giveMeTheTrail(h,0,h1); giveMeTheTrail(h,1,h2);
    giveMeTheTrail(h,2,h3); giveMeTheTrail(h,3,h4);

    giveMeTheTrail(h,4,dTrail);
    int DBtype = 0;
    int DBturn = 0;

    int lastLoc = -1;
    int turnsBefore = 0;

    for (i = 0; i < TRAIL_SIZE; i++) {
        if (dTrail[i] >= 0 && dTrail[i] <= MAX_MAP_LOCATION) {
            lastLoc = dTrail[i]; turnsBefore = i; break;

        } else if (dTrail[i] >= DOUBLE_BACK_1 && dTrail[i] <= DOUBLE_BACK_5) {
            DBtype = dTrail[i] + 1 - DOUBLE_BACK_1; DBturn = i;
        }
        
    }

    if (DBturn + DBtype <= turnsBefore) turnsBefore -= DBturn - DBtype;

    else {
        for (i = turnsBefore; i < TRAIL_SIZE; i++) {
            if (dTrail[i] >= 0 && dTrail[i] <= MAX_MAP_LOCATION) {
                lastLoc = dTrail[i];
                if (DBturn + DBtype > i) turnsBefore = 2*DBturn+DBtype-i; //calculates how many turns from prev Loc.
                else turnsBefore = i - DBturn-DBtype;
            }
        }
    }
    
//  printf("About to enter intensive part of function ;-;\n");
    Map map = newBasicMap();
//  int numLocations = 0;
    LocationID *potentialLoc = malloc(sizeof(LocationID));
    potentialLoc = NearbyCities(map,lastLoc,potentialLoc,numLocations, ROAD);
//  printf("Nearly there ;-;\n");
//  int offset = 0;
    int numNearby = *numLocations;

    for (i = 0; i < turnsBefore; i++) {
//      printf("Inside numLocations = %d i = %d\n",*numLocations,i);
        for (j = 0; j < *numLocations; j++) {
            if (inArray(h1+turnsBefore-i,potentialLoc[i],TRAIL_SIZE-turnsBefore+i) ||
                inArray(h2+turnsBefore-i,potentialLoc[i],TRAIL_SIZE-turnsBefore+i) ||
                inArray(h3+turnsBefore-i,potentialLoc[i],TRAIL_SIZE-turnsBefore+i) ||
                inArray(h4+turnsBefore-i,potentialLoc[i],TRAIL_SIZE-turnsBefore+i))
                potentialLoc[i] = -1;
        }
//      printf("Half the function done offset = %d; numNearby = %d\n",*offset,numNearby);

        for (k = *offset; k < numNearby; k++) {
            if (potentialLoc[k] != -1)
            potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,ROAD);
//          printf("k = %d Loc = %d\n",k,potentialLoc[k+1]);
        }
        *offset = numNearby;
        numNearby = *numLocations;
        
    }
    //Returns big array, most likely places are going to be at the end,
    return potentialLoc;
}