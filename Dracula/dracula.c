// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Game.h"
#include "DracView.h"
#include "Places.h"
#include "Map.h"
#include "commonFunctions.h"

char *locationFurthestFromNearest(DracView gameState, LocationID *adjLocations,
																	int numLoc, PlayerID player);

int isIsolated(PlayerID player, DracView gameState);

int islandEmpty(DracView gameState);

int atLeastOneIsolated(char **location, DracView gameState); // sets location
		//adjacent to most isolated player  in a positionn furthest from other players

LocationID optimalStartLocation(DracView gameState);

PlayerID mostIsolatedPlayer(DracView gameState);

char *optimalLocation(DracView gameState, LocationID *adjLocations,
																	int numLoc, PlayerID player);

char *convertToDbOrHide (char *locAbbrev, DracView currentView);

void decideDraculaMove(DracView gameState)
{
	char *location;
	LocationID loc;
	LocationID trail[TRAIL_SIZE];
	giveMeTheTrail(gameState, PLAYER_DRACULA, trail);

	int y;
	for (y = 0 ; y < TRAIL_SIZE ; y++) {
		 printf("trail[%d] = %s\n", y, idToName(trail[y]));
	}
		char locAbbrev[2];
	  if (giveMeTheRound(gameState) == 0) {
			if (islandEmpty(gameState)) {
				registerBestPlay("ED", "first move");
			} else if (atLeastOneIsolated(&location, gameState)) {
				loc = nameToID(location);
				idToAbbrev(loc, locAbbrev);
				registerBestPlay(locAbbrev, "first move");
			} else {
				location = idToName(optimalStartLocation(gameState));
				loc = nameToID(location);
				idToAbbrev(loc, locAbbrev);
				registerBestPlay(locAbbrev, "first move");
			}
			return;
		} else {
		int numLoc;
		LocationID *adjLocations = whereCanIgo(gameState, &numLoc, ROAD, SEA);
		printf("Where am i %s\n", idToName(whereIs(gameState, PLAYER_DRACULA)));

	 	 for (y = 0 ; y < numLoc ; y++) {
	 	 		printf("adjLocations[%d] = %s\n", y, idToName(adjLocations[y]));
	 	 }
		if (numLoc == 0) {
			registerBestPlay("TP", "other moves");
		}

		location = locationFurthestFromNearest(gameState, adjLocations, numLoc,
																						PLAYER_DRACULA);
		loc = nameToID(location);
		idToAbbrev(loc, locAbbrev);
		convertToDbOrHide (locAbbrev, gameState);
		printf("best play %s\n", location);
		registerBestPlay(locAbbrev, "other moves");
		return;
    }
}

char *locationFurthestFromNearest(DracView gameState, LocationID *adjLocations,
																	int numLoc, PlayerID player) {
	int otherHunters;
  char *location;
	int length = 0;
	int x;
	for (x = 0 ; x < numLoc ; x++) {
				int lengthToNearest = -1;
				for (otherHunters = PLAYER_LORD_GODALMING; otherHunters <= PLAYER_MINA_HARKER; otherHunters++) {
					if (((findPathLength(adjLocations[x], whereIs(gameState,
							otherHunters)) <= lengthToNearest) && (player != otherHunters)) ||
							lengthToNearest == -1){
								lengthToNearest = findPathLength(adjLocations[x],
									whereIs(gameState, otherHunters));
					}

				}

				if (length <= lengthToNearest) {
					length = lengthToNearest;
					location = idToName(adjLocations[x]);
				}
	}

	return location;
}

int isIsolated(PlayerID player, DracView gameState) {
	PlayerID otherPlayers;
	for (otherPlayers = PLAYER_LORD_GODALMING ; otherPlayers <= PLAYER_MINA_HARKER; otherPlayers++) {
		if ((otherPlayers != player) && findPathLength(whereIs(gameState, otherPlayers),
		 															whereIs(gameState, player)) <= 2 ) return 0;
	}
	return 1;
}

int islandEmpty(DracView gameState) {
	PlayerID players;
	LocationID location;
	for (players = PLAYER_LORD_GODALMING ; players <= PLAYER_MINA_HARKER; players++) {
		location = whereIs(gameState, players);
		if (location == EDINBURGH || location == MANCHESTER || location == LIVERPOOL
				|| location == SWANSEA	|| location == LONDON || location == PLYMOUTH)
				return 0;
	}
	return 1;
}


