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
#include <time.h>

LocationID seaLoc[NUM_SEA] = {0, 4, 7, 10, 23, 32, 33, 43, 48, 64};

// register the best play to the game engine
void submitPlay(DracView gameState, LocationID *adjLoc, Map map, Map railMap, LocationID *path, LocationID best,
                int isHide, LocationID trail[TRAIL_SIZE],
                LocationID hideTrail[TRAIL_SIZE], PlayerMessage msg);

// change the location to HI or DB if needed
void locToDBOrHI(char *locAbbrev, LocationID locID,
                                LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE]);

// check which location in dracula's trail is known by the hunters
//   (trail[0] is the loc of dracula in last round)
// return array of trail indexes of that know locations
int *knownByHunter(int *numLoc, LocationID trail[TRAIL_SIZE],
                    LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// check whether the largest island is occupied by a hunter
int islandEmpty(LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// find the most isolated hunter, return -1 if no isolated hunter is found
PlayerID mostIsolated(Map map, Map railMap, LocationID *path, Round currRound, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// calculate an optimal score for a place which deal with all the factors
int optimalLocScore(int distFromHunt, int numOfNextAdj, int possHunt, int numHunt,
                    int numPossHunt, int locScore, int tuneScore, int distFromSea);

// higher score if the place suits dracula more
int locationScore(Map map, LocationID location, int health, Round currRound);

// count the number of hunters that are able go to the given place next round
int possibleHunters(DracView gameState, LocationID place, Round currRound,
                    LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) ;

// count the number of hunters that will possibly go to dest in the next round
int numOfPossHunter(DracView gameState, Map map, Map railMap, LocationID dest, int *knownLoc,
                    int knownNumLoc, Round currRound, LocationID trail[TRAIL_SIZE],
                    LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE], LocationID *path);

// count the number of hunters in a given location in current round
int numOfHunter(LocationID location, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// count the number of possible move next round if Dracula choose dest as a location to go
// also check that if we need to hide dest if we choose it as next move
// *needHiding == 1 means need hiding
// *needHiding == 0 means no hiding is needed
int numOfNextAdj(LocationID dest, LocationID trail[TRAIL_SIZE],
                LocationID hideTrail[TRAIL_SIZE], int *needHiding);

// check if the hunter is doing a research
int isResearch(DracView gameState);

//check if drac is surrounded with hunters in adjacent locations excluding his current location
int isSurrounded(LocationID *adjLoc, int numLoc, LocationID dCurrLoc,
                  LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// find the distance between dest and the nearest hunter
int distFromNearestHunter(Map map, Map railMap, LocationID location, LocationID *path, Round round,
                          LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// choose an adjacent location that is furthest away from the nearest hunter
// path[1] store that adjacent location
// the function returns the length between that locatino and the nearest hunter's location
int furthestLength(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc, Round round,
                   LocationID *path, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// Dracula version of perdict Dracula move
// to predict the moves of dracula in next few round when one of the dracula's location is known
// predict where dracula will go in round round
LocationID predictDracMove(DracView gameState, Map map, Map railMap, LocationID dracSrc, Round round,
                            Round currRound, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                            LocationID *path);

// to predict the moves of hunter in next round when one of the dracula's location is known
LocationID predictHunterMove(DracView gameState, Map map, Map railMap, PlayerID hunter, 
                            Round currRound,
                            LocationID trail[TRAIL_SIZE], int *knownLoc, int knownNumLoc,
                            LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE], LocationID *path);

// check if dracula is in a dangerous situation
int isDangerous(Map map, Map railMap, int *knownLoc, int knownNumLoc, 
                int distFromSea, int distFromHunt,
                LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                Round currRound, LocationID *path);

// check if dracula is gonna win
int isGonnaWin(DracView gameState, LocationID fallOffLoc, int health);

// calculate the distance between the nearest sea and the dest
// (need a new ver of findpathlength for drac (cant revisit places))
int distFromNearestSea(Map map, Map railMap, LocationID location, LocationID *path);

// find the shortest path to escape to a sea and return the next location in that path
// minLength is the shortest length required to go to that sea
LocationID escapeToSea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                        int *minLength, Round currRound, int *knownLoc, int knownNumLoc,
                        int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                        LocationID trail[TRAIL_SIZE], LocationID *path);

// check if the given dest is a best place among all other places that passed in before
// all min and max variable keep updating if use it in a loop
int isBestLoc(LocationID dest, int possHunt, int *minPossHunt, int numPossHunt,
              int *minNumPossHunt, int numHunt, int *minNumHunt, int length,
              int *minLength, int locScore, int *maxScore);

// adjust the optimsl score of a location with special cases considered
// distFromHunt = distFromNearestHunter(dest, path, hTrail)
int scoreTunner(LocationID dest, int numHunt, int possHunt, int needHiding,
                int *knownLoc, int knownNumLoc, int distFromHunt,
                LocationID trail[TRAIL_SIZE]);

// check whether dracula is in concern by the hunter
int isConcerned(Map map, Map railMap, LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                Round currRound, LocationID *path);

// to count the number of land type in adjLoc
int numOfLand(LocationID * adjLoc, int numLoc);


LocationID furthestLocation(DracView gameState, Map map, Map railMap, LocationID *adjLocations,
                       int numLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);


void decideDraculaMove(DracView gameState)
{
    PlayerID hunter;
    LocationID x, currLoc;
    int y, i;
    int numLoc = 0, count = 0, knownNumLoc;
    LocationID trail[TRAIL_SIZE];
    LocationID hideTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, trail, FALSE);
    LocationID dCurrLoc = trail[0];
    LocationID fallOffLoc = trail[5];
    giveMeTheTrail(gameState, PLAYER_DRACULA, hideTrail, TRUE);
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];

    copyArray(trail, tmpTrail, TRAIL_SIZE);
    copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
    LocationID *adjLoc = NULL;
    LocationID *path = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    Round currRound = giveMeTheRound(gameState);
    int health = howHealthyIs(gameState, PLAYER_DRACULA);
    Map map = newGraph();
    Map railMap = newRailGraph();
    LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE];
    for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
      giveMeTheTrail(gameState, hunter, hTrail[hunter], FALSE);
    }
    int *knownLoc = knownByHunter(&knownNumLoc, trail, hTrail);
    int optimalScore, maxOptimalScore = -1000;

printf("hhll = %d\n", health);
    for (y = 0 ; y < TRAIL_SIZE ; y++) {
        printf("trail[%d] = %s\n", y, idToName(trail[y]));
    }
    for (y = 0 ; y < TRAIL_SIZE ; y++) {
        printf("hideTrail[%d] = %d\n", y, hideTrail[y]);
    }

    if (currRound == 0) {
        PlayerID mostIsolatedHunter = mostIsolated(map, railMap, path, currRound, hTrail);
        if (islandEmpty(hTrail)) {
            registerBestPlay("ED", "first move");
        } else if (mostIsolatedHunter != -1) {
            LocationID from = hTrail[mostIsolatedHunter][0];
            for (x = MIN_MAP_LOCATION; x < NUM_MAP_LOCATIONS; x++) {
                if (hunterPathLength(map, railMap, from, x, mostIsolatedHunter, 1, path) == 2) {
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
            currLoc = furthestLocation(gameState, map, railMap, adjLoc, numLoc, hTrail);
            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, FALSE,
                        trail, hideTrail, "first move");
        } else {
            numLoc = NUM_MAP_LOCATIONS;
            adjLoc = malloc(numLoc * sizeof(LocationID));
            for (x = MIN_MAP_LOCATION; x < NUM_MAP_LOCATIONS; x++) adjLoc[x] = x;
            currLoc = furthestLocation(gameState, map, railMap, adjLoc, numLoc, hTrail);
            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, FALSE,
                        trail, hideTrail, "first move");
        }
        return;
    } else {
        adjLoc = whereCanIgo(&numLoc, tmpTrail, tmpHideTrail, dCurrLoc, TRUE, TRUE);
        printf("Where am i %s\n", idToName(whereIs(gameState, PLAYER_DRACULA)));

        for (y = 0 ; y < numLoc ; y++) {
            printf("I//adjLocations[%d] = %s\n", y, idToName(adjLoc[y]));
//int length;
/*for (hunter=0;hunter<4;hunter++) {
printf("currRound = %d\n",currRound);
printf("hunter %d is at %s\n", hunter, idToName(hTrail[hunter][0]));
hunterPathLength(map, railMap, hTrail[hunter][0], adjLoc[y], hunter, currRound+1);
findPathLength(hTrail[hunter][0], adjLoc[y], hunter, currRound+1, path);}*/
        }

        if (numLoc == 0) {
            registerBestPlay("TP", "come to CD and catch me!");
            return;
        }

        if (health <= 2) {
printf("hhhhhhh\n");
            for (x = 0; x < numLoc; x++) {
                if (idToType(adjLoc[x]) != SEA) {
                    currLoc = adjLoc[x];
                    count++;
                }
            }
printf("count = %d\n", count);
            if (count == 1) {
              submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
                          trail, hideTrail, "I hate water!");
                return;
            } else if (count > 1) {
                for (x = 0; x < numLoc; x++) {
                    if (idToType(adjLoc[x]) == SEA) {
                        shiftLeft(adjLoc, x, numLoc);
                        numLoc--;
                    }
                }
            }
        }

        /*if (isBeingKilled(gameState)) {
                  adjLoc = whereCanIgo(gameState, &numLoc, TRUE, TRUE);
                  currLoc = lastStand(gameState, adjLoc, numLoc);
                  printf("last Stand %s\n", idToName(currLoc));
                  submitPlay(gameState, adjLoc, currLoc, TRUE, "other moves");
                  return;
                }*/
        int distCurrFromHunt = distFromNearestHunter(map, railMap, dCurrLoc, path, currRound, hTrail);
        int distCurrFromSea = distFromNearestSea(map, railMap, dCurrLoc, path);

        if (isGonnaWin(gameState, fallOffLoc, health) ||
            isDangerous(map ,railMap, knownLoc, knownNumLoc, distCurrFromSea, distCurrFromHunt,
                        dCurrLoc, hTrail, currRound, path)) {
            int minLength;
            currLoc = escapeToSea(gameState, map, railMap, adjLoc, numLoc, &minLength,
                                  currRound, knownLoc, knownNumLoc, health, hTrail, trail, path);
            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
                        trail, hideTrail, "The world is mine!");
            return;
        }

        for (i = 0; i < numLoc; i++) {
clock_t begin = clock();
          int distFromHunt = distFromNearestHunter(map, railMap, adjLoc[i], path, currRound, hTrail);
          int distFromSea = distFromNearestSea(map ,railMap, adjLoc[i], path);
          int needHiding;
          int nNextAdj = numOfNextAdj(adjLoc[i], trail, hideTrail, &needHiding);
          int possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
          int numHunt = numOfHunter(adjLoc[i], hTrail);

          int numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                              knownNumLoc, currRound, trail, hTrail, path);
          //if (numLoc > 4) numPossHunt = 0;
          //else numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                              //knownNumLoc, currRound, trail, hTrail, path);
                              
          int locScore = locationScore(map, adjLoc[i], health, currRound);
          int tuneScore = scoreTunner(adjLoc[i], numHunt, possHunt, needHiding,
                                      knownLoc, knownNumLoc, distFromHunt, trail);
          optimalScore = optimalLocScore(distFromHunt, nNextAdj, possHunt, numHunt,
                                          numPossHunt, locScore, tuneScore, distFromSea);
    clock_t end = clock();
