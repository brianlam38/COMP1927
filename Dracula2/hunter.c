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

#define G_START STRASBOURG // Godalming = Stratsbourg  (centre)
#define S_START GRANADA // Seward = Lisbon          (left)
#define V_START CONSTANTA // Van Helsing = Varna      (right)
#define M_START MANCHESTER // Mina Harker = Galway     (top)

static void submitID(LocationID dest, char *message);
static LocationID *whereToGo(int player,int *numLocations, int from,int sea, int stationsAllowed);
//static LocationID howToGetTo(HunterView h, LocationID dest, LocationID from, int player, int *findPathLength, int sea,int rail);
static int convergeOnDrac(HunterView h);
static int convergeOnLeader(HunterView h);
//static int researchedBefore(HunterView h, LocationID *htrail);
static int visitedDest(HunterView h, LocationID place, int pos);
static LocationID searchNearby(HunterView h, int player);
LocationID *whereDracWent(HunterView h, int *numLocations, int *offset);

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

        Need to also figure out how to tackle S?, since we don't do much when it occurs.
*/

void decideHunterMove(HunterView gameState)
{  

    srand(time(NULL));
    int player = whoAmI(gameState);         // store curr player
    int round = giveMeTheRound(gameState);  // store curr round
    int health = howHealthyIs(gameState,player);

    LocationID dTrail[TRAIL_SIZE];          // get drac trail
    giveMeTheTrail(gameState,PLAYER_DRACULA,dTrail);

    LocationID hTrail[TRAIL_SIZE];          // get current hunter trail
    giveMeTheTrail(gameState,player,hTrail);

    LocationID startLocations[4] = {G_START,S_START,V_START,M_START};

    int temp = 0;

    printf("The current round is: %d\n", round);
    /* Round 0 placement */
    if (round == 0) {
    
        submitID(startLocations[player],"Ready or not, here I come :D");  

    /* Low health, go to hospital */
    } else if (whereIs(gameState,player) == ST_JOSEPH_AND_ST_MARYS && hTrail[0] != ST_JOSEPH_AND_ST_MARYS) {
        submitID (howToGetTo(hTrail[0],ST_JOSEPH_AND_ST_MARYS,round,player,&temp,0,1), "The comeback is real");
    /* First 6 turns, converge on GODALMING */
    } else if (round < 6) {
        int i;
        int isFound = 0;
        //int inSea = 0;
        for (i = 0; i < (TRAIL_SIZE); i++) {
            if ((dTrail[i] >= MAX_MAP_LOCATION || dTrail[i] < 0) && dTrail[i] != TELEPORT)
                continue;
            else {
                isFound = 1;
                break;  
            }         
        }
        if (isFound == 0)
            submitID(convergeOnLeader(gameState), "Converging on LEADER");
        else
            submitID(convergeOnDrac(gameState), "Converging on DRAC");
    /* After initial 6 turns */ 
    } else if (round >= 6) {
        if (dTrail[0] == hTrail[0]) {
            submitID(hTrail[0], "Dracula is here, I am staying!");
        } else {
            int i;
            int isFound = 0;
            for (i = 0; i < (TRAIL_SIZE); i++) {
                if (dTrail[i] > MAX_MAP_LOCATION && dTrail[i] != TELEPORT)
                    continue;
                else {
                    isFound = 1;
                    break;
                }
            }
            if (isFound == 0) {
                //if (round % 4 == 0)
                //    submitID(hTrail[0], "Researching!");
                int steps = 0;
                if (health <= 2)
                    submitID(hTrail[0],"Gotta heal up");
                else if (!visitedDest(gameState,startLocations[player],TRAIL_SIZE))
                    submitID(howToGetTo(startLocations[player],hTrail[0],round,player, &steps,0,1), "Can't find Drac, going home");
                else if (!visitedDest(gameState,ROME,TRAIL_SIZE))
                    submitID(howToGetTo(ROME,hTrail[0],round,player, &steps,0,1), "Can't find Drac, probs in that loop");
                else
                    submitID(searchNearby(gameState, player), "Searching Nearby");
            } else {
                if (dTrail[i] > MAX_MAP_LOCATION && dTrail[i] != TELEPORT) {
                      submitID(hTrail[0], "Temporary Fix");
                } else
                    submitID(convergeOnDrac(gameState), "Converging on DRAC");
            }
        }
    }               
}

