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
LocationID Island[NUM_ISLAND] = {22, 38, 39, 41, 51, 61};
LocationID portCity[NUM_PORT_CITY] = {1, 2, 3, 5, 6, 11, 15, 16, 20, 21, 22, 27, 29, 31, 35, 37, 38, 39, 42, 46, 47, 51, 53, 54, 55, 61, 65, 66, 67};
LocationID topArea[NUM_TOP_AREA] = {4, 21, 22, 23, 27, 33, 38, 39, 41, 48, 61};
LocationID leftArea[NUM_LEFT_AREA] = {1, 4, 5, 7, 11, 15, 30, 37, 40, 43, 55, 56, 63};
LocationID middleArea[NUM_MIDDLE_AREA] = {2, 9, 11, 12, 18, 19, 23, 24, 25, 28, 29, 31, 35, 36, 42, 44, 45, 46, 48, 49, 50, 52, 60, 63, 67, 68, 70};
LocationID rightArea[NUM_RIGHT_AREA] = {3, 8, 10, 13, 14, 17, 20, 26, 32, 34, 54, 57, 58, 59, 62, 65, 66, 68, 69};
LocationID bottomArea[NUM_BOTTOM_AREA] = {0, 3, 6, 16, 24, 29, 32, 42, 43, 47, 53, 54, 64, 65, 67};

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

// check which area is empty and which area dracula should go
int whichAreaToGo(LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// calculate an optimal score for a place which deal with all the factors
int optimalLocScore(int distFromHunt, int numOfNextAdj, int possHunt, int numHunt,
                    int numPossHunt, int locScore, int tuneScore, int distFromSea, int distAdjust);

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
// *needHiding == 2 means DB
// *needHiding == 1 means HI
// *needHiding == 0 means no hiding is needed
int numOfNextAdj(LocationID dest, LocationID trail[TRAIL_SIZE],
                 LocationID hideTrail[TRAIL_SIZE], int *needHiding);

// see if hunters are far away enough from dracula
int isFarAwayEnough(Map map, Map railMap, LocationID *adjLoc, int numLoc, LocationID *path, Round currRound, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// check if the hunter is doing a research
int isResearch(DracView gameState);

//check if drac is surrounded with hunters in adjacent locations excluding his current location
int isSurrounded(LocationID *adjLoc, int numLoc, LocationID dCurrLoc,
                 LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// find the distance between dest and the nearest hunter
int distFromNearestHunter(Map map, Map railMap, LocationID location, LocationID *path, Round round,
                          LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// Finds # moves to get from src -> dest for dracula
int dracPathLength(Map map, LocationID src, LocationID dest, LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *nextLoc, int hideAllow);

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

// find the distance between CD and the nearest hunter
int hunterDistFromCD(Map map, Map railMap, LocationID *path, Round round,
                     LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// check if Dracula should go to CD to gain health
int shouldGoToCD(Map map, Map railMap, LocationID dCurrLoc, LocationID *adjLoc, int numLoc, Round currRound, LocationID *path, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE], int distCurrFromHunt, LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE]);

// check if dracula is in a dangerous situation
int isDangerous(Map map, Map railMap, int *knownLoc, int knownNumLoc,
                int distFromSea, int distFromHunt,
                LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                Round currRound, LocationID *path);

// check if dracula is gonna win
int isGonnaWin(DracView gameState, LocationID fallOffLoc, int health);

// calculate the distance between the nearest sea and the dest
// (need a new ver of findpathlength for drac (cant revisit places))
int distFromNearestSea(Map map, LocationID location, LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE]);

// find the shortest path to escape to a sea and return the next location in that path
// minLength is the shortest length required to go to that sea
LocationID escapeToSea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                       int *minLength, Round currRound, int *knownLoc, int knownNumLoc,
                       int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                       LocationID trail[TRAIL_SIZE], LocationID tmpTrail[TRAIL_SIZE], LocationID tmpHideTrail[TRAIL_SIZE], LocationID *path);

// check if the given dest is a best place among all other places that passed in before
// all min and max variable keep updating if use it in a loop
int isBestLoc(LocationID dest, int possHunt, int *minPossHunt, int numPossHunt,
              int *minNumPossHunt, int numHunt, int *minNumHunt, int length,
              int *minLength, int locScore, int *maxScore);

// find the shortest path to escape to CD and return the next location in that path
// minLength is the shortest length required to go to
LocationID escapeToCD(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                      int *minLength, Round currRound, int *knownLoc, int knownNumLoc,
                      int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                      LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path);

// find the shortest path to escape from a sea to a city
LocationID escapeToCity(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                        Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                        int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                        LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID tmpTrail[TRAIL_SIZE], LocationID tmpHideTrail[TRAIL_SIZE], LocationID *path, int area, int distAdjust);

// find a suitable path to escape to an empty area through sea and return the next location in that path
LocationID escapeThroughSea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                            Round currRound, int *knownLoc, int knownNumLoc,
                            int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                            LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path, int area);

// calculate an optimal score for a place which deal with all the factors when going to an empty area
int bestLocScore(int distFromPlace, int nNextAdj, int possHunt, int numHunt,
                 int numPossHunt, int locScore, int tuneScore, int distFromHunt, int distAdjust);

// find the shortest and suitable path to escape to an empty area and return the next location in that path
LocationID escapeToEmptyArea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                             Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                             int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                             LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path, int area, int distAdjust);

// find the shortest path to escape to right area and return the next location in that path
LocationID escapeToRight(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                         Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                         int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                         LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path);

// find the shortest path to escape to left area and return the next location in that path
LocationID escapeToLeft(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                          Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                          int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                          LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path);

// find the shortest path to escape to bottom area and return the next location in that path
LocationID escapeToBottom(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                          Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                          int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                          LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path);

// find the shortest path to escape to top area and return the next location in that path
LocationID escapeToTop(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                          Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                          int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                          LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path);

// find the shortest path to escape to middle area and return the next location in that path
LocationID escapeToMiddle(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                          Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                          int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                          LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path);

// adjust the optimsl score of a location with special cases considered
// distFromHunt = distFromNearestHunter(dest, path, hTrail)
int scoreTunner(LocationID dest, int numHunt, int possHunt, int needHiding,
                int *knownLoc, int knownNumLoc, int distFromHunt,
                LocationID trail[TRAIL_SIZE]);

// check whether dracula is in concern by the hunter
int isConcerned(Map map, Map railMap, LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                Round currRound, LocationID *path, int knownNumLoc);

// to count the number of land type in adjLoc
int numOfLand(LocationID * adjLoc, int numLoc);

// find the start location which is furthest away from all the hunters
LocationID startLocation(Map map, Map railMap, LocationID *path,
                         LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]);

// keep dracula staying in a sea
LocationID stayInSea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                     Round currRound, int *knownLoc, int knownNumLoc,
                     int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                     LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path);