double time_spent = (double)(end - begin); // CLOCKS_PER_SEC;
printf("ot = %lf\n", time_spent);
          if (optimalScore > maxOptimalScore) {
            maxOptimalScore = optimalScore;
            currLoc = adjLoc[i];
          }
        }

        /*for (i = 0 ; i < numLoc ; i++) {
printf("posssssss\n");
            if (possibleHunters(gameState, adjLoc[i]) == 1) {
                for (hunter = PLAYER_LORD_GODALMING ; hunter <= PLAYER_MINA_HARKER; hunter++) {
                    if (whereIs(gameState, hunter) == adjLoc[i]) {
                         if ((howHealthyIs(gameState, hunter) < 2) && (round % 13 != 0)) {
                            submitPlay(gameState, adjLoc, adjLoc[i], TRUE, "Kill me if you dare");
                            return;
                         }
                    }
                }
            }
        }*/
printf("best play %s\n", idToName(currLoc));
        submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
                  trail, hideTrail, "other moves");
        return;
    }
}

// register the best play to the game engine
void submitPlay(DracView gameState, LocationID *adjLoc, Map map, Map railMap, LocationID *path, LocationID best,
                int isHide, LocationID trail[TRAIL_SIZE],
                LocationID hideTrail[TRAIL_SIZE], PlayerMessage msg) {
    //LocationID trail[TRAIL_SIZE];
    //LocationID hideTrail[TRAIL_SIZE];
    //giveMeTheTrail(gameState, PLAYER_DRACULA, trail, FALSE);
    //giveMeTheTrail(gameState, PLAYER_DRACULA, hideTrail, TRUE);
    char *locAbbrev = malloc(3 * sizeof(char));
    idToAbbrev(best, locAbbrev);
    if (isHide == TRUE) locToDBOrHI(locAbbrev, best, trail, hideTrail);
    registerBestPlay(locAbbrev, msg);
    free(locAbbrev);
    free(adjLoc);
    free(path);
    disposeMap(map);
    disposeMap(railMap);
    return;
}

