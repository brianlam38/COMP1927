// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include "Places.h"
#include "Map.h"
#include "commonFunctions.h"

int findPathLength(Map map, LocationID src, LocationID dest, int *path);

void decideDraculaMove(DracView gameState)
{
	// TODO ...
	// Replace the line below by something better
	//registerBestPlay("CD","Mwuhahahaha");

	int numLoc;
	if (giveMeTheRound(gameState) == 0) {
		registerBestPlay("CD", "first move"); // use a randomiser?
		return;
	}
	LocationID *adjLocations = whereCanIgo(gameState, &numLoc, ROAD, SEA);
	LocationID trail[TRAIL_SIZE];
	giveMeTheTrail(gameState, PLAYER_DRACULA, trail);
	char *location;
	//int i = 0;
	// for (; i < numLoc ; i++) {
	// 			int y = 0;
	// 			for (; y < TRAIL_SIZE-1 ; y++) {
	// 				if (adjLocations[i] != trail[y]) location = idToName(adjLocations[i]);
	// 			}
	// }

	int path[72];
	Map map = newMap();
	int x = 0;
	int length = 0;
	for (; x < numLoc ; x++) {
				printf("%d %s\n", adjLocations[x], idToName(adjLocations[x]));
				int player = 0;
				for (; player < 4; player++) {
					if (findPathLength(map, adjLocations[x], whereIs(gameState, player), path) >= length){
						length = findPathLength(map, adjLocations[x], whereIs(gameState, player), path);
						location = idToName(adjLocations[x]);
					}
				}
	}

	registerBestPlay(location, "other moves");

	return;

}

int findPathLength(Map map, LocationID src, LocationID dest, int *path)
{
		if (src == dest) {
			path[0] =src;
			return 0;
		}
    int *visited = calloc(map->nV,sizeof(int));
    LocationID *tmppath = calloc(map->nV,sizeof(LocationID));
    Queue q = newQueue();
    QueueJoin(q,src);
    int isFound = 0;
    int length = 0;
    while (!QueueIsEmpty(q) && !isFound) {
        LocationID x = QueueLeave(q);
        visited[x] = 1;
				VList curr;
				LocationID y;
        for (curr = map->connections[x]; curr != NULL; curr = curr->next) {
					y = curr->v;
          if (visited[y]) continue;
            tmppath[y] = x;
            if (y == dest) {
                isFound = 1;
                break;
            }
            if (!visited[y]) {
                QueueJoin(q,y);
                visited[y] = 1;
            }
        }
    }
    if (isFound) {
      LocationID v;
      for (v = dest; v != src; v = tmppath[v]) {
				length++;
			}
    }
    return length;
}
