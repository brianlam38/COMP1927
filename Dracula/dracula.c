// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Game.h"
#include "DracView.h"
#include "Places.h"
#include "Map.h"
#include "commonFunctions.h"

//char *furthestLocation(DracView gameState, LocationID *adjLocations,
//                                  int numLoc, PlayerID player);

// check if there is an isolated hunter
// a hunter is said to be isolated if he is in a location that is
//  unattainable by another hunter in 2 rounds
//int isIsolated(PlayerID player, DracView gameState);

//int islandEmpty(DracView gameState);

//int atLeastOneIsolated(char **location, DracView gameState); // sets location
//adjacent to most isolated player  in a positionn furthest from other players

//LocationID optimalStartLocation(DracView gameState);
//
//PlayerID mostIsolatedPlayer(DracView gameState);

char *optimalLocation(DracView gameState, LocationID *adjLocations,
                      int numLoc, PlayerID player);




int locationScore(LocationID location, int length, int health);
int possibleHunters(DracView gameState, LocationID place);
char *furthestLocation(DracView gameState, LocationID *adjLocations,
                       int numLoc);
int islandEmpty(DracView gameState);
PlayerID mostIsolated(DracView gameState);
void locToDBOrHI(char *locAbbrev, DracView currentView);


void decideDraculaMove(DracView gameState)
{
//    char *location;
//    LocationID loc;
    LocationID trail[TRAIL_SIZE];
LocationID trail2[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, trail, FALSE);
    int y;
    for (y = 0 ; y < TRAIL_SIZE ; y++) {
        printf("trail[%d] = %d\n", y, trail[y]);
    }
giveMeTheTrail(gameState, PLAYER_DRACULA, trail2, TRUE);
    for (y = 0 ; y < TRAIL_SIZE ; y++) {
        printf("hideTrail[%d] = %d\n", y, trail2[y]);
    }

    char *locAbbrev;    
    LocationID *adjLoc = NULL;
    int numLoc = 0;
    LocationID x, nextLoc;
    Round round = giveMeTheRound(gameState);
    
    if (round == 0) {
        PlayerID mostIsolatedHunter = mostIsolated(gameState);
        if (islandEmpty(gameState)) {
            registerBestPlay("ED", "first move");
        } else if (mostIsolatedHunter != -1) {
            LocationID from = whereIs(gameState, mostIsolatedHunter);
            for (x = MIN_MAP_LOCATION; x < NUM_MAP_LOCATIONS; x++) {
                if (findPathLength(from, x, mostIsolatedHunter, 1, &nextLoc) == 2) {
                    numLoc++;
                    if (adjLoc != NULL) {
                        adjLoc = realloc(adjLoc, numLoc * sizeof(LocationID));
                        assert(adjLoc != NULL);
                        adjLoc[numLoc - 1] = x;
                    } else {
                        adjLoc = malloc(numLoc * sizeof(LocationID));
                        assert(adjLoc != NULL);
                        adjLoc[numLoc - 1] = x;
                    }
                }
            }
            locAbbrev = furthestLocation(gameState, adjLoc, numLoc);
            registerBestPlay(locAbbrev, "first move");
            free(adjLoc);
        } else {
            numLoc = NUM_MAP_LOCATIONS;
            adjLoc = malloc(numLoc * sizeof(LocationID));
            for (x = MIN_MAP_LOCATION; x < NUM_MAP_LOCATIONS; x++) adjLoc[x] = x;
            locAbbrev = furthestLocation(gameState, adjLoc, numLoc);
            registerBestPlay(locAbbrev, "first move");
            free(adjLoc);
        }
        return;
    } else {
        adjLoc = whereCanIgo(gameState, &numLoc, TRUE, TRUE);
        printf("Where am i %s\n", idToName(whereIs(gameState, PLAYER_DRACULA)));
        
        for (y = 0 ; y < numLoc ; y++) {
            printf("adjLocations[%d] = %s\n", y, idToName(adjLoc[y]));
        }
        if (numLoc == 0) {
            registerBestPlay("TP", "come to CD and catch me!");
            return;
        }
        
        //if (round % 13 == 0 && numLoc) {
           // for (

        locAbbrev = furthestLocation(gameState, adjLoc, numLoc);
int location = abbrevToID(locAbbrev);
printf("best play %s\n", idToName(location));
        locToDBOrHI(locAbbrev, gameState);
        registerBestPlay(locAbbrev, "other moves");
        free(adjLoc);
        return;
    }
}