// Returns LocationID of whereToGoNext to hunt drac
LocationID convergeOnDrac(HunterView h) {

    clock_t start = clock();

    int round = giveMeTheRound(h);
    int player = whoAmI(h); // get player
    //Round round = giveMeTheRound(h);                      // unused var ATM
    LocationID dTrail[TRAIL_SIZE];                          // get drac trail
    giveMeTheTrail(h,PLAYER_DRACULA,dTrail);
    int i; //numSeas = 0;
  
  
    for (i = 0; i < TRAIL_SIZE; i++) {                      // Iterate through trail #
        if ((dTrail[i] >= 0 && dTrail[i] <= MAX_MAP_LOCATION) || dTrail[i] == TELEPORT)    // Trail found, exit loop
            break;
        //if (dTrail[i] == SEA_UNKNOWN) numSeas++;      
    }

    if (dTrail[i] == TELEPORT) dTrail[i] = CASTLE_DRACULA;

    printf("Converge on Dracula at %d function! start = %lu\n",dTrail[i],start);
    if (dTrail[i] >= 0 && dTrail[i] <= MAX_MAP_LOCATION) {
        int myNums = 0;

        LocationID dest = howToGetTo(dTrail[i],whereIs(h,player),giveMeTheRound(h),player,&myNums,1,1); //using temp, since we don't actually need the length

        printf("The howToGetTo took %lu time\n",clock()-start);

        if (i == 0) {

            if (howHealthyIs(h,player) <= 2 && !visitedDest(h,dest,1))
                return whereIs(h,player); //We found Dracula!
            else 
                return dest;

        } else if (dTrail[i] >= MIN_MAP_LOCATION && dTrail[i] <= MAX_MAP_LOCATION) {
            
            srand(time(NULL));
           // LocationID inCase = dTrail[i];
            int numLocations = 0;       
            int offset = 0;
            int stepsAway = 10;

            LocationID *dracsChoices = whereDracWent(h,&numLocations,&offset);

            if (numLocations == 1 && dracsChoices[0] != -1) {
                dest = howToGetTo(dracsChoices[0],whereIs(h,player),round,player,&stepsAway,0,1);
                free(dracsChoices);
                return dest;
            } else if (numLocations == 2 && dracsChoices[player%2] != -1) {

                dest = howToGetTo(dracsChoices[player%2],whereIs(h,player),round,player,&stepsAway,0,1);
                free(dracsChoices);
                return dest;
            } else if (numLocations == 3 && dracsChoices[player%3] != -1) {
                dest = howToGetTo(dracsChoices[player%3],whereIs(h,player),round,player,&stepsAway,0,1);
                free(dracsChoices);
                return dest;
            } else if (numLocations == 4 && dracsChoices[player%4] != -1) {
                dest = howToGetTo(dracsChoices[player%4],whereIs(h,player),round,player,&stepsAway,0,1);
                free(dracsChoices);
                return dest;

            }


            dest = findMostCommon(dracsChoices+offset,numLocations - offset);
            
            if (dest == -1)
                dest = findMostCommon(dracsChoices,numLocations);


            if (dest == -1 || visitedDest(h,dest,3)) {

                for (i = 0; dest == -1 && (i <= numLocations/2 || i < 5) && numLocations > 0; i++) {
                                                //numLocations/2 is used cause it seems decent

                     dest = dracsChoices[rand()%numLocations];
                     if (dest != -1)
                        dest = howToGetTo(dest,whereIs(h,player),round,player,&stepsAway,0,1);
                }
            }

            if (dest == -1) dest = dracsChoices[numLocations-1];
            if (dest == -1) dest = dracsChoices[0];            

            free (dracsChoices);

            if (dest != -1)
                dest = howToGetTo(dest,whereIs(h,player),giveMeTheRound(h),player,&stepsAway,0,1);


            if (dest != -1 && (dest == whereIs(h,(player+1)%4) || 
                    dest == whereIs(h,(player+1)%4) || dest == whereIs(h,(player+1)%4))) {
                    return searchNearby(h,player);
       }

            if (dest != -1)
                return dest;
            else { 
                return searchNearby(h, player);

            }
        }
      
      
    }

    //This function will return the same place the hunter is, if the hunter is already there
    //To actually find Drac, add stuff to the function, so the hunters search around 'dest'
    
    // Use pathLength() to determine #turns away Dracula is
    // Use BFS to determine which cities he may currently be in (within the #turns/degrees
    // Close him off?
     //just in case this function was called in the wrong
    return searchNearby(h, player);                              //situation, it calls searchNearby()
}