// determine whether dracula should stay in or leave CD
LocationID stayOrLeaveCD(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                         Round currRound, int *knownLoc, int knownNumLoc,
                         int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                         LocationID trail[TRAIL_SIZE], LocationID tmpTrail[TRAIL_SIZE], LocationID tmpHideTrail[TRAIL_SIZE], LocationID *path);

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
    
    //printf("hhll = %d\n", health);
    for (y = 0 ; y < TRAIL_SIZE ; y++) {
        //printf("trail[%d] = %s\n", y, idToName(trail[y]));
    }
    for (y = 0 ; y < TRAIL_SIZE ; y++) {
        printf("hideTrail[%d] = %d\n", y, hideTrail[y]);
    }
    
    if (currRound == 0) {
        /*PlayerID mostIsolatedHunter = mostIsolated(map, railMap, path, currRound, hTrail);
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
         } else {*/
        //numLoc = NUM_MAP_LOCATIONS;
        adjLoc = malloc(numLoc * sizeof(LocationID));
        //for (x = MIN_MAP_LOCATION; x < NUM_MAP_LOCATIONS; x++) adjLoc[x] = x;
        currLoc = startLocation(map, railMap, path, hTrail);
        submitPlay(gameState, adjLoc, map, railMap, path, currLoc, FALSE,
                   trail, hideTrail, "first move");
        //}
        return;
    } else {
        adjLoc = whereCanIgo(&numLoc, tmpTrail, tmpHideTrail, dCurrLoc, TRUE, TRUE);
        printf("Where am i %s\n", idToName(whereIs(gameState, PLAYER_DRACULA)));
        
        for (y = 0 ; y < numLoc ; y++) {
            printf("I//adjLocations[%d] = %s\n", y, idToName(adjLoc[y]));
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
         }*/printf("sdfsf\n");
        int distCurrFromHunt = distFromNearestHunter(map, railMap, dCurrLoc, path, currRound, hTrail);printf("aaaa\n");
        int distCurrFromSea = distFromNearestSea(map, dCurrLoc, trail, hideTrail);
        printf("sserge\n");
        int area = whichAreaToGo(dCurrLoc, hTrail);
        int distAdjust = 0;
        if (isFarAwayEnough(map, railMap, adjLoc, numLoc, path, currRound, hTrail)) distAdjust = 1;
        // if we are gonna win
        if (currRound > 6) {
            if (isGonnaWin(gameState, fallOffLoc, health)) {
                currLoc = stayInSea(gameState, map, railMap, adjLoc, numLoc,
                                    currRound, knownLoc, knownNumLoc,
                                    health, hTrail, trail, hideTrail, path);
                submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
                           trail, hideTrail, "The world is mine!");
                return;
            }
        }
        printf("after win\n");
        // if we are in dangerous situation
        if (isDangerous(map ,railMap, knownLoc, knownNumLoc, distCurrFromSea, distCurrFromHunt,
                        dCurrLoc, hTrail, currRound, path)) {
            if (idToType(dCurrLoc) == SEA) {
                currLoc = escapeToCity(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, distCurrFromHunt, health, hTrail, trail, hideTrail, tmpTrail, tmpHideTrail, path, area, distAdjust);
            } else {
                int minLength;
                currLoc = escapeToSea(gameState, map, railMap, adjLoc, numLoc, &minLength,
                                  currRound, knownLoc, knownNumLoc, health, hTrail, trail, tmpTrail, tmpHideTrail, path);
            }
            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
                       trail, hideTrail, "I'll be back cause I'm unbeatable!");
            return;
        }
        printf("after danger\n");
        // if we are in CD
        if (dCurrLoc == CASTLE_DRACULA) {
            currLoc = stayOrLeaveCD(gameState, map, railMap, adjLoc, numLoc,
                          currRound, knownLoc, knownNumLoc,
                          health, hTrail, trail, tmpTrail, tmpHideTrail, path);
            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
                       trail, hideTrail, "Always wait you in CD!");
            return;
        }
        printf("after CD\n");
        // if we can go to CD safely
        if (shouldGoToCD(map, railMap, dCurrLoc, adjLoc, numLoc, currRound, path, hTrail, distCurrFromHunt, trail, hideTrail)) {
            printf("after should?\n");
            int minLength;
            currLoc = escapeToCD(gameState, map, railMap, adjLoc, numLoc,
                                  &minLength, currRound, knownLoc, knownNumLoc,
                                 health, hTrail, trail, hideTrail, path);
            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
                       trail, hideTrail, "You'll never know what I'm thinking!");
            return;
        }
        printf("after going\n");
        // go to empty area