// change the location to HI or DB if needed
void locToDBOrHI(char *locAbbrev, LocationID locID,
                LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE]) {
    shiftRight(trail, 0, TRAIL_SIZE - 1);
    shiftRight(hideTrail, 0, TRAIL_SIZE - 1);
    trail[0] = locID;
    hideTrail[0] = locID;
    int dbOrHi = hasDBOrHI(hideTrail);
//printf("trail[0] = %d, trail[1] = %d, dbORHi = %d, HAS_HIDE = %d\n", trail[0], trail[1], dbOrHi, HAS_HIDE);
    if (dbOrHi == BOTH_HIDE_AND_DB) return;
    if (trail[0] == trail[1] &&
        (dbOrHi != HAS_HIDE) &&
        idToType(trail[0]) != SEA) {
        strcpy(locAbbrev, "HI");
        hideTrail[0] = HIDE;
    } else if (dbOrHi != HAS_DOUBLE_BACK) {
        if (trail[0] == trail[1]) {
            strcpy(locAbbrev, "D1");
            hideTrail[0] = DOUBLE_BACK_1;
        } else if (trail[0] == trail[2]) {
            strcpy(locAbbrev, "D2");
            hideTrail[0] = DOUBLE_BACK_2;
        } else if (trail[0] == trail[3]) {
            strcpy(locAbbrev, "D3");
            hideTrail[0] = DOUBLE_BACK_3;
        } else if (trail[0] == trail[4]) {
            strcpy(locAbbrev, "D4");
            hideTrail[0] = DOUBLE_BACK_4;
        } else if (trail[0] == trail[5]) {
            strcpy(locAbbrev, "D5");
            hideTrail[0] = DOUBLE_BACK_5;
        }
    }
    return;
}