// higher score if the place suits dracula more
int locationScore(LocationID location, int length, int health) {
    Map map = newMap();
    int nRail = 0;        // count the number of rail connections of a place
    int nRoad = 0;        // count the number of road connections of a place
    int nBoat = 0;        // count the number of baot connections of a place
    int typeScore = 0;
    int locScore = 0;
    VList cur = map->connections[location];
    
    for (; cur != NULL; cur = cur->next) {
        if (cur->type == RAIL || cur->type == ANY)
            nRail++;
        if (cur->type == ROAD || cur->type == ANY)
            nRoad++;
        if (cur->type == BOAT || cur->type == ANY)
            nBoat++;
    }
    if (idToType(location) == SEA) {
        typeScore = health/2;
    } else if (idToType(location) == LAND) {
        typeScore = 62;
    }
    
    locScore = (length * 10) + typeScore + nRoad + nBoat - nRail;
    //printf("locScore = %d\n", locScore);
    disposeMap(map);
    return locScore;
}

// count the number of hunters that are able go to the given place next round
int possibleHunters(DracView gameState, LocationID place) {
    PlayerID hunter;
    int numLoc;
    int count = 0;
    Round round = giveMeTheRound(gameState) + 1;
    
    for (hunter = PLAYER_LORD_GODALMING ; hunter <= PLAYER_MINA_HARKER; hunter++) {
        LocationID *futureMove = whereCanTheyGo(gameState, &numLoc,
                                                whereIs(gameState, hunter),
                                                hunter, round, TRUE, TRUE, TRUE);
        if (inArray(futureMove, place, numLoc) != -1)
            count++;
    }
    return count;
}

// find the adjacent location which is furthest away from all the hunters
char *furthestLocation(DracView gameState, LocationID *adjLocations,
                       int numLoc) {
    PlayerID hunter;
    LocationID location;
    char *abbrev = malloc(3 * sizeof(char));
    Round round = giveMeTheRound(gameState) + 1;
    int length;
    int nearestLength = MAX_MAP_LOCATION;
    int maxLength = -1;
    int tmpScore;
    int locScore = 0;
    int possHunters;
    int minPossHunters = 5;
    LocationID nextLoc;
    int x;
    
    for (x = 0 ; x < numLoc ; x++) {
        possHunters = possibleHunters(gameState, adjLocations[x]);
        for (hunter = PLAYER_LORD_GODALMING; hunter <= PLAYER_MINA_HARKER; hunter++) {
            length = findPathLength(adjLocations[x], whereIs(gameState, hunter), hunter, round, &nextLoc);
            //printf("src = %s, dest = %s, round = %d, player = %d, length = %d\n", idToName(adjLocations[x]), idToName(whereIs(gameState, hunter)), round, hunter, length);
            if (length < nearestLength) nearestLength = length;
        }
        if (possHunters < minPossHunters) {
            maxLength = nearestLength;
            location = adjLocations[x];
            minPossHunters = possHunters;
        } else if ((possHunters == minPossHunters) && (nearestLength > maxLength)) {
            maxLength = nearestLength;
            location = adjLocations[x];
        } else if ((possHunters == minPossHunters) && (nearestLength == maxLength)) {
            tmpScore = locationScore(adjLocations[x], nearestLength, howHealthyIs(gameState, PLAYER_DRACULA));
            if (tmpScore > locScore) {
                location = adjLocations[x];
                locScore = tmpScore;
            }
        }
    }
    idToAbbrev(location, abbrev);
    return abbrev;
}

// check whether the largest island is occupied by a hunter
int islandEmpty(DracView gameState) {
    PlayerID player;
    LocationID location;
    
    for (player = PLAYER_LORD_GODALMING ; player <= PLAYER_MINA_HARKER; player++) {
        location = whereIs(gameState, player);
        if (location == EDINBURGH || location == MANCHESTER ||
            location == LIVERPOOL || location == SWANSEA	||
            location == LONDON || location == PLYMOUTH)
            return 0;
    }
    return 1;
}


//int atLeastOneIsolated(char **location, DracView gameState) {
//    int numLoc;
//    PlayerID player;
//    int distance = 0;
//    for (player = 0; player < 4; player++) {
//        LocationID *adjLocations = whereCanTheyGo(gameState, &numLoc, player,
//                                                  ROAD, RAIL, SEA);
//        LocationID otherLocation = nameToID(furthestLocation(gameState,
//                                                                        adjLocations, numLoc, player));
//        int nearestHunterDistance = findPathLength(otherLocation,
//                                                   whereIs(gameState, player));
//        if (isIsolated(player, gameState) && distance < nearestHunterDistance) {
//            distance = nearestHunterDistance;
//            *location = idToName(whereIs(gameState, player));
//        }
//    }
//    if (distance != 0) {
//        player = mostIsolatedPlayer(gameState);
//        LocationID *adjLocations = whereCanTheyGo(gameState, &numLoc, player, ROAD, RAIL, SEA);
//        *location = furthestLocation(gameState, adjLocations, numLoc, player);
//    }
//    return distance != 0 ? 1 : 0;
//}