//        int inRight = 0, inLeft = 0, inBottom = 0, inTop = 0, inMiddle = 0;
//        if (inArray(rightArea, dCurrLoc, NUM_RIGHT_AREA) != -1) inRight = 1;
//        if (inArray(leftArea, dCurrLoc, NUM_LEFT_AREA) != -1) inLeft = 1;
//        if (inArray(bottomArea, dCurrLoc, NUM_BOTTOM_AREA) != -1) inBottom = 1;
//        if (inArray(topArea, dCurrLoc, NUM_TOP_AREA) != -1) inTop = 1;
//        if (inArray(middleArea, dCurrLoc, NUM_MIDDLE_AREA) != -1) inMiddle = 1;
//    
//        int rightEmpty = 1, leftEmpty = 1, bottomEmpty = 1, topEmpty = 1, middleEmpty = 1;
//        for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
//            if (inArray(rightArea, hTrail[hunter][0], NUM_RIGHT_AREA) != -1) rightEmpty = 0;
//            if (inArray(leftArea, hTrail[hunter][0], NUM_LEFT_AREA) != -1) leftEmpty = 0;
//            if (inArray(bottomArea, hTrail[hunter][0], NUM_BOTTOM_AREA) != -1) bottomEmpty = 0;
//            if (inArray(topArea, hTrail[hunter][0], NUM_TOP_AREA) != -1) topEmpty = 0;
//            if (inArray(middleArea, hTrail[hunter][0], NUM_MIDDLE_AREA) != -1) middleEmpty = 0;
//        }
//        printf("inR=%d, inL=%d,inB=%d,inT=%d,inM=%d,  RE=%d,LE=%d,BE=%d,TE=%d,ME=%d\n", inRight, inLeft, inBottom, inTop, inMiddle, rightEmpty, leftEmpty, bottomEmpty, topEmpty, middleEmpty);
//        if ((rightEmpty == 1 && inRight == 1) || (leftEmpty == 1 && inLeft == 1) || (bottomEmpty == 1 && inBottom == 1) || (topEmpty == 1 && inTop == 1) || (middleEmpty == 1 && inMiddle == 1)) {
//        if (area == CURR_EMPTY) {
//        } else if (area == RIGHT_EMPTY) {
//            currLoc = escapeToRight(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, distCurrFromHunt, health, hTrail, trail, hideTrail, path);
//            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
//                       trail, hideTrail, "I hate wasting time R!");
//            return;
//        } else if (area == LEFT_EMPTY) {
//            currLoc = escapeToLeft(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, distCurrFromHunt, health, hTrail, trail, hideTrail, path);
//            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
//                       trail, hideTrail, "I hate wasting time L!");
//            return;
//        } else if (area == BOTTOM_EMPTY) {
//            currLoc = escapeToBottom(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, distCurrFromHunt, health, hTrail, trail, hideTrail, path);
//            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
//                       trail, hideTrail, "I hate wasting time B!");
//            return;
//        } else if (area == TOP_EMPTY) {
//            currLoc = escapeToTop(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, distCurrFromHunt, health, hTrail, trail, hideTrail, path);
//            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
//                       trail, hideTrail, "I hate wasting time T!");
//            return;
//        } else if (area == MIDDLE_EMPTY) {
//            currLoc = escapeToMiddle(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, distCurrFromHunt, health, hTrail, trail, hideTrail, path);
//            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
//                       trail, hideTrail, "I hate wasting time M!");
//            return;
//        }
        
        
        if (area != CURR_EMPTY && area != -1) {
            currLoc = escapeToEmptyArea(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, distCurrFromHunt, health, hTrail, trail, hideTrail, path, area, distAdjust);
            submitPlay(gameState, adjLoc, map, railMap, path, currLoc, TRUE,
                       trail, hideTrail, "I hate wasting time!");
            return;
        }
        printf("after empty\n");
        
        // other cases
        for (i = 0; i < numLoc; i++) {
            clock_t begin = clock();
            int distFromHunt = distFromNearestHunter(map, railMap, adjLoc[i], path, currRound, hTrail);
            tmpTrail[0] = adjLoc[i];
            tmpHideTrail[0] = adjLoc[i];
            //for (i = 0;i<TRAIL_SIZE;i++) printf("tmphitral[%d] = %s\n", i, idToName(tmpHideTrail[i]));
            for (y = 0 ; y < numLoc ; y++) {
                //("other//adjLocations[%d] = %s\n", y, idToName(adjLoc[y]));
            }
            //printf("i = %d, adjLoc = %s\n", i, idToName(adjLoc[i]));
            int distFromSea = distFromNearestSea(map ,adjLoc[i], tmpTrail, tmpHideTrail);
            printf("distFromSea = %d\n", distFromSea);
            int needHiding;
            printf("ha?\n");
            int nNextAdj = numOfNextAdj(adjLoc[i], trail, hideTrail, &needHiding);
            printf("after nnextasj\n");
            int possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
            printf("after poss\n");
            int numHunt = numOfHunter(adjLoc[i], hTrail);
            printf("after num\n");
            int numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                              knownNumLoc, currRound, trail, hTrail, path);
            int locScore = locationScore(map, adjLoc[i], health, currRound);
            printf("passing in need = %d\n", needHiding);
            int tuneScore = scoreTunner(adjLoc[i], numHunt, possHunt, needHiding,
                                        knownLoc, knownNumLoc, distFromHunt, trail);
            optimalScore = optimalLocScore(distFromHunt, nNextAdj, possHunt, numHunt,
                                           numPossHunt, locScore, tuneScore, distFromSea, distAdjust);
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
    
    for (i = TRAIL_SIZE; i >= 0; i--) {
        for (hunter = PLAYER_LORD_GODALMING; hunter <NUM_HUNTERS && visited != 1; hunter++) {
            for (j = i; j >= 0; j--) {
                if (((hTrail[hunter][j] == trail[i]) && (trail[i] != UNKNOWN_LOCATION) && (idToType(trail[i]) != SEA)) || trail[i] == CASTLE_DRACULA) {
                    printf("knwon:trail[%d] = %s\n", i, idToName(trail[i]));
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

// find the start location which is furthest away from all the hunters
LocationID startLocation(Map map, Map railMap, LocationID *path,
                         LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    PlayerID hunter;
    LocationID location, x;
    int length;
    int nearestLength;
    int maxLength = -1;
    int tmpScore;
    int locScore = 0;
    
    for (x = 0 ; x < NUM_MAP_LOCATIONS; x++) {
        if (idToType(x) == SEA) continue;
        nearestLength = MAX_MAP_LOCATION;
        for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
            length = hunterPathLength(map, railMap, hTrail[hunter][0], x, hunter, 1, path);
            if (length < nearestLength) nearestLength = length;
        }
        //printf("x = %s, nearestlength = %d, maxlength = %d\n", idToName(x), nearestLength, maxLength);
        if (nearestLength > maxLength) {
            maxLength = nearestLength;
            locScore = locationScore(map, x, GAME_START_BLOOD_POINTS, 0);
            location = x;
        } else if (nearestLength == maxLength) {
            tmpScore = locationScore(map, x, GAME_START_BLOOD_POINTS, 0);
            if (tmpScore > locScore) {
                location = x;
                locScore = tmpScore;
            }
        }
    }
    return location;
}

// check which area is empty and which area dracula should go
int whichAreaToGo(LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    PlayerID hunter;
    int inRight = 0, inLeft = 0, inBottom = 0, inTop = 0, inMiddle = 0;
    if (inArray(rightArea, dCurrLoc, NUM_RIGHT_AREA) != -1) inRight = 1;
    if (inArray(leftArea, dCurrLoc, NUM_LEFT_AREA) != -1) inLeft = 1;
    if (inArray(bottomArea, dCurrLoc, NUM_BOTTOM_AREA) != -1) inBottom = 1;
    if (inArray(topArea, dCurrLoc, NUM_TOP_AREA) != -1) inTop = 1;
    if (inArray(middleArea, dCurrLoc, NUM_MIDDLE_AREA) != -1) inMiddle = 1;
    
    int rightEmpty = 1, leftEmpty = 1, bottomEmpty = 1, topEmpty = 1, middleEmpty = 1;
    for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
        if (inArray(rightArea, hTrail[hunter][0], NUM_RIGHT_AREA) != -1) rightEmpty = 0;
        if (inArray(leftArea, hTrail[hunter][0], NUM_LEFT_AREA) != -1) leftEmpty = 0;
        if (inArray(bottomArea, hTrail[hunter][0], NUM_BOTTOM_AREA) != -1) bottomEmpty = 0;
        if (inArray(topArea, hTrail[hunter][0], NUM_TOP_AREA) != -1) topEmpty = 0;
        if (inArray(middleArea, hTrail[hunter][0], NUM_MIDDLE_AREA) != -1) middleEmpty = 0;
    }
    printf("inR=%d, inL=%d,inB=%d,inT=%d,inM=%d,  RE=%d,LE=%d,BE=%d,TE=%d,ME=%d\n", inRight, inLeft, inBottom, inTop, inMiddle, rightEmpty, leftEmpty, bottomEmpty, topEmpty, middleEmpty);
    if ((rightEmpty == 1 && inRight == 1) || (leftEmpty == 1 && inLeft == 1) || (bottomEmpty == 1 && inBottom == 1) || (topEmpty == 1 && inTop == 1) || (middleEmpty == 1 && inMiddle == 1)) {
        return CURR_EMPTY;
    } else if (rightEmpty == 1) {
        return RIGHT_EMPTY;
    } else if (leftEmpty == 1) {
        return LEFT_EMPTY;
    } else if (bottomEmpty == 1) {
        return BOTTOM_EMPTY;
    } else if (topEmpty == 1) {
        return TOP_EMPTY;
    } else if (middleEmpty == 1) {
        return MIDDLE_EMPTY;
    }
    return -1;
}

// calculate an optimal score for a place which deal with all the factors
int optimalLocScore(int distFromHunt, int nNextAdj, int possHunt, int numHunt,
                    int numPossHunt, int locScore, int tuneScore, int distFromSea, int distAdjust) {
    int optimalScore;
    int distFromHuntScore, distFromSeaScore;
    if (distAdjust == 1) distFromHuntScore = 10 * distFromHunt;
    else distFromHuntScore = 48 * distFromHunt;
    if (distFromHunt - distFromSea > 1) distFromSeaScore = 0;
    else distFromSeaScore = 10 * distFromSea;
    optimalScore = distFromHuntScore + 16 * nNextAdj - 4 * possHunt - numHunt - 2 * numPossHunt + locScore + tuneScore - distFromSeaScore;
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
            typeScore = -30;
        } else if (currRound % 13 == 0){
            typeScore = -30;
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
//        int i;printf("hunter=%d\n",hunter);
//        for (i=0;i<numLoc;i++) {
//            printf("possmove=%s\n", idToName(futureMove[i]));
//        }
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
// *needHiding == 2 means DB
// *needHiding == 1 means HI
// *needHiding == 0 means no hiding is needed
int numOfNextAdj(LocationID dest, LocationID trail[TRAIL_SIZE],
                 LocationID hideTrail[TRAIL_SIZE], int *needHiding) {
    //printf("in next adj fun\n");
    int numLoc;
    char *locAbbrev = malloc(3 * sizeof(char));
    idToAbbrev(dest, locAbbrev);
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    
    copyArray(trail, tmpTrail, TRAIL_SIZE);
    copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
       // printf("be4 where i go\n");
    locToDBOrHI(locAbbrev, dest, tmpTrail, tmpHideTrail);
    whereCanIgo(&numLoc, tmpTrail,
                tmpHideTrail, dest, TRUE, TRUE);
    //printf("hideTrail[0] = %d\n", tmpHideTrail[1]);
    if (tmpHideTrail[1] == HIDE) *needHiding = 1;
    else if (tmpHideTrail[1] > HIDE) *needHiding = 2;
    else *needHiding = 0;
    free(locAbbrev);
    return numLoc;
}

/*int teleport adjust() {
    for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
        if (hunterPathLength(map, railMap, hTrail[hunter][0], KLAUSENBURG, hunter, currRound+1, path) <= 2) return 0;
        if (hunterPathLength(map, railMap, hTrail[hunter][0], GALATZ, hunter, currRound+1, path) <= 2) return 0;
    }
    nNextAdj == 0;...
}*/

// see if hunters are far away enough from dracula
int isFarAwayEnough(Map map, Map railMap, LocationID *adjLoc, int numLoc, LocationID *path, Round currRound, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    int distFromHunt;
    int i;
    for (i = 0; i < numLoc; i++) {
        distFromHunt = distFromNearestHunter(map, railMap, adjLoc[i], path, currRound, hTrail);
        if (distFromHunt <= 2) return 0;
    }
    return 1;
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

// Finds # moves to get from src -> dest for dracula
int dracPathLength(Map map, LocationID src, LocationID dest, LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *nextLoc, int hideAllow) {
    if (src == dest) return 0;
    
    int *visited = calloc(map->nV, sizeof(int));
    assert(visited != NULL);
    int *inserted = calloc(map->nV, sizeof(int));
    assert(inserted != NULL);
    Queue q = newQueue();
    QueueJoin(q,src);
    int i, isFound = 0;
    int length = 0;
    int needHiding, type;
    VList cur, dracPath[NUM_MAP_LOCATIONS];
    dracPath[src] = trailAsList(trail, hideTrail);
    visited[src] = 1;
    inserted[src] = 1;
    //printf("src=%s, dest=%s, hideAllow=%d\n", idToName(src), idToName(dest), hideAllow);
    for (cur = dracPath[src]; cur != NULL && cur->v != -1; cur = cur->next) {
       //printf("%s >>", idToName(cur->v));
    }
    //printf("\n");
    
    while (!QueueIsEmpty(q) && !isFound) {
        LocationID x = QueueLeave(q);
        VList curr;
        LocationID y;
        //printf("x = %s\n", idToName(x));
        for (curr = map->connections[x]; curr != NULL; curr = curr->next) {
            if (curr->type == RAIL || curr->v == ST_JOSEPH_AND_ST_MARYS) continue;
            y = curr->v;
            if (visited[y]) continue;
            
            int dbOrHi = hasDBOrHIList(dracPath[x]);
            needHiding = 0;
            type = 0;
            if (dbOrHi != HAS_DOUBLE_BACK || dbOrHi != BOTH_HIDE_AND_DB) {
                cur = dracPath[x];
                for (i = 0; i < TRAIL_SIZE - 1; i++) {
                    if (y == cur->v && cur->type == 0) {
                        needHiding = 1;
                        type = 2;
                    }
                    cur = cur->next;
                }
            }
            cur = dracPath[x];
            for (i = 0; i < TRAIL_SIZE - 1; i++) cur = cur->next;
            while (cur != NULL && cur->v != -1) {
                visited[cur->v] = 0;
                cur = cur->next;
            }
            //printf("y = %s, needHiding = %d\n", idToName(y), needHiding);
            if (hideAllow == 0 && needHiding == 1) continue;
            dracPath[y] = listInsert(dracPath[x], y, type);
            inserted[y] = 1;
            if (y == dest) {
                isFound = 1;
                //printf("isFound\n");
                break;
            }
            if (!visited[y]) {
                QueueJoin(q,y);
                visited[y] = 1;
            }
        }
    }
    if (isFound) {
        cur = dracPath[dest];
        //printf("src is %s, to %s\n", idToName(src), idToName(cur->v));
        //if (cur->next == NULL) printf("NULL\n");
        for (cur = dracPath[dest]; cur != NULL && cur->v != -1; cur = cur->next) {
            //printf("%s >>", idToName(cur->v));
        }
        //printf("\n");
        for (cur = dracPath[dest]; cur->v != src; cur = cur->next) {
            length++;
            *nextLoc = cur->v;
            //printf("%s >", idToName(cur->v));
        }
        //printf("%s\n", idToName(src));
    }
    if (!QueueIsEmpty(q)) dropQueue(q);
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        if (inserted[i]) free(dracPath[i]);
    }
    free(visited);
    free(inserted);
    //printf("dracLength = %d, nextLoc = %s\n", length, idToName(*nextLoc));
    return length;
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
    printf("%dadj = %s\n", hunter, idToName(huntMove));
    free(predictPath);
    return huntMove;
}

// find the distance between CD and the nearest hunter
int hunterDistFromCD(Map map, Map railMap, LocationID *path, Round round,
                     LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE]) {
    PlayerID hunter;
    int length, minLength = NUM_MAP_LOCATIONS;
    
    for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
        length = hunterPathLength(map, railMap, hTrail[hunter][0], CASTLE_DRACULA, hunter, round+1, path);
        if (length < minLength) minLength = length;
    }
    return minLength;
}

// check if Dracula should go to CD to gain health
int shouldGoToCD(Map map, Map railMap, LocationID dCurrLoc, LocationID *adjLoc, int numLoc, Round currRound, LocationID *path, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE], int distCurrFromHunt, LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE]) {
    PlayerID hunter;
    LocationID nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, count = 0;
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) count++;
    }
    free(locAbbrev);
    
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    int lengthToCD = dracPathLength(map, dCurrLoc, CASTLE_DRACULA, trail, hideTrail, &nextLoc, hideAllow);
    //printf("be4 hunt dist\n");
    //int huntDistFromCD = hunterDistFromCD(map, railMap, path, currRound, hTrail);
    
    //printf("after hunt dist\n");
    // avoid being surrounded by hunters in CD
    if (dCurrLoc == KLAUSENBURG || dCurrLoc == GALATZ) {
        for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
            if (hunterPathLength(map, railMap, hTrail[hunter][0], KLAUSENBURG, hunter, currRound+1, path) <= 2) return 0;
            if (hunterPathLength(map, railMap, hTrail[hunter][0], GALATZ, hunter, currRound+1, path) <= 2) return 0;
        }
        return 1;
        //if (huntDistFromCD <= 2) return 0;
    }
    if (lengthToCD <= 4 && distCurrFromHunt > lengthToCD) return 1;