// check which location in dracula's trail is known by the hunters
//   (trail[0] is the loc of dracula in last round)
// return array of trail indexes of that know locations
int *knownByHunter(int *numLoc, LocationID trail[TRAIL_SIZE],
                    LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    PlayerID hunter;
    *numLoc = 0;
    int *knownLoc = malloc(TRAIL_SIZE * sizeof(LocationID));
    int i, j, visited = 0;

    for (i = 0; i < TRAIL_SIZE; i++) {
        for (hunter = PLAYER_LORD_GODALMING; hunter <NUM_HUNTERS && visited != 1; hunter++) {
            for (j = 0; j < TRAIL_SIZE; j++) {
                if ((hTrail[hunter][j] == trail[i]) && (trail[i] != UNKNOWN_LOCATION)) {
                    knownLoc[*numLoc] = i;
                    (*numLoc)++;
                    visited = 1;
                    break;
                }
            }
        }
        visited = 0;
    }
    //for (i=0;i<(*numLoc);i++) printf("known=trail[%d]", knownLoc[i]);
    return knownLoc;
}

// check whether the largest island is occupied by a hunter
int islandEmpty(LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    PlayerID player;
    LocationID location;

    for (player = PLAYER_LORD_GODALMING ; player < NUM_HUNTERS; player++) {
        location = hTrail[player][0];
        if (location == EDINBURGH || location == MANCHESTER ||
            location == LIVERPOOL || location == SWANSEA	||
            location == LONDON || location == PLYMOUTH)
            return 0;
    }
    return 1;
}

// find the most isolated hunter, return -1 if no isolated hunter is found
PlayerID mostIsolated(Map map, Map railMap, LocationID *path, Round currRound, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    int distance = 0;
    int length, nearest = MAX_MAP_LOCATION;
    PlayerID player, hunter;
    PlayerID mostIsolatedHunter = -1;
    //LocationID *path = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));

    for (player = PLAYER_LORD_GODALMING; player < NUM_HUNTERS; player++) {
        for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
            if (hunter != player) {
                length = hunterPathLength(map, railMap, hTrail[hunter][0], hTrail[player][0], hunter, currRound + 1, path);
                if (length < nearest) nearest = length;
            }
        }
        if (nearest > distance) {
            distance = nearest;
            mostIsolatedHunter = player;
        }
    }
    //free(path);
    if (distance <= 2) return -1;
    return mostIsolatedHunter;
}

// calculate an optimal score for a place which deal with all the factors
int optimalLocScore(int distFromHunt, int nNextAdj, int possHunt, int numHunt,
                    int numPossHunt, int locScore, int tuneScore, int distFromSea) {
  int optimalScore;
  optimalScore = 48 * distFromHunt + 16 * nNextAdj - 4 * possHunt - numHunt - 2 * numPossHunt + locScore + tuneScore - 10 * distFromSea;
printf("aaa = %d\n", distFromHunt);
printf("bbb = %d\n", nNextAdj);
printf("ccc = %d\n", possHunt);
printf("ddd = %d\n", numHunt);
printf("eee = %d\n", numPossHunt);
printf("fff = %d\n", locScore);
printf("ggg = %d\n", tuneScore);
printf("hhh = %d\n", distFromSea);
printf("scsg = %d\n", optimalScore);
  return optimalScore;
}

// higher score if the place suits dracula more
int locationScore(Map map, LocationID location, int health, Round currRound) {
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
        if (health <= 2) {
          typeScore = -100;
        } else if (health <= 12) {
          typeScore = -40;
        } else if (currRound % 13 == 0){
          typeScore = -50;
        }
    } else if (idToType(location) == LAND) {
        typeScore = 62;
    }

    locScore = typeScore + nRoad * 2 + nBoat - nRail;
    return locScore;
}