// Returns LocationID of whereToGoNext for initial 5 turns
LocationID convergeOnLeader(HunterView h) {
    assert(giveMeTheRound(h) < 6);

    clock_t start = clock();

    printf("Converge on leader function! start = %lu\n",start)
;
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
    int i = 0, j = 0; //int j = 0;   // uninitialised
    Round round = giveMeTheRound(h);
    //Don't want to wander around the sea
    LocationID *placesToGo = whereCanIgo(h,&numPlaces,1,0,1);  // stores next possible location to go
    LocationID inCase = -1;                                    // value for no location found

    if (round < 6 && player == PLAYER_LORD_GODALMING) {
        for (j = TRAIL_SIZE; j > 0; j--) { 
            for (i = 0; i < numPlaces; i++) {                        // loop through possible places
                int visited = visitedDest(h,placesToGo[i],j);

                if (!visited) {                                   // if not visited and am Godalming    
                    inCase = placesToGo[i];                                 // patrol around STRASBOURG
                    int size = 0;
                    LocationID *placesNearDest = whereToGo(player,&size,placesToGo[i],0,(round+1)%4);
                    if (inArray(placesNearDest,STRASBOURG,size)) {
                        free(placesNearDest);
                        int dest = placesToGo[i];
                        free(placesToGo);
                        return dest;
                    }
                    free(placesNearDest);
                }
            }            
        }

    } else {
        for (j = TRAIL_SIZE; j > 0; j--) { 
            for (i = 0; i < numPlaces; i++) {                        // loop through possible places
                int visited = visitedDest(h,placesToGo[i],j);
                if (!visited) {
                    int dest = placesToGo[i];       // if not visited and !Godalming
                    free(placesToGo);
                    return dest;
                }                                   // return location
            }
        }            
    }
    //Random trawling to places unvisited, if there is nothing to do         
    //Once the trail is researched, and hunters are nearby
    //make a searching function to explore the nearby area to find dracula.
  
    //returns -1 if unable to find anywhere unvisited
    //returns an unvisited place not next to STRATSBOURG otherwise
    printf("Time taken for search = %lu\n",(clock()-start));
    if (numPlaces > 0) {
        inCase = placesToGo[numPlaces-1];
    } else {
        inCase = placesToGo[0];
    }
    free(placesToGo);
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

// Not sure how useful this function is yet
char hMessage(HunterView h) {

    // Figure out how to chain together multiple statements,
    // for more informative messages?

    char *messageStorage = calloc(MESSAGE_SIZE, sizeof(char));  // allocate message array
    Round round = giveMeTheRound(h);
    
    if (round == 0) { strcpy(messageStorage,"HUNTER START"); }  // sample message

    return *messageStorage;                                     // return message
}
//DO NOT call this function if a hunter is at drac's location or drac is at
//CD, cause its pointless and it'll probs break the function
//Function returns list of dracs possible locations.
//Those Locations between offset and numLocations are most likely, if they exist
LocationID *whereDracWent(HunterView h, int *numLocations, int *offset) {

    int i,j,k;
    LocationID h1[TRAIL_SIZE], h2[TRAIL_SIZE], h3[TRAIL_SIZE], h4[TRAIL_SIZE];
    LocationID dTrail[TRAIL_SIZE];

    giveMeTheTrail(h,0,h1); giveMeTheTrail(h,1,h2);
    giveMeTheTrail(h,2,h3); giveMeTheTrail(h,3,h4);

    giveMeTheTrail(h,4,dTrail);
 
//Finding hides and double backs in the trail

    int DBtype = 0;
    int DBturn = 0;

    int lastLoc = -1;
    int turnsBefore = 0;

    int HIturn = -1;

//Creating or modifying the trail so its easier to figure things out

    for (i = 0; i < TRAIL_SIZE; i++) {
        if ((dTrail[i] >= 0 && dTrail[i] <= MAX_MAP_LOCATION) || dTrail[i] == TELEPORT) {
            lastLoc = dTrail[i]; turnsBefore = i; break;

        } else if (dTrail[i] >= DOUBLE_BACK_1 && dTrail[i] <= DOUBLE_BACK_5) {
            DBtype = dTrail[i] + 1 - DOUBLE_BACK_1; DBturn = i;
        } else if (dTrail[i] == HIDE) {
            HIturn = i;
        }

    }
    if (dTrail[i] == TELEPORT) {
        dTrail[i] = CASTLE_DRACULA;
        lastLoc = CASTLE_DRACULA;
    }


    //Modify the trail if there is a hide there
    if (HIturn >= 0) {
        shiftLeft(dTrail,HIturn,TRAIL_SIZE-1);
    }


    //Modify the trail to suit double backs

    if (DBtype > 0 && lastLoc != -1) {
        if (DBtype+DBturn < TRAIL_SIZE)
            dTrail[DBturn] = dTrail[DBturn+DBtype];
        else 
            dTrail[DBturn] = UNKNOWN_LOCATION-1;
    }

    for (i = 0; i < TRAIL_SIZE; i++) {
        if (dTrail[i] >= 0 && dTrail[i] <= MAX_MAP_LOCATION) {
            lastLoc = dTrail[i]; turnsBefore = i; break;
        }
    }

    if (dTrail[0] >= MIN_MAP_LOCATION && dTrail[0] <= MAX_MAP_LOCATION) {
        LocationID *potentialLoc = malloc(sizeof(LocationID));
        potentialLoc[0] = dTrail[0];
        *numLocations = 1;
        *offset = 0;
        return potentialLoc;
    }
//Now starting to navigate the map to find potential Locs for drac
    Map map = newBasicMap();

    LocationID *potentialLoc = malloc(sizeof(LocationID));
    potentialLoc = NearbyCities(map,lastLoc,potentialLoc,numLocations, ROAD);
    potentialLoc = NearbyCities(map,lastLoc,potentialLoc,numLocations, BOAT);   

    int numNearby = *numLocations;

 
    for (i = turnsBefore-1; i >= 0; i--) {

//If the location was visited by a hunter, after drac has been there, it would be
//Already in dTrail, so they can be -1. Also don't add locs alreay in the trail

        for (j = numNearby-1; j >= *offset; j--) {
            if (inArray(h1,potentialLoc[j],i) >= 0 ||
                inArray(h2,potentialLoc[j],i) >= 0 ||
                inArray(h3,potentialLoc[j],i) >= 0 ||
                inArray(h4,potentialLoc[j],i) >= 0 ||
                inArray(potentialLoc,potentialLoc[j],*offset) >= 0) {
//                printf("Location in array is %d, numNearby = %d j = %d offset = %d\n",potentialLoc[j],numNearby, j, *offset);
                potentialLoc[j] = -1;
            }
        }

//Adding locs to the array and filtering them
        for (k = *offset; k < numNearby; k++) {


            if (potentialLoc[k] != -1 && i != 0) {

                if ((dTrail[i] == CITY_UNKNOWN || idToType(dTrail[i]) == LAND) //In a city, then move to city
                                                && dTrail[i-1] == CITY_UNKNOWN)
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,ROAD);

                else if (dTrail[i] == SEA_UNKNOWN || idToType(dTrail[i]) == SEA) //in sea then move anywhere
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,BOAT);

                else if ((dTrail[i] == CITY_UNKNOWN || idToType(dTrail[i]) == LAND) //on land then move to city
                                                && dTrail[i-1] == SEA_UNKNOWN)
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,BOAT);

                else if (dTrail[i] == UNKNOWN_LOCATION-1 &&         //Unknown location to a city
                        (dTrail[i-1] == CITY_UNKNOWN || dTrail[i-1] == UNKNOWN_LOCATION-1)) {

                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,BOAT);
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,ROAD);

                }  else if (dTrail[i] == UNKNOWN_LOCATION-1 && dTrail[i-1] == SEA_UNKNOWN) {//unknown Loc to sea
 
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,BOAT);
 
                } else if ((dTrail[i] == CITY_UNKNOWN || idToType(dTrail[i]) == LAND) && //city to unknown loc
                                                        dTrail[i-1] == UNKNOWN_LOCATION-1) {

                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,BOAT);
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,ROAD);  
                }

                if (dTrail[i-1] == CITY_UNKNOWN && ((dTrail[i] == SEA_UNKNOWN) || //sea to city
                                                        idToType(dTrail[i]) == SEA)) {
                    for (j = numNearby; j < *numLocations; j++) {
                        if (idToType(potentialLoc[j]) == SEA)
                            potentialLoc[j] = -1;
                    }
                } else if (dTrail[i-1] == SEA_UNKNOWN && ((dTrail[i] == SEA_UNKNOWN) ||//sea to sea
                                                        idToType(dTrail[i]) == SEA)) {
                    for (j = numNearby; j < *numLocations; j++) {
                        if (idToType(potentialLoc[j]) == LAND)
                            potentialLoc[j] = -1;
                    }
                }


            } else if (potentialLoc[k] != -1) {
                //Finding the locations nearby
                if (dTrail[0] == CITY_UNKNOWN && dTrail[1] == CITY_UNKNOWN)
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,ROAD);
                else if (dTrail[0] == CITY_UNKNOWN && dTrail[1] == SEA_UNKNOWN)
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,BOAT);
                else if (dTrail[0] == SEA_UNKNOWN)
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,BOAT);
                else if (dTrail[0] == UNKNOWN_LOCATION-1) {
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,BOAT);
                    potentialLoc = NearbyCities(map,potentialLoc[k],potentialLoc,numLocations,ROAD);
                }

            }
                //get rid of redundant locs in the trail
            if (dTrail[0] == SEA_UNKNOWN) { 
                for (j = numNearby; j <*numLocations; j++) {
                    if (idToType(potentialLoc[j]) == LAND)
                        potentialLoc[j] = -1;
                }

            } else if (dTrail[0] == CITY_UNKNOWN) {
                 for (j = numNearby; j <*numLocations; j++) {
                    if (idToType(potentialLoc[j]) == SEA)
                        potentialLoc[j] = -1;
                }
                                            
            }
//          printf("k = %d Loc = %d\n",k,potentialLoc[k+1]);
        }
        *offset = numNearby;
        numNearby = *numLocations;
        
    }
    //Returns big array, most likely places are going to be at the end,

    return potentialLoc;
}