//    for (hunter = PLAYER_LORD_GODALMING; hunter < NUM_HUNTERS; hunter++) {
//        if (inArray(rightArea, hTrail[hunter][0], NUM_RIGHT_AREA) != -1) return 0;
//    }
    return 0;
}

// check if dracula is in a dangerous situation
int isDangerous(Map map, Map railMap, int *knownLoc, int knownNumLoc,
                int distFromSea, int distFromHunt,
                LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                Round currRound, LocationID *path) {
    //if (knownNumLoc > 0) {
    //if (knownLoc[0] < 3) return 1;
    //}
    printf("distfromsea=%d, distfrom hunt = %d, known =%d\n", distFromSea, distFromHunt, knownNumLoc);
    if (distFromSea > 3 && distFromHunt < 2) return 1;
    if (isConcerned(map, railMap, dCurrLoc, hTrail, currRound, path, knownNumLoc)) return 1;
    //for (i = 0; i < numLoc; i++) {
      //  if (distFromNearestHunter(map, railMap, adjLoc[i], path, currRound, hTrail) <= 1);
    //}
    //if (knownNumLoc > 0 && distFromHunt <= 2) return 1;
    if (knownNumLoc > 0) {
        if (knownLoc[0] < 3 && distFromHunt <= 2) {
            printf("knownLoc[0]=%d\n", knownLoc[0]);
            if (inArray(seaLoc, dCurrLoc, NUM_SEA) != -1 && inArray(portCity, dCurrLoc, NUM_PORT_CITY) != -1) return 1;
        }
    }
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
int distFromNearestSea(Map map, LocationID location, LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE]) {
    int i, length, minLength = NUM_MAP_LOCATIONS;
    LocationID nextLoc;
    for (i = 0; i < NUM_SEA; i++) {
        //printf("distFromSea\n");
        length = dracPathLength(map, location, seaLoc[i], trail, hideTrail, &nextLoc, 1);
        if (length < minLength) minLength = length;
    }
    return minLength;
}