// count the number of hunters that are able go to the given place next round
int possibleHunters(DracView gameState, LocationID place, Round currRound,
                    LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    PlayerID hunter;
    int numLoc;
    int count = 0;

    if (idToType(place) == SEA) return 0;
    for (hunter = PLAYER_LORD_GODALMING ; hunter < NUM_HUNTERS; hunter++) {
        LocationID *futureMove = whereCanTheyGo(gameState, &numLoc,
                                                hTrail[hunter][0],
                                                hunter, currRound + 1, TRUE, TRUE, TRUE);
        if (inArray(futureMove, place, numLoc) != -1)
            count++;
        free(futureMove);
    }
    return count;
}

// count the number of hunters that will possibly go to dest in the next round
int numOfPossHunter(DracView gameState, Map map, Map railMap, LocationID dest, int *knownLoc,
                    int knownNumLoc, Round currRound, LocationID trail[TRAIL_SIZE],
                    LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE], LocationID *path) {
  PlayerID hunter;
  int count = 0;

  if (knownNumLoc > 0) {
    for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
      if (dest == predictHunterMove(gameState, map, railMap, hunter, currRound, trail, knownLoc,
                                    knownNumLoc, hTrail, path)) count++;
    }
  }
  return count;
}

// count the number of hunters in a given location in current round
int numOfHunter(LocationID location, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
  PlayerID hunter;
  int count = 0;

  for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
    if (location == hTrail[hunter][0]) count++;
  }
  return count;
}

// count the number of possible move next round if Dracula choose dest as a location to go
// also check that if we need to hide dest if we choose it as next move
// *needHiding == 1 means need hiding
// *needHiding == 0 means no hiding is needed
int numOfNextAdj(LocationID dest, LocationID trail[TRAIL_SIZE],
                LocationID hideTrail[TRAIL_SIZE], int *needHiding) {
  int numLoc;
  char *locAbbrev = malloc(3 * sizeof(char));
  idToAbbrev(dest, locAbbrev);
  LocationID tmpHideTrail[TRAIL_SIZE];
  LocationID tmpTrail[TRAIL_SIZE];

  copyArray(trail, tmpTrail, TRAIL_SIZE);
  copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
  locToDBOrHI(locAbbrev, dest, tmpTrail, tmpHideTrail);
  whereCanIgo(&numLoc, tmpTrail,
                  tmpHideTrail, dest, TRUE, TRUE);
  if (hideTrail[0] > MAX_MAP_LOCATION) *needHiding = 1;
  else *needHiding = 0;
  free(locAbbrev);
  return numLoc;
}

// check if the hunter is doing a research
int isResearch(DracView gameState) {
    PlayerID hunter;
    int count = 0 ;
    LocationID start, end;

    for (hunter = PLAYER_LORD_GODALMING; hunter <NUM_HUNTERS; hunter++) {
        lastMove(gameState, hunter, &start, &end);
        if (start == end ) count++;
    }
    if (count == NUM_HUNTERS) return 1;
    else return 0;
}

//check if drac is surrounded with hunters in adjacent locations excluding his current location
int isSurrounded(LocationID *adjLoc, int numLoc, LocationID dCurrLoc,
                  LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
  int i;
  for (i = 0 ; i < numLoc ; i++) {
     if (numOfHunter(adjLoc[i], hTrail) == 0
        && adjLoc[i] != dCurrLoc) return 0;
  }
  return 1;
}

// find the distance between dest and the nearest hunter
int distFromNearestHunter(Map map, Map railMap, LocationID location, LocationID *path, Round round,
                          LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
  PlayerID hunter;
  int length, minLength = NUM_MAP_LOCATIONS;

  for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
      length = hunterPathLength(map, railMap, hTrail[hunter][0], location, hunter, round+1, path);
      if (length < minLength) minLength = length;
  }
  return minLength;
}

// choose an adjacent location that is furthest away from the nearest hunter
// path[1] store that adjacent location
// the function returns the length between that locatino and the nearest hunter's location
int furthestLength(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc, Round round,
                  LocationID *path, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    PlayerID hunter;
    int length, i;
    int nearestLength = MAX_MAP_LOCATION;
    int maxLength = -1;
    LocationID tmp;
  //  LocationID *path = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));

    for (i = 0; i < numLoc; i++) {
      for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
        length = hunterPathLength(map, railMap, hTrail[hunter][0], adjLoc[i], hunter, round+1, path);
        if (length < nearestLength) {
          nearestLength = length;
          tmp = adjLoc[i];
        }
      }
      if (nearestLength > maxLength) {
        maxLength = nearestLength;
        path[1] = tmp;
      }
    }
    return maxLength;
}