int atLeastOneIsolated(char **location, DracView gameState) {
	int numLoc;
	PlayerID player;
	int distance = 0;
	for (player = 0; player < 4; player++) {
		LocationID *adjLocations = whereCanTheyGo(gameState, &numLoc, player,
																							ROAD, RAIL, SEA);
		LocationID otherLocation = nameToID(locationFurthestFromNearest(gameState,
																				adjLocations, numLoc, player));
		int nearestHunterDistance = findPathLength(otherLocation,
																								whereIs(gameState, player));
		if (isIsolated(player, gameState) && distance < nearestHunterDistance) {
			distance = nearestHunterDistance;
			*location = idToName(whereIs(gameState, player));
		}
	}
	if (distance != 0) {
		player = mostIsolatedPlayer(gameState);
		LocationID *adjLocations = whereCanTheyGo(gameState, &numLoc, player, ROAD, RAIL, SEA);
		*location = locationFurthestFromNearest(gameState, adjLocations, numLoc, player);
	}
	return distance != 0 ? 1 : 0;
}

PlayerID mostIsolatedPlayer( DracView gameState) {
	int numLoc;
	PlayerID player;
	int distance = 0;
	PlayerID mostIsolatedPlayer = -1;
	for (player = 0; player < 4; player++) {
		LocationID *adjLocations = whereCanTheyGo(gameState, &numLoc, player,
																							ROAD, RAIL, SEA);
		LocationID otherLocation = nameToID(locationFurthestFromNearest(gameState,
																				adjLocations, numLoc, player));
		int nearestHunterDistance = findPathLength(otherLocation,
																								whereIs(gameState, player));
		if (isIsolated(player, gameState) && distance < nearestHunterDistance) {
			distance = nearestHunterDistance;
			mostIsolatedPlayer = player;
		}
	}
	return mostIsolatedPlayer;
}

LocationID optimalStartLocation(DracView gameState) {
	LocationID tempLocation;
	LocationID location;
	int total=0;
	int tempTotal;
	int players;
	for (tempLocation = 0; tempLocation < 71; tempLocation++) {
		tempTotal = 0;
		players = 0;
		for ( ; players < 4 ; players++ ) {
			tempTotal += findPathLength(tempLocation, whereIs(gameState, players));
		}
		if (total < tempTotal) {
			total = tempTotal;
			location = tempLocation;
		}
	}
	return location;
}

char *optimalLocation(DracView gameState, LocationID *adjLocations, int numLoc,
											PlayerID player) {
	int otherHunters;
  char *location;
	int total = -1;
	int tempTotal = 0;
	PQueue pq = newPQueue();
	int x;
	for (x = 0 ; x < numLoc ; x++) {
		tempTotal = 0;
		for (otherHunters = 0; otherHunters < 4; otherHunters++) {
			if (player != otherHunters){
				PQueueJoin(pq, adjLocations[x], findPathLength(adjLocations[x],
										whereIs(gameState, otherHunters)));
			}
		}
		int order = 0; //adjust weighting here
		while(!PQueueIsEmpty(pq)) {
		 	if (order == 0) tempTotal += PQueueLeave(pq) * 40; //weighing 1
			if (order == 1) tempTotal += PQueueLeave(pq) * 30; //weighing 2
			if (order == 2) tempTotal += PQueueLeave(pq) * 20; //weighing 3
			if (order == 3) tempTotal += PQueueLeave(pq) * 10; //weighing 4
			order++;
		}
		if (total > tempTotal || total == -1) {
			total = tempTotal;
			location = idToName(adjLocations[x]);
			printf("tempTotal: %d\n", tempTotal);
			printf("location: %s\n", location);
		}
	}
	return location;
}

char *convertToDbOrHide (char *locAbbrev, DracView currentView) {
	LocationID trail[TRAIL_SIZE];
	giveMeTheTrail(currentView, PLAYER_DRACULA, trail);
	if (abbrevToID(locAbbrev) == trail[0] && (hasDBOrHI(trail, DRAC_VIEW) !=
	 		HAS_HIDE)) {
		strcpy(locAbbrev, "HI");
	} else if (abbrevToID(locAbbrev) == trail[0]) {
		strcpy(locAbbrev, "D1");;
	} else if (abbrevToID(locAbbrev) == trail[1]) {
		strcpy(locAbbrev, "D2");
	} else if (abbrevToID(locAbbrev) == trail[2]) {
		strcpy(locAbbrev, "D3");
	} else if (abbrevToID(locAbbrev) == trail[3]) {
		strcpy(locAbbrev, "D4");
	} else if (abbrevToID(locAbbrev) == trail[4]) {
		strcpy(locAbbrev, "D5");
	}
	return locAbbrev;
}