// find the shortest path to escape to a sea and return the next location in that path
// minLength is the shortest length required to go to that sea
LocationID escapeToSea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                       int *minLength, Round currRound, int *knownLoc, int knownNumLoc,

                       int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                       LocationID trail[TRAIL_SIZE], LocationID tmpTrail[TRAIL_SIZE], LocationID tmpHideTrail[TRAIL_SIZE], LocationID *path) {
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
        tmpTrail[0] = adjLoc[x];
        tmpHideTrail[0] = adjLoc[x];
        length = distFromNearestSea(map ,adjLoc[x], tmpTrail, tmpHideTrail);
        locScore = locationScore(map, adjLoc[x], health, currRound);
        printf("adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, maxScore = %d\n", idToName(adjLoc[x]), numHunt, numPossHunt, possHunt, length, locScore, maxScore);
        printf("minPossHunt = %d, minNumPossHunt = %d, minNumHunt = %d, minLength = %d, maxScore = %d\n", minPossHunt, minNumPossHunt, minNumHunt, *minLength, maxScore);
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
        *minNumPossHunt = numPossHunt;
        *minNumHunt = numHunt;
        *minLength = length;
        *maxScore = locScore;
        isBest = 1;
    } else if (possHunt == *minPossHunt) {
        if (numPossHunt < *minNumPossHunt) {
            *minNumPossHunt = numPossHunt;
            *minNumHunt = numHunt;
            *minLength = length;
            *maxScore = locScore;
            isBest = 1;
        } else if (numPossHunt == *minNumPossHunt) {
            if (numHunt < *minNumHunt && numHunt == 0) {
                *minNumHunt = numHunt;
                *minLength = length;
                *maxScore = locScore;
                isBest = 1;
            } else {
                if (length < *minLength) {
                    *minLength = length;
                    *maxScore = locScore;
                    isBest = 1;
                } else if (length == *minLength) {
                    if (locScore > *maxScore) {
                        *maxScore = locScore;
                        isBest = 1;
                    }
                }
            }
//            if (numHunt < *minNumHunt) {
//                *minNumHunt = numHunt;
//                if (numHunt == 0) {
//                    *minLength = length;
//                    *maxScore = locScore;
//                    isBest = 1;
//                } else {
//                    if (length < *minLength) {
//                        *minLength = length;
//                        *maxScore = locScore;
//                        isBest = 1;
//                    } else if (length == *minLength) {
//                        if (locScore > *maxScore) {
//                            *maxScore = locScore;
//                            isBest = 1;
//                        }
//                    }
//                }
//            } else if (*minNumHunt != 0) {
//                if (length < *minLength) {
//                    *minLength = length;
//                    *maxScore = locScore;
//                    isBest = 1;
//                } else if (length == *minLength) {
//                    if (locScore > *maxScore) {
//                        *maxScore = locScore;
//                        isBest = 1;
//                    }
//                }
//            }
        }
    }
    return isBest;
}

// find the shortest path to escape to CD and return the next location in that path
// minLength is the shortest length required to go to
LocationID escapeToCD(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                      int *minLength, Round currRound, int *knownLoc, int knownNumLoc,
                      int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                      LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path) {
    int numHunt, minNumHunt = NUM_HUNTERS;
    int numPossHunt, minNumPossHunt = NUM_HUNTERS;
    int possHunt, minPossHunt = NUM_HUNTERS;
    int locScore, maxScore = -1000;
    *minLength = NUM_MAP_LOCATIONS;
    LocationID currLoc, nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, lengthToCD, count = 0;
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) count++;
    }
    free(locAbbrev);
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    
    copyArray(trail, tmpTrail, TRAIL_SIZE);
    copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
    shiftRight(tmpTrail, 0, TRAIL_SIZE - 1);
    shiftRight(tmpHideTrail, 0, TRAIL_SIZE - 1);
    //printf("hideA = %d\n", hideAllow);
    for (i = 0; i < numLoc; i++) {
        //printf("abcd \n");
        numHunt = numOfHunter(adjLoc[i], hTrail);
        //printf("bcde %d\n", numHunt);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        //printf("fghi %d\n", numPossHunt);
        possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
        //printf("xyz %d\n", possHunt);
        tmpTrail[0] = adjLoc[i];
        tmpHideTrail[0] = adjLoc[i];
        lengthToCD = dracPathLength(map, adjLoc[i], CASTLE_DRACULA, tmpTrail, tmpHideTrail, &nextLoc, hideAllow);
        //printf("lmno %d\n", lengthToCD);
        locScore = locationScore(map, adjLoc[i], health, currRound);
        if (isBestLoc(adjLoc[i], possHunt, &minPossHunt, numPossHunt,
                      &minNumPossHunt, numHunt, &minNumHunt, lengthToCD,
                      minLength, locScore, &maxScore))
            currLoc = adjLoc[i];
    }
    return currLoc;
}

// find the shortest path to escape from a sea to a city
LocationID escapeToCity(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                       Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                       int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                       LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID tmpTrail[TRAIL_SIZE], LocationID tmpHideTrail[TRAIL_SIZE], LocationID *path, int area, int distAdjust) {
    int x, nNextAdj, needHiding, length, minLength = NUM_MAP_LOCATIONS;
    int numHunt, minNumHunt = NUM_HUNTERS;
    int numPossHunt, minNumPossHunt = NUM_HUNTERS;
    int possHunt, minPossHunt = NUM_HUNTERS;
    int locScore, maxScore = -1000;
    LocationID currLoc;
    
    for (x = 0; x < numLoc; x++) {
        if (idToType(adjLoc[x]) == SEA) continue;
        numHunt = numOfHunter(adjLoc[x], hTrail);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[x], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        possHunt = possibleHunters(gameState, adjLoc[x], currRound, hTrail);
        tmpTrail[0] = adjLoc[x];
        tmpHideTrail[0] = adjLoc[x];
        length = distFromNearestSea(map ,adjLoc[x], tmpTrail, tmpHideTrail);
        locScore = locationScore(map, adjLoc[x], health, currRound);
        //printf("escape to city: adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, maxScore = %d\n", idToName(adjLoc[x]), numHunt, numPossHunt, possHunt, length, locScore, maxScore);
        //printf("escape to cty : minPossHunt = %d, minNumPossHunt = %d, minNumHunt = %d, minLength = %d, maxScore = %d\n", minPossHunt, minNumPossHunt, minNumHunt, minLength, maxScore);
        if (isBestLoc(adjLoc[x], possHunt, &minPossHunt, numPossHunt,
                      &minNumPossHunt, numHunt, &minNumHunt, length,
                      &minLength, locScore, &maxScore)) {
            currLoc = adjLoc[x];
            nNextAdj = numOfNextAdj(adjLoc[x], trail, hideTrail, &needHiding);
        }
    }
    //printf("needHiding -= %d, nnext = %d\n", needHiding, nNextAdj);
    if (minPossHunt != 0 || needHiding != 0 || nNextAdj <= 3) {
        printf("minPossHunt=%d, needHiding=%d, nNextAdj=%d\n", minPossHunt, needHiding, nNextAdj);
        if (area != CURR_EMPTY && area != -1) {
            currLoc = escapeThroughSea(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, health, hTrail, trail, hideTrail, path, area);
        } else {
            currLoc = stayInSea(gameState, map, railMap, adjLoc, numLoc,
                            currRound, knownLoc, knownNumLoc,
                            health, hTrail, trail, hideTrail, path);
        }
        return currLoc;
    }
    if (area != CURR_EMPTY && area != -1) {
        currLoc = escapeToEmptyArea(gameState, map, railMap, adjLoc, numLoc, currRound, knownLoc, knownNumLoc, distCurrFromHunt, health, hTrail, trail, hideTrail, path, area, distAdjust);
    }
    return currLoc;
}