// Dracula version of perdict Dracula move
// to predict the moves of dracula in next few round when one of the dracula's location is known
// predict where dracula will go in round round
LocationID predictDracMove(DracView gameState, Map map, Map railMap, LocationID dracSrc, Round round,
                          Round currRound, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                          LocationID *path) {
    PlayerID hunter;
    LocationID predictLoc;
    int length;
    int nearestLength = MAX_MAP_LOCATION;
    int maxLength = -1;
    int numLoc;
    LocationID *adjLoc = whereCanTheyGo(gameState, &numLoc,
                               dracSrc, PLAYER_DRACULA, round,
                               TRUE, FALSE, TRUE);
    int i;

    for (i = 0 ; i < numLoc ; i++) {
        for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
            length = hunterPathLength(map, railMap, hTrail[hunter][currRound - round], adjLoc[i], hunter, round+1, path);
            if (length < nearestLength) nearestLength = length;
        }
        if (nearestLength > maxLength) {
            maxLength = nearestLength;
            predictLoc = adjLoc[i];
        } else if (nearestLength == maxLength) {
            if (idToType(adjLoc[i]) != SEA) {
                predictLoc = adjLoc[i];
            }
        }
    }
    return predictLoc;
}

// to predict the moves of hunter in next round when one of the dracula's location is known
LocationID predictHunterMove(DracView gameState, Map map, Map railMap, PlayerID hunter, 
                            Round currRound,
                            LocationID trail[TRAIL_SIZE], int *knownLoc, int knownNumLoc,
                            LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE], LocationID *path) {
    int length;
    LocationID huntMove;
    LocationID dracMove = predictDracMove(gameState, map, railMap, trail[knownLoc[0]], currRound - knownLoc[0],
                                          currRound, hTrail, path);
    LocationID *predictPath = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    int i;
    for (i = 1; i <= knownLoc[0]; i++) {
        dracMove = predictDracMove(gameState, map ,railMap, dracMove, currRound - knownLoc[0] + i, currRound, hTrail, path);
    }
    length = hunterPathLength(map, railMap, hTrail[hunter][0], dracMove, hunter, currRound + 1, predictPath);
    if (length > 0) huntMove = predictPath[1];
    else huntMove = predictPath[0];
    free(predictPath);
    return huntMove;
}

// check if dracula is in a dangerous situation
int isDangerous(Map map, Map railMap, int *knownLoc, int knownNumLoc, 
                int distFromSea, int distFromHunt,
                LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                Round currRound, LocationID *path) {
    if (knownNumLoc > 0) {
      if (knownLoc[0] < 3) return 1;
    }
    if (distFromSea > 3 && distFromHunt < 2) return 1;
    if (isConcerned(map, railMap, dCurrLoc, hTrail, currRound, path)) return 1;
    //if (distFromHunt <= 1) return 1;
    return 0;
}

// check if dracula is gonna win
int isGonnaWin(DracView gameState, LocationID fallOffLoc, int health) {
    int score = giveMeTheScore(gameState);
    int numTraps, numVamps;
    whatsThere(gameState, fallOffLoc,
               &numTraps, &numVamps);
    if (numVamps > 0) score -= 13;
    if (score < health/2) return 1;
    return 0;
}

// calculate the distance between the nearest sea and the dest
/////// (need a new ver of findpathlength for drac (cant revisit places))//////
int distFromNearestSea(Map map, Map railMap, LocationID location, LocationID *path) {
  int i, length, minLength = NUM_MAP_LOCATIONS;
  for (i = 0; i < NUM_SEA; i++) {
      length = hunterPathLength(map, railMap, location, seaLoc[i], PLAYER_DRACULA, 1, path);
      if (length < minLength) minLength = length;
  }
  return minLength;
}

// find the shortest path to escape to a sea and return the next location in that path
// minLength is the shortest length required to go to that sea
LocationID escapeToSea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                        int *minLength, Round currRound, int *knownLoc, int knownNumLoc,
                        int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                        LocationID trail[TRAIL_SIZE], LocationID *path) {
  int x, length;
  int numHunt, minNumHunt = NUM_HUNTERS;
  int numPossHunt, minNumPossHunt = NUM_HUNTERS;
  int possHunt, minPossHunt = NUM_HUNTERS;
  int locScore, maxScore = -1000;
  *minLength = NUM_MAP_LOCATIONS;
  LocationID currLoc;

    for (x = 0; x < numLoc; x++) {
      numHunt = numOfHunter(adjLoc[x], hTrail);
      numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[x], knownLoc,
                          knownNumLoc, currRound, trail, hTrail, path);
      possHunt = possibleHunters(gameState, adjLoc[x], currRound, hTrail);
      // needa deal with cases that drac cant revisit the trail loc later
      length = distFromNearestSea(map ,railMap, adjLoc[x], path);
      locScore = locationScore(map, adjLoc[x], length, health);
      if (isBestLoc(adjLoc[x], possHunt, &minPossHunt, numPossHunt,
                    &minNumPossHunt, numHunt, &minNumHunt, length,
                    minLength, locScore, &maxScore))
          currLoc = adjLoc[x];
    }
  return currLoc;
}

