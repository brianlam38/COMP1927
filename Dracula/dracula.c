// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
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

void decideDraculaMove(DracView gameState)
{
 char *location;

 if (giveMeTheRound(gameState) == 0) {
		// if (islandEmpty(gameState)) {
		// 	registerBestPlay("MN", "first move");
		// } else
		if (atLeastOneIsolated(&location, gameState)) {
			registerBestPlay(location, "first move");
		} else {
			location = idToName(optimalStartLocation(gameState));
			registerBestPlay(location, "first move");
		}
		return;
	}

	int numLoc;
	LocationID *adjLocations = whereCanIgo(gameState, &numLoc, ROAD, SEA);
	location = locationFurthestFromNearest(gameState, adjLocations, numLoc,
																					PLAYER_DRACULA);
	registerBestPlay(location, "other moves");
	return;
}

char *locationFurthestFromNearest(DracView gameState, LocationID *adjLocations,
																	int numLoc, PlayerID player) {
	int otherHunters;
  char *location;
	int length = 0;
	int x;
	for (x = 0 ; x < numLoc ; x++) {
				int lengthToNearest = -1;
				for (otherHunters = 0; otherHunters < 4; otherHunters++) {
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
	int otherPlayers;
	for (otherPlayers = 0 ; otherPlayers < 4; otherPlayers++) {
		if ((otherPlayers != player) && findPathLength(whereIs(gameState, otherPlayers),
		 															whereIs(gameState, player)) <= 2 ) return 0;
	}
	return 1;
}

int islandEmpty(DracView gameState) {
	int players;
	LocationID location;
	for (players = 0 ; players < 4; players++) {
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

// char *optimalLocation(DracView gameState, LocationID *adjLocations, int numLoc,
// 											PlayerID player) {
// 	int otherHunters;
//   char *location;
// 	int length = 0;
// 	int x;
// 	for (x = 0 ; x < numLoc ; x++) {
// 				int lengthToNearest = -1;
// 				for (otherHunters = 0; otherHunters < 4; otherHunters++) {
// 					if ((findPathLength(adjLocations[x], whereIs(gameState,
// 							otherHunters)) <= lengthToNearest) && (player != otherHunters) ||
// 							lengthToNearest = -1){
// 								lengthToNearest = findPathLength(adjLocations[x],
// 									whereIs(gameState, otherHunters));
// 					}
// 				}
// 				if (length <= lengthToNearest) {
// 					length = lengthToNearest;
// 					location = idToName(adjLocations[x]);
// 				}
// 	}
// 	return location;
// }