// calculate an optimal score for a place which deal with all the factors when going to an empty area
int bestLocScore(int distFromPlace, int nNextAdj, int possHunt, int numHunt,
                    int numPossHunt, int locScore, int tuneScore, int distFromHunt, int distAdjust) {
    int bestScore;
    if (distAdjust) distFromHunt = 0;
    bestScore = 200 - 10 * distFromPlace + 5 * nNextAdj - 40 * possHunt - 8 * numHunt - 20 * numPossHunt + locScore + tuneScore + 30 * distFromHunt;
    printf("aaa = %d\n", distFromPlace);
    printf("bbb = %d\n", nNextAdj);
    printf("ccc = %d\n", possHunt);
    printf("ddd = %d\n", numHunt);
    printf("eee = %d\n", numPossHunt);
    printf("fff = %d\n", locScore);
    printf("ggg = %d\n", tuneScore);
    printf("hhh = %d\n", distFromHunt);
    printf("scsg = %d\n", bestScore);
    return bestScore;
}

// find the shortest and suitable path to escape to an empty area and return the next location in that path
LocationID escapeToEmptyArea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                         Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                         int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                         LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path, int area, int distAdjust) {
    int length, minLength = NUM_MAP_LOCATIONS;
    int numHunt, minNumHunt = NUM_HUNTERS;
    int numPossHunt, minNumPossHunt = NUM_HUNTERS;
    int possHunt, minPossHunt = NUM_HUNTERS;
    int locScore, maxScore = -1000;
    LocationID currLoc, nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, count = 0;
    int nNextAdj, needHiding;
    int tmpScore, bestScore = -1000;
    int distFromHunt ,tuneScore;
    LocationID dest = ATLANTIC_OCEAN;
    //int *hideArray = calloc(numLoc, sizeof(int));
    
    if (area == RIGHT_EMPTY) dest = BUCHAREST;
    else if (area == LEFT_EMPTY) dest = SARAGOSSA;
    else if (area == BOTTOM_EMPTY) dest = ROME;
    else if (area == TOP_EMPTY) dest = LONDON;
    else if (area == MIDDLE_EMPTY) dest = ZURICH;
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) {
            count++;
            //hideArray[i] = 1;
        }
    }
    free(locAbbrev);
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    
    for (i = 0; i < numLoc; i++) {
        numHunt = numOfHunter(adjLoc[i], hTrail);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
        tmpTrail[0] = adjLoc[i];
        tmpHideTrail[0] = adjLoc[i];
        length = dracPathLength(map, adjLoc[i], dest, tmpTrail, tmpHideTrail, &nextLoc, hideAllow);
        nNextAdj = numOfNextAdj(adjLoc[i], trail, hideTrail, &needHiding);
        locScore = locationScore(map, adjLoc[i], health, currRound);
        tuneScore = scoreTunner(adjLoc[i], numHunt, possHunt, needHiding,
                                knownLoc, knownNumLoc, distCurrFromHunt, trail);
        if (needHiding != 0/*hideArray[i] == 1*/) locScore -= 40;
        distFromHunt = distFromNearestHunter(map, railMap, adjLoc[i], path, currRound, hTrail);
        printf("EMPTY: adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, tuneScore = %d, dist = %d, adj=%d, area=%d\n", idToName(adjLoc[i]), numHunt, numPossHunt, possHunt, length, locScore, tuneScore, distFromHunt, nNextAdj, area);
        printf("EMPTY: minPossHunt = %d, minNumPossHunt = %d, minNumHunt = %d, minLength = %d, maxScore = %d\n", minPossHunt, minNumPossHunt, minNumHunt, minLength, maxScore);
        tmpScore = bestLocScore(length, nNextAdj, possHunt, numHunt,
                                numPossHunt, locScore, tuneScore, distFromHunt, distAdjust);
        if (tmpScore > bestScore) {
            currLoc = adjLoc[i];
            bestScore = tmpScore;
        }
        //        if (isBestLoc(adjLoc[i], possHunt, &minPossHunt, numPossHunt,
        //                      &minNumPossHunt, numHunt, &minNumHunt, length,
        //                      &minLength, locScore, &maxScore))
        //            currLoc = adjLoc[i];
    }
    printf("currLoc is %d\n", currLoc);
    //free(hideArray);
    return currLoc;
}

// find a suitable path to escape to an empty area through sea and return the next location in that path
LocationID escapeThroughSea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                             Round currRound, int *knownLoc, int knownNumLoc,
                             int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                             LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path, int area) {
    int length;
    int numHunt;
    int numPossHunt;
    int possHunt;
    int locScore;
    LocationID currLoc, nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, count = 0;
    int nNextAdj, needHiding;
    int tmpScore, bestScore = -1000;
    int distFromHunt ,tuneScore = 0;
    LocationID dest = ATLANTIC_OCEAN;
    //int *hideArray = calloc(numLoc, sizeof(int));
    
    if (area == RIGHT_EMPTY) dest = BUCHAREST;
    else if (area == LEFT_EMPTY) dest = SARAGOSSA;
    else if (area == BOTTOM_EMPTY) dest = ROME;
    else if (area == TOP_EMPTY) dest = LONDON;
    else if (area == MIDDLE_EMPTY) dest = ZURICH;
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) {
            count++;
            //hideArray[i] = 1;
        }
    }
    free(locAbbrev);
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    
    for (i = 0; i < numLoc; i++) {
        numHunt = numOfHunter(adjLoc[i], hTrail);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
        tmpTrail[0] = adjLoc[i];
        tmpHideTrail[0] = adjLoc[i];
        length = dracPathLength(map, adjLoc[i], dest, tmpTrail, tmpHideTrail, &nextLoc, hideAllow);
        nNextAdj = numOfNextAdj(adjLoc[i], trail, hideTrail, &needHiding);
        locScore = locationScore(map, adjLoc[i], health, currRound);
        //tuneScore = scoreTunner(adjLoc[i], numHunt, possHunt, needHiding,
          //                      knownLoc, knownNumLoc, distCurrFromHunt, trail);
        if (needHiding != 0/*hideArray[i] == 1*/) locScore -= 40;
        if (idToType(adjLoc[i]) == SEA) locScore += 100;
        distFromHunt = distFromNearestHunter(map, railMap, adjLoc[i], path, currRound, hTrail);
        printf("Through SEA: adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, tuneScore = %d, dist = %d, adj=%d\n", idToName(adjLoc[i]), numHunt, numPossHunt, possHunt, length, locScore, tuneScore, distFromHunt, nNextAdj);
        tmpScore = bestLocScore(length, nNextAdj, possHunt, numHunt,
                                numPossHunt, locScore, tuneScore, distFromHunt, 0);
        if (tmpScore > bestScore) {
            currLoc = adjLoc[i];
            bestScore = tmpScore;
        }
        //        if (isBestLoc(adjLoc[i], possHunt, &minPossHunt, numPossHunt,
        //                      &minNumPossHunt, numHunt, &minNumHunt, length,
        //                      &minLength, locScore, &maxScore))
        //            currLoc = adjLoc[i];
    }
    printf("currLoc is %d\n", currLoc);
    //free(hideArray);
    return currLoc;
}