// check if the given dest is a best place among all other places that passed in before
// all min and max variable keep updating if use it in a loop
int isBestLoc(LocationID dest, int possHunt, int *minPossHunt, int numPossHunt,
                    int *minNumPossHunt, int numHunt, int *minNumHunt, int length,
                    int *minLength, int locScore, int *maxScore) {
  int isBest = 0;

  if (possHunt < *minPossHunt) {
    *minPossHunt = possHunt;
    *minLength = length;
    isBest = 1;
  } else if (possHunt == *minPossHunt) {
    if (numPossHunt < *minNumPossHunt) {
        *minNumPossHunt = numPossHunt;
        *minLength = length;
        isBest = 1;
    } else if (numPossHunt == *minNumPossHunt) {
        if (numHunt < *minNumHunt) {
            *minNumHunt = numHunt;
            if (numHunt == 0) {
                *minLength = length;
                isBest = 1;
            } else {
                if (length < *minLength) {
                    *minLength = length;
                    isBest = 1;
                } else if (length == *minLength) {
                    if (locScore > *maxScore) {
                      *maxScore = locScore;
                      isBest = 1;
                    }
                }
            }
        } else if (*minNumHunt != 0) {
            if (length < *minLength) {
                *minLength = length;
                isBest = 1;
            } else if (length == *minLength) {
              if (locScore > *maxScore) {
                *maxScore = locScore;
                isBest = 1;
              }
            }
        }
    }
  }
  return isBest;
}

// adjust the optimsl score of a location with special cases considered
// distFromHunt = distFromNearestHunter(dest, path, hTrail)
int scoreTunner(LocationID dest, int numHunt, int possHunt, int needHiding,
                int *knownLoc, int knownNumLoc, int distFromHunt,
                LocationID trail[TRAIL_SIZE]) {
  int i;
  int tuneScore = 0;

  // case: just meet  a hunter in current city
  if (knownNumLoc != 0) {
    if (knownLoc[0] == 0) {
    if (!needHiding) {
      if (numHunt == 0) {
        tuneScore += 100;
      } else {
        tuneScore -= possHunt * (30);
      }
    } else {
      for (i = 0; i < knownNumLoc; i++) {
        if (knownLoc[i] != 5 && dest == trail[knownLoc[i]]) {
          tuneScore -= 100;
        } else if (knownLoc[i] == 5 && dest == trail[knownLoc[i]]) {
          if (numHunt == 0) tuneScore += 60;
          else tuneScore -= possHunt * (30);
        } else {
          if (numHunt == 0) tuneScore += 80;
          else tuneScore -= possHunt * (30);
        }
      }
    }
    }
  }

  // case: hunters are far away enough (reserve HI and DB)
  if (distFromHunt > 3) {
    if (!needHiding) tuneScore += 100;
    else tuneScore -= 100;
    if (idToType(dest) == SEA) tuneScore -= 50;
    else tuneScore += 50;
  }
  return tuneScore;
}

// check whether dracula is in concern by the hunter
int isConcerned(Map map, Map railMap, LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                Round currRound, LocationID *path) {
  PlayerID hunter;
  int count = 0;

  for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
    if (hunterPathLength(map, railMap, hTrail[hunter][0], dCurrLoc, hunter, currRound+1, path) <= 2)
      count++;
  }
  if (count >= 3) return 1;
  else return 0;
}

// to count the number of land type in adjLoc
int numOfLand(LocationID * adjLoc, int numLoc) {
  int i, count = 0;
  for (i = 0; i < numLoc; i++) {
    if (idToType(adjLoc[i]) == LAND) count++;
  }
  return count;
}