// find the most isolated hunter, return -1 if no isolated hunter is found
PlayerID mostIsolated(DracView gameState) {
    int distance = 0;
    int length, nearest = MAX_MAP_LOCATION;
    PlayerID player, hunter;
    PlayerID mostIsolatedHunter = -1;
    LocationID nextLoc;
    
    for (player = PLAYER_LORD_GODALMING; player <= PLAYER_MINA_HARKER; player++) {
        for (hunter = PLAYER_LORD_GODALMING; hunter <= PLAYER_MINA_HARKER; hunter++) {
            if (hunter != player) {
                length = findPathLength(whereIs(gameState, hunter),
                                        whereIs(gameState, player),
                                        hunter, giveMeTheRound(gameState) + 1, &nextLoc);
                if (length < nearest)
                    nearest = length;
            }
        }
        if (nearest > distance) {
            distance = nearest;
            mostIsolatedHunter = player;
        }
    }
    if (distance <= 2) return -1;
    return mostIsolatedHunter;
}

//LocationID optimalStartLocation(DracView gameState) {
//    LocationID tempLocation;
//    LocationID location;
//    int total=0;
//    int tempTotal;
//    int players;
//    
//    for (x = MIN_MAP_LOCATION); x < NUM_MAP_LOCATIONS; x++) {
//        if (findPathLength()
//    
//    for (tempLocation = 0; tempLocation < 71; tempLocation++) {
//        tempTotal = 0;
//        players = 0;
//        for ( ; players < 4 ; players++ ) {
//            tempTotal += findPathLength(tempLocation, whereIs(gameState, players));
//        }
//        if (total < tempTotal) {
//            total = tempTotal;
//            location = tempLocation;
//        }
//    }
//    return location;
//}

//char *optimalLocation(DracView gameState, LocationID *adjLocations, int numLoc,
//                      PlayerID player) {
//    int otherHunters;
//    char *location;
//    int total = -1;
//    int tempTotal = 0;
//    PQueue pq = newPQueue();
//    int x;
//    for (x = 0 ; x < numLoc ; x++) {
//        tempTotal = 0;
//        for (otherHunters = 0; otherHunters < 4; otherHunters++) {
//            if (player != otherHunters){
//                PQueueJoin(pq, adjLocations[x], findPathLength(adjLocations[x],
//                                                               whereIs(gameState, otherHunters)));
//            }
//        }
//        int order = 0; //adjust weighting here
//        while(!PQueueIsEmpty(pq)) {
//            if (order == 0) tempTotal += PQueueLeave(pq) * 40; //weighing 1
//            if (order == 1) tempTotal += PQueueLeave(pq) * 30; //weighing 2
//            if (order == 2) tempTotal += PQueueLeave(pq) * 20; //weighing 3
//            if (order == 3) tempTotal += PQueueLeave(pq) * 10; //weighing 4
//            order++;
//        }
//        if (total > tempTotal || total == -1) {
//            total = tempTotal;
//            location = idToName(adjLocations[x]);
//            printf("tempTotal: %d\n", tempTotal);
//            printf("location: %s\n", location);
//        }
//    }
//    return location;
//}

void locToDBOrHI(char *locAbbrev, DracView currentView) {
    LocationID trail[TRAIL_SIZE];
    LocationID hideTrail[TRAIL_SIZE];
    giveMeTheTrail(currentView, PLAYER_DRACULA, trail, FALSE);
    giveMeTheTrail(currentView, PLAYER_DRACULA, hideTrail, TRUE);
    shiftRight(trail, 0, TRAIL_SIZE - 1);
    shiftRight(hideTrail, 0, TRAIL_SIZE - 1);
    trail[0] = abbrevToID(locAbbrev);
    hideTrail[0] = abbrevToID(locAbbrev);
    int dbOrHi = hasDBOrHI(hideTrail);

    if (dbOrHi == BOTH_HIDE_AND_DB) return;      
    if (trail[0] == trail[1] &&
        (dbOrHi != HAS_HIDE) &&
        idToType(trail[0]) != SEA) {
        strcpy(locAbbrev, "HI");
    } else if (dbOrHi != HAS_DOUBLE_BACK) {
        if (trail[0] == trail[1]) {
            strcpy(locAbbrev, "D1");
        } else if (trail[0] == trail[2]) {
            strcpy(locAbbrev, "D2");
        } else if (trail[0] == trail[3]) { 
            strcpy(locAbbrev, "D3");
        } else if (trail[0] == trail[4]) {
            strcpy(locAbbrev, "D4");
        } else if (trail[0] == trail[5]) {
            strcpy(locAbbrev, "D5");
        }
    }
    return;
}