// find the shortest path to escape to right area and return the next location in that path
LocationID escapeToRight(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                        Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                        int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                        LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path) {
    int length, minLength = NUM_MAP_LOCATIONS;
    int numHunt, minNumHunt = NUM_HUNTERS;
    int numPossHunt, minNumPossHunt = NUM_HUNTERS;
    int possHunt, minPossHunt = NUM_HUNTERS;
    int locScore, maxScore = -1000;
    LocationID currLoc, nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, count = 0;
    int nNextAdj, needHiding;
    int tmpScore, bestScore = -1000;
    int distFromHunt ,tuneScore;
    //int *hideArray = calloc(numLoc, sizeof(int));
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) {
            count++;
            //hideArray[i] = 1;
        }
    }
    free(locAbbrev);
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    
    int distAdjust = 0;
    if (isFarAwayEnough(map, railMap, adjLoc, numLoc, path, currRound, hTrail)) distAdjust = 1;
    
    for (i = 0; i < numLoc; i++) {
        numHunt = numOfHunter(adjLoc[i], hTrail);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
        tmpTrail[0] = adjLoc[i];
        tmpHideTrail[0] = adjLoc[i];
        length = dracPathLength(map, adjLoc[i], BUCHAREST, tmpTrail, tmpHideTrail, &nextLoc, hideAllow);
        nNextAdj = numOfNextAdj(adjLoc[i], trail, hideTrail, &needHiding);
        locScore = locationScore(map, adjLoc[i], health, currRound);
        tuneScore = scoreTunner(adjLoc[i], numHunt, possHunt, needHiding,
                                                knownLoc, knownNumLoc, distCurrFromHunt, trail);
        if (needHiding != 0/*hideArray[i] == 1*/) locScore -= 40;
        distFromHunt = distFromNearestHunter(map, railMap, adjLoc[i], path, currRound, hTrail);
        printf("R: adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, maxScore = %d\n", idToName(adjLoc[i]), numHunt, numPossHunt, possHunt, length, locScore, maxScore);
        printf("R: minPossHunt = %d, minNumPossHunt = %d, minNumHunt = %d, minLength = %d, maxScore = %d\n", minPossHunt, minNumPossHunt, minNumHunt, minLength, maxScore);
        tmpScore = bestLocScore(length, nNextAdj, possHunt, numHunt,
                                numPossHunt, locScore, tuneScore, distFromHunt, distAdjust);
        if (tmpScore > bestScore) {
            currLoc = adjLoc[i];
            bestScore = tmpScore;
        }
//        if (isBestLoc(adjLoc[i], possHunt, &minPossHunt, numPossHunt,
//                      &minNumPossHunt, numHunt, &minNumHunt, length,
//                      &minLength, locScore, &maxScore))
//            currLoc = adjLoc[i];
    }
    printf("currLoc is %d\n", currLoc);
    //free(hideArray);
    return currLoc;
}

// find the shortest path to escape to left area and return the next location in that path
LocationID escapeToLeft(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                       Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                       int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                       LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path) {
    int length, minLength = NUM_MAP_LOCATIONS;
    int numHunt, minNumHunt = NUM_HUNTERS;
    int numPossHunt, minNumPossHunt = NUM_HUNTERS;
    int possHunt, minPossHunt = NUM_HUNTERS;
    int locScore, maxScore = -1000;
    LocationID currLoc, nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, count = 0;
    int *hideArray = calloc(numLoc, sizeof(int));
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) {
            count++;
            hideArray[i] = 1;
        }
    }
    free(locAbbrev);
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    
    for (i = 0; i < numLoc; i++) {
        numHunt = numOfHunter(adjLoc[i], hTrail);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
        tmpTrail[0] = adjLoc[i];
        tmpHideTrail[0] = adjLoc[i];
        length = dracPathLength(map, adjLoc[i], SARAGOSSA, tmpTrail, tmpHideTrail, &nextLoc, hideAllow);
        locScore = locationScore(map, adjLoc[i], health, currRound);
        if (hideArray[i] == 1) locScore -= 50;
        //printf("adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, maxScore = %d\n", idToName(adjLoc[x]), numHunt, numPossHunt, possHunt, length, locScore, maxScore);
        //printf("minPossHunt = %d, minNumPossHunt = %d, minNumHunt = %d, minLength = %d, maxScore = %d\n", minPossHunt, minNumPossHunt, minNumHunt, *minLength, maxScore);
        if (isBestLoc(adjLoc[i], possHunt, &minPossHunt, numPossHunt,
                      &minNumPossHunt, numHunt, &minNumHunt, length,
                      &minLength, locScore, &maxScore))
            currLoc = adjLoc[i];
    }
    //printf("currLoc is %d\n", currLoc);
    free(hideArray);
    return currLoc;
}

// find the shortest path to escape to bottom area and return the next location in that path
LocationID escapeToBottom(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                        Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                        int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                        LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path) {
    int length, minLength = NUM_MAP_LOCATIONS;
    int numHunt, minNumHunt = NUM_HUNTERS;
    int numPossHunt, minNumPossHunt = NUM_HUNTERS;
    int possHunt, minPossHunt = NUM_HUNTERS;
    int locScore, maxScore = -1000;
    LocationID currLoc, nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, count = 0;
    int *hideArray = calloc(numLoc, sizeof(int));
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) {
            count++;
            hideArray[i] = 1;
        }
    }
    free(locAbbrev);
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    
    for (i = 0; i < numLoc; i++) {
        numHunt = numOfHunter(adjLoc[i], hTrail);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
        tmpTrail[0] = adjLoc[i];
        tmpHideTrail[0] = adjLoc[i];
        length = dracPathLength(map, adjLoc[i], ROME, tmpTrail, tmpHideTrail, &nextLoc, hideAllow);
        locScore = locationScore(map, adjLoc[i], health, currRound);
        //printf("adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, maxScore = %d\n", idToName(adjLoc[x]), numHunt, numPossHunt, possHunt, length, locScore, maxScore);
        //printf("minPossHunt = %d, minNumPossHunt = %d, minNumHunt = %d, minLength = %d, maxScore = %d\n", minPossHunt, minNumPossHunt, minNumHunt, *minLength, maxScore);
        if (isBestLoc(adjLoc[i], possHunt, &minPossHunt, numPossHunt,
                      &minNumPossHunt, numHunt, &minNumHunt, length,
                      &minLength, locScore, &maxScore))
            currLoc = adjLoc[i];
    }
    free(hideArray);
    return currLoc;
}

// find the shortest path to escape to top area and return the next location in that path
LocationID escapeToTop(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                        Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                        int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                        LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path) {
    int length, minLength = NUM_MAP_LOCATIONS;
    int numHunt, minNumHunt = NUM_HUNTERS;
    int numPossHunt, minNumPossHunt = NUM_HUNTERS;
    int possHunt, minPossHunt = NUM_HUNTERS;
    int locScore, maxScore = -1000;
    LocationID currLoc, nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, count = 0;
    int *hideArray = calloc(numLoc, sizeof(int));
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) {
            count++;
            hideArray[i] = 1;
        }
    }
    free(locAbbrev);
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    
    for (i = 0; i < numLoc; i++) {
        numHunt = numOfHunter(adjLoc[i], hTrail);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
        tmpTrail[0] = adjLoc[i];
        tmpHideTrail[0] = adjLoc[i];
        length = dracPathLength(map, adjLoc[i], LONDON, tmpTrail, tmpHideTrail, &nextLoc, hideAllow);
        locScore = locationScore(map, adjLoc[i], health, currRound);
        //printf("adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, maxScore = %d\n", idToName(adjLoc[x]), numHunt, numPossHunt, possHunt, length, locScore, maxScore);
        //printf("minPossHunt = %d, minNumPossHunt = %d, minNumHunt = %d, minLength = %d, maxScore = %d\n", minPossHunt, minNumPossHunt, minNumHunt, *minLength, maxScore);
        if (isBestLoc(adjLoc[i], possHunt, &minPossHunt, numPossHunt,
                      &minNumPossHunt, numHunt, &minNumHunt, length,
                      &minLength, locScore, &maxScore))
            currLoc = adjLoc[i];
    }
    free(hideArray);
    return currLoc;
}