/*
//stayNearSea returns a location from drac's adjacent location that
//not at sea but as close as possible. If there are more than one location
//than it will return which one is furthest from the nearest hunter.
//If there are no locations, not at sea than it will return
//a location using the furthestLocation function
LocationID stayNearSea(DracView gameState, LocationID *adjLoc, int numLoc, int hideIsBad) {
  LocationID bestMove = -1;
  int x;

  for (x = 0; x < numLoc; x++) {
    if (idToType(adjLoc[x]) != SEA) {
      if (bestMove == -1 || distFromNearestSea(gameState, adjLoc[x]) < distFromNearestSea(gameState, bestMove)) {
        bestMove = adjLoc[x];
      } else if (distFromNearestSea(gameState, adjLoc[x]) == distFromNearestSea(gameState, bestMove)) {
        if (distFromNearestHunter(gameState, adjLoc[x]) > distFromNearestHunter(gameState, bestMove)) bestMove = adjLoc[x];
      }
    }
  }
  if (bestMove == -1) bestMove = furthestLocation(gameState, adjLoc, numLoc);
  return bestMove;
}


int surroundedNextToSea(DracView gameState, LocationID *adjLoc, int numLoc) {
  //check if drac is surrounded with hunters in adjacent locations excluding his current location
  int x;
  int atLeastOne = 1;
  int nextToSea = 0;
  for (x = 0 ; x < numLoc ; x++) {
     if (numOfHunter(gameState, adjLoc[x]) == 0 && idToType(adjLoc[x]) != SEA) {
       atLeastOne = 0;
     }
     if (idToType(adjLoc[x]) == SEA) {
       nextToSea = 1;
     }
  }
  if (atLeastOne && nextToSea) return 1;
  return 0;
}

int isBeingKilled(DracView gameState) {
  int numLoc;
  LocationID *adjLoc = whereCanIgo(gameState, &numLoc, TRUE, TRUE);
  int x;
  int hunter;
  int numNearby=0;
  for (hunter = PLAYER_LORD_GODALMING; hunter <= PLAYER_MINA_HARKER; hunter++) {
    for (x = 0; x < numLoc ; x++) {
      if (whereIs(gameState, hunter) == adjLoc[x]) numNearby++;
    }
  }
  if (isSurrounded(gameState, adjLoc, numLoc) && numNearby*10 >= howHealthyIs(gameState, PLAYER_DRACULA)) return 1;
  if (surroundedNextToSea(gameState, adjLoc, numLoc) && howHealthyIs(gameState,
      PLAYER_DRACULA) <= 2 ) {
        if (numNearby*10 >= howHealthyIs(gameState, PLAYER_DRACULA)) return 1;
  }
  return 0;
}

//nextToHunter returns 1 if  Dracula has a hunter in his adjacent locations
int nextToHunter(DracView gameState, PlayerID player, LocationID *adjLoc, int numLoc) {
  int x;
  for (x = 0 ; x < numLoc; x++) {
    if (whereIs(gameState, player) == adjLoc[x]) return 1;
  }
  return 0;
}

//lastStand returns the location of a hunter that  is in Dracula's
//adjacentLocation if their health is less than or equal to 6.
//Otherwise it returns a location using the furthestLocation function
LocationID lastStand(DracView gameState, LocationID *adjLoc, int numLoc) {
  int hunter;
  for (hunter = PLAYER_LORD_GODALMING; hunter <= PLAYER_MINA_HARKER; hunter++) {
    if (nextToHunter(gameState, hunter, adjLoc, numLoc) && howHealthyIs(gameState, hunter) <= 6) {
      return whereIs(gameState, hunter);
    }
  }
  return furthestLocation(gameState, adjLoc, numLoc);
}

*/




// find the adjacent location which is furthest away from all the hunters
LocationID furthestLocation(DracView gameState, Map map, Map railMap, LocationID *adjLocations,
                       int numLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    PlayerID hunter;
    LocationID location;
    //char *abbrev = malloc(3 * sizeof(char));
    Round round = giveMeTheRound(gameState);
    int length;
    int nearestLength = MAX_MAP_LOCATION;
    int maxLength = -1;
    int tmpScore;
    int locScore = 0;
    int possHunters;
    int minPossHunters = 5;
    LocationID *path = calloc(NUM_MAP_LOCATIONS, sizeof(LocationID));
    int x;

    for (x = 0 ; x < numLoc; x++) {
      printf ("fur adjLocations[%d] = %s\n", x, idToName(adjLocations[x]));
        possHunters = possibleHunters(gameState, adjLocations[x], round, hTrail);
        for (hunter = PLAYER_LORD_GODALMING; hunter <= PLAYER_MINA_HARKER; hunter++) {
            length = hunterPathLength(map, railMap, hTrail[hunter][0], adjLocations[x], hunter, round+1, path);
            //printf("src = %s, dest = %s, round = %d, player = %d, length = %d\n", idToName(adjLocations[x]), idToName(whereIs(gameState, hunter)), round, hunter, length);
            if (length < nearestLength) nearestLength = length;
        }
        if (possHunters < minPossHunters) {
            maxLength = nearestLength;
            location = adjLocations[x];
            minPossHunters = possHunters;
        } else if ((possHunters == minPossHunters) && (nearestLength > maxLength)) {
            if ((round % 13 == 0) && (idToType(adjLocations[x]) == SEA) &&
                (maxLength > 2)) {
            } else if ((idToType(adjLocations[x]) == SEA) && (maxLength > 3)) {
            } else {
                maxLength = nearestLength;
                location = adjLocations[x];
            }
        } else if ((possHunters == minPossHunters) && (nearestLength == maxLength)) {
            if ((round % 13 == 0) && (idToType(adjLocations[x]) == SEA) &&
                (maxLength > 2)) {
            } else {
                tmpScore = locationScore(map, adjLocations[x], howHealthyIs(gameState, PLAYER_DRACULA), round);
                if (tmpScore > locScore) {
                    location = adjLocations[x];
                    locScore = tmpScore;
                }
            }
        }
    }
    free(path);
    //idToAbbrev(location, abbrev);
    return location;
}