// find the shortest path to escape to middle area and return the next location in that path
LocationID escapeToMiddle(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                        Round currRound, int *knownLoc, int knownNumLoc, int distCurrFromHunt,
                        int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                        LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path) {
    int length, minLength = NUM_MAP_LOCATIONS;
    int numHunt, minNumHunt = NUM_HUNTERS;
    int numPossHunt, minNumPossHunt = NUM_HUNTERS;
    int possHunt, minPossHunt = NUM_HUNTERS;
    int locScore, maxScore = -1000;
    LocationID currLoc, nextLoc;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    char *locAbbrev = malloc(3 * sizeof(char));
    int i, hideAllow, count = 0;
    int *hideArray = calloc(numLoc, sizeof(int));
    
    for (i = 0; i < numLoc; i++) {
        idToAbbrev(adjLoc[i], locAbbrev);
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
        if (tmpHideTrail[0] > MAX_MAP_LOCATION) {
            count++;
            hideArray[i] = 1;
        }
    }
    free(locAbbrev);
    if (count >= numLoc - 1) hideAllow = 1;
    else hideAllow = 0;
    
    for (i = 0; i < numLoc; i++) {
        numHunt = numOfHunter(adjLoc[i], hTrail);
        numPossHunt = numOfPossHunter(gameState, map, railMap, adjLoc[i], knownLoc,
                                      knownNumLoc, currRound, trail, hTrail, path);
        possHunt = possibleHunters(gameState, adjLoc[i], currRound, hTrail);
        tmpTrail[0] = adjLoc[i];
        tmpHideTrail[0] = adjLoc[i];
        length = dracPathLength(map, adjLoc[i], ZURICH, tmpTrail, tmpHideTrail, &nextLoc, hideAllow);
        locScore = locationScore(map, adjLoc[i], health, currRound);
        //printf("adjLoc= =%s, numHunt = %d, numPossHunt = %d, possHunt = %d, length = %d, locScore = %d, maxScore = %d\n", idToName(adjLoc[x]), numHunt, numPossHunt, possHunt, length, locScore, maxScore);
        //printf("minPossHunt = %d, minNumPossHunt = %d, minNumHunt = %d, minLength = %d, maxScore = %d\n", minPossHunt, minNumPossHunt, minNumHunt, *minLength, maxScore);
        if (isBestLoc(adjLoc[i], possHunt, &minPossHunt, numPossHunt,
                      &minNumPossHunt, numHunt, &minNumHunt, length,
                      &minLength, locScore, &maxScore))
            currLoc = adjLoc[i];
    }
    free(hideArray);
    return currLoc;
}

// determine whether dracula should stay in or leave CD
LocationID stayOrLeaveCD(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                         Round currRound, int *knownLoc, int knownNumLoc,
                         int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                         LocationID trail[TRAIL_SIZE], LocationID tmpTrail[TRAIL_SIZE], LocationID tmpHideTrail[TRAIL_SIZE], LocationID *path) {
    LocationID currLoc;
    int huntDistFromCD = hunterDistFromCD(map, railMap, path, currRound, hTrail);
    
    if (huntDistFromCD > 3) {
        if (inArray(adjLoc, CASTLE_DRACULA, numLoc) != -1) {
            currLoc = CASTLE_DRACULA;
            return currLoc;
        }
    }
    int minLength;
    currLoc = escapeToSea(gameState, map, railMap, adjLoc, numLoc,
                         &minLength, currRound, knownLoc, knownNumLoc,
                         health, hTrail, trail, tmpTrail, tmpHideTrail, path);
    return currLoc;
}

// adjust the optimsl score of a location with special cases considered
int scoreTunner(LocationID dest, int numHunt, int possHunt, int needHiding,
                int *knownLoc, int knownNumLoc, int distFromHunt,
                LocationID trail[TRAIL_SIZE]) {
    int i;
    int tuneScore = 0;
    
    // case: if current location is known by hunter
    if (knownNumLoc != 0) {
        if (knownLoc[0] == 0) {
            if (needHiding == 0) {
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
    if ((knownNumLoc == 0 && distFromHunt > 1) || distFromHunt > 2) {
        if (needHiding == 0) tuneScore += 100;
        else if (needHiding == 1) tuneScore -= 80;
        else tuneScore -= 120;
        if (idToType(dest) == SEA) tuneScore -= 40;
    }
    
    //
    if (possHunt == 0 && needHiding == 0) tuneScore += 40;
    //printf("needhiding = %d, lcoation = %s, tuneScore = %d\n", needHiding, idToName(dest), tuneScore);
    return tuneScore;
}

// check whether dracula is in concern by the hunter
int isConcerned(Map map, Map railMap, LocationID dCurrLoc, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                Round currRound, LocationID *path, int knownNumLoc) {
    PlayerID hunter;
    int count = 0;
    
    if (knownNumLoc == 0) return 0;
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

// keep dracula in sea
LocationID stayInSea(DracView gameState, Map map, Map railMap, LocationID *adjLoc, int numLoc,
                     Round currRound, int *knownLoc, int knownNumLoc,
                     int health, LocationID hTrail[NUM_HUNTERS][TRAIL_SIZE],
                     LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE], LocationID *path) {
    LocationID best = -1;
    LocationID tmpHideTrail[TRAIL_SIZE];
    LocationID tmpTrail[TRAIL_SIZE];
    int distFromHunt, maxDistFromHunt = -1;
    
    if (idToType(trail[0]) == SEA) {
        int needHiding, adjNumLoc, nAdjSea = 0;
        char *locAbbrev = malloc(3 * sizeof(char));
        LocationID *nextAdjLoc;
        int i, j, count;
        
        for (i = 0; i < numLoc; i++) {
            if (idToType(adjLoc[i]) != SEA) continue;
            idToAbbrev(adjLoc[i], locAbbrev);
            copyArray(trail, tmpTrail, TRAIL_SIZE);
            copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
            locToDBOrHI(locAbbrev, adjLoc[i], tmpTrail, tmpHideTrail);
            nextAdjLoc = whereCanIgo(&adjNumLoc, tmpTrail,
                                     tmpHideTrail, adjLoc[i], TRUE, TRUE);
            if (tmpHideTrail[0] > MAX_MAP_LOCATION) needHiding = 1;
            else needHiding = 0;
            count = 0;
            for (j = 0; j < adjNumLoc; j++) {
                if (idToType(nextAdjLoc[j]) == SEA) count++;
            }
            distFromHunt = distFromNearestHunter(map, railMap, adjLoc[i], path, currRound, hTrail);
            printf("place=%s, dist=%d\n", idToName(adjLoc[i]), distFromHunt);
            if (distFromHunt > maxDistFromHunt) {
                maxDistFromHunt = distFromHunt;
                best = adjLoc[i];
                nAdjSea = count;
            } else if (distFromHunt == maxDistFromHunt) {
                if (count > nAdjSea) {
                    best = adjLoc[i];
                    nAdjSea = count;
                } else if (count == nAdjSea) {
                    if (!needHiding) best = adjLoc[i];
                }
            }
        }
        free(locAbbrev);
    }
    if (best == -1) {
        int minLength;
        copyArray(trail, tmpTrail, TRAIL_SIZE);
        copyArray(hideTrail, tmpHideTrail, TRAIL_SIZE);
        shiftRight(tmpTrail, 0, TRAIL_SIZE - 1);
        shiftRight(tmpHideTrail, 0, TRAIL_SIZE - 1);
        best = escapeToSea(gameState, map, railMap, adjLoc, numLoc,
                           &minLength, currRound, knownLoc, knownNumLoc,
                           health, hTrail, trail, tmpTrail, tmpHideTrail, path);
    }
    return best;
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

/*// find the adjacent location which is furthest away from all the hunters
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
 //printf ("fur adjLocations[%d] = %s\n", x, idToName(adjLocations[x]));
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
 }*/
