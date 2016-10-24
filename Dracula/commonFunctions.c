// commonFunctions.c ... common functions implementation
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "commonFunctions.h"
#include "Map.h"
#include "Places.h"
#include "Globals.h"
#include "GameView.h"
#include <time.h>

//count the number of nearby cities of a specified location and
// store the nearby cities in an array
LocationID *NearbyCities(Map map, LocationID from,
                         LocationID *nearby, int *size, int type) {
    VList curr;

    // traverse the neighbour of the city and renew the size and
    //  array when a new nearby city of type "type" is found
    for(curr = map->connections[from]; curr != NULL; curr = curr->next) {
        if (curr->type == type || curr->type == ANY) {
            (*size)++;
            nearby = realloc(nearby, (*size) * sizeof(LocationID));
            assert(nearby != NULL);
            nearby[*size - 1] = curr->v;
        }
    }
    return nearby;
}

//check if an element is in the given array
int inArray(int *array, int object, int size) {
    int i;
    for(i = 0; i < size; i++) {
        if (array[i] == object) return i;
    }
    return -1;
}

// shift the array to the left
void shiftLeft(LocationID *array, int start, int end) {
    int i;
    for (i = start; i < end; i++)
        array[i] = array[i + 1];
}

// shift the array to the right
void shiftRight(LocationID *array, int start, int end) {
    int i;
    for (i = end; i > start; i--)
        array[i] = array[i - 1];
}

//given an "other" location abbreviation, return its ID number
LocationID otherToID(char *abbrev) {
    if (strcmp(abbrev, "C?") == 0) {
        return CITY_UNKNOWN;
    } else if (strcmp(abbrev,"S?") == 0) {
        return SEA_UNKNOWN;
    } else if (strcmp(abbrev,"HI") == 0) {
        return HIDE;
    } else if (strcmp(abbrev,"D1") == 0) {
        return DOUBLE_BACK_1;
    } else if (strcmp(abbrev,"D2") == 0) {
        return DOUBLE_BACK_2;
    } else if (strcmp(abbrev,"D3") == 0) {
        return DOUBLE_BACK_3;
    } else if (strcmp(abbrev,"D4") == 0) {
        return DOUBLE_BACK_4;
    } else if (strcmp(abbrev,"D5") == 0) {
        return DOUBLE_BACK_5;
    } else if (strcmp(abbrev,"TP") == 0) {
        return TELEPORT;
    } else {
        return abbrevToID(abbrev);
    }
}

//To count the number of chars in a string
int countChar(char* string) {
    char *p;
    int nChar = 0;
    for (p = string; *p != '\0'; p++) {
        nChar++;
    }
    return nChar;
}

//get the abbreviation of location from a play
void strToAbbrev(char *play, char abbrev[]) {
    abbrev[0] = play[1];
    abbrev[1] = play[2];
    abbrev[2] = '\0';
    return;
}

//get the health of hunter in each turn
int hunterTurnHealth(char *pastPlays, int health, LocationID prevLocation,
                     LocationID currLocation) {
    assert(pastPlays != NULL);
    assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    assert(validPlace(prevLocation) || prevLocation == UNKNOWN_LOCATION);
    assert(validPlace(currLocation) || currLocation == UNKNOWN_LOCATION);

    char *p = pastPlays;
    int i;

    // check if the previous and the current location is the same
    //  they gain health from rest if the same

    p += 3;
    // loop each time a hunter has encountered an event and
    //  update the health when they lost life
    for (i = 0; i <= NUM_EVENT_ENCOUNTER; i++) {
        switch(*p) {
            case 'T' :
            health -= LIFE_LOSS_TRAP_ENCOUNTER;
            break;
            case 'D' :
            health -= LIFE_LOSS_DRACULA_ENCOUNTER;
            break;
            default :
            break;
        }
        p++;
    }

    if (health <= 0) return 0;

    if (currLocation == prevLocation) {
        health += LIFE_GAIN_REST;
        if (health > GAME_START_HUNTER_LIFE_POINTS) {
            health = GAME_START_HUNTER_LIFE_POINTS;
        }
    }
    assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    return health;
}

//initialise all the trail locations to UNKNOWN_LOCATION
void initialiseTrail(LocationID trail[TRAIL_SIZE]) {
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) trail[i] = UNKNOWN_LOCATION;
}

//updates player's trail by inserting new location at the start
void updatePlayerTrail(LocationID trail[TRAIL_SIZE],
                       char *newLocation, PlayerID player) {
    int i;
    LocationID currID;
    char currLocation[3];

    strToAbbrev(newLocation, currLocation);
    // delete the oldest location and add the new one
    for (i = TRAIL_SIZE - 1; i > 0; i--)
        trail[i] = trail[i - 1];
    if (player == PLAYER_DRACULA) {
        // change the "other" location to actual location of Dracula
        //  as Dracula always knows where he is
        currID = dracSpecialLocation(otherToID(currLocation), trail);
    } else {
        currID = otherToID(currLocation);
    }
    trail[i] = currID;
}

//get the actual location ID for dracula if
// his move is a hide or double back
LocationID dracSpecialLocation(LocationID currID,
                               LocationID trail[TRAIL_SIZE]) {
    switch (currID) {
        case HIDE:
            currID = trail[1];
            break;
        case DOUBLE_BACK_1:
            currID = trail[1];
            break;
        case DOUBLE_BACK_2:
            currID = trail[2];
            break;
        case DOUBLE_BACK_3:
            currID = trail[3];
            break;
        case DOUBLE_BACK_4:
            currID = trail[4];
            break;
        case DOUBLE_BACK_5:
            currID = trail[5];
            break;
        case TELEPORT:
            currID = CASTLE_DRACULA;
            break;
        default:
            break;
    }
    return currID;
}

//check and update the number of traps and
// number of immature vampires in a given location
void numEncounter(LocationID trail[TRAIL_SIZE], char c,
                  LocationID where, int *numTraps, int *numVamps) {
    assert(validPlace(where) && idToType(where) != SEA);
    int i;

    // check if "where" is in the trail
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (where == trail[i]) i = TRAIL_SIZE;
    }

    if (i != TRAIL_SIZE + 1) {
        // number of all encounters are reset to 0
        //  if "where" is already out of the trail
        *numTraps = 0;
        *numVamps = 0;
    } else {
        if (c == 'T') (*numTraps)++;
        if (c == 'V') (*numVamps)++;
    }
    return;
}

// create new empty Queue
Queue newQueue()
{
    Queue q;
    q = malloc(sizeof(QueueRep));
    assert(q != NULL);
    q->head = NULL;
    q->tail = NULL;
    return q;
}

// free memory used by Queue
void dropQueue(Queue Q)
{
    QueueNode *curr, *next;
    assert(Q != NULL);
    // free list nodes
    curr = Q->head;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    // free queue rep
    free(Q);
}

// display as 3 > 5 > 4 > ...
void showQueue(Queue Q)
{
    QueueNode *curr;
    assert(Q != NULL);
    // free list nodes
    curr = Q->head;
    while (curr != NULL) {
        printf("%d",curr->value);
        if (curr->next != NULL)
            printf(">");
        curr = curr->next;
    }
    printf("\n");
}

// add LocationID at end of Queue
void QueueJoin(Queue Q, LocationID it)
{
    assert(Q != NULL);
    QueueNode *new = malloc(sizeof(QueueNode));
    assert(new != NULL);
    new->value = it;
    new->next = NULL;
    if (Q->head == NULL)
        Q->head = new;
    if (Q->tail != NULL)
        Q->tail->next = new;
    Q->tail = new;
}

// remove LocationID from front of Queue
LocationID QueueLeave(Queue Q)
{
    assert(Q != NULL);
    assert(Q->head != NULL);
    LocationID it = Q->head->value;
    QueueNode *old = Q->head;
    Q->head = old->next;
    if (Q->head == NULL)
        Q->tail = NULL;
    //free(old);
    return it;
}

// check for no LocationIDs
int QueueIsEmpty(Queue Q)
{
    return (Q->head == NULL);
}

// ###########################
// FUNCTIONS UNIQUE TO DRACULA
// ###########################


LocationID howToGetTo(LocationID dest, LocationID from, int round,
                             int player, int *pathLength, int sea, int train) {

    if (dest == from) return dest;

    LocationID seenList[NUM_MAP_LOCATIONS] = {0};
    LocationID prevList[NUM_MAP_LOCATIONS] = {0};
    LocationID stepList[NUM_MAP_LOCATIONS] = {0}; 
    Queue toVisit = newQueue();
    Map map = newRailMap();
//    int steps = (round+player)%4; 

    seenList[from] = 1 + train - train;
    prevList[from] = -1;

    if (train)                                    // move by train
        stepList[from] = (round+player)%4;
    QueueJoin(toVisit,from);
    VList i;
  
    while(!QueueIsEmpty(toVisit) && !seenList[dest]) {      // while queue !empty & dest !reached
  
        LocationID curr = QueueLeave(toVisit);
//    printf("Curr = %d\n",curr);

        for (i = map->connections[curr]; i != NULL; i=i->next) {                // loop through adj cities (next moves)

            if ((i->type == BOAT && sea) || i->type == ROAD || (train && stepList[curr]+ RAIL_0 >= i->type)) {
                if (!seenList[i->v]) {                // if location has not been seen:
                    seenList[i->v] = 1;                   // Mark location as seen = 1
                    prevList[i->v] = curr;                // Store location in prevList
                    if (train)                                       
                        stepList[i->v] = (stepList[curr] + 1)%4;  
                }
                if (seenList[dest]) break;                      // if dest is found, break loop
                QueueJoin(toVisit,i->v);              // add connection to queue
            }
        }
    }
    dropQueue(toVisit);
    LocationID curr = dest;
    while (prevList[curr] != from) {    // Traversing back
        curr = prevList[curr];
	(*pathLength)++;
    }

    return curr;
}

int findMostCommon(int *array,int size) {
    int Locations[NUM_MAP_LOCATIONS] = {0};
    int i;

    for (i = 0; i < size; i++) {
        if (array[i] != -1)
            Locations[array[i]]++;
    }
    int mode = 0;
    for (i = 0; i < NUM_MAP_LOCATIONS; i++) {
        if (Locations[i] >= mode) {
            mode = i;
        }
    }
    if (Locations[i] > 1)
        return mode;
    else return -1;
}

// Finds # moves to get from src -> dest
int simpleFindPathLength( LocationID src, LocationID dest)
{
    Map map = newMap();
        if (src == dest) return 0;
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









/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////

// create new empty Queue
PQueue newPQueue()
{
    PQueue pq;
    pq = malloc(sizeof(PQueueRep));
    assert(pq != NULL);
    pq->head = NULL;
    pq->tail = NULL;
    return pq;
}

// free memory used by Queue
void dropPQueue(PQueue PQ)
{
    PQueueNode *curr, *next;
    assert(PQ != NULL);
    // free list nodes
    curr = PQ->head;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    // free queue rep
    free(PQ);
}

// display as 3 > 5 > 4 > ...
void showPQueue(PQueue PQ)
{
    PQueueNode *curr;
    assert(PQ != NULL);
    // free list nodes
    curr = PQ->head;
    while (curr != NULL) {
        printf("location: %d",curr->value);
        printf("distance from: %d",curr->distance);
        if (curr->next != NULL)
            printf(">");
        curr = curr->next;
    }
    printf("\n");
}

// add LocationID at end of Queue
void PQueueJoin(PQueue PQ, LocationID it, int distance)
{
    assert(PQ != NULL);
    PQueueNode *new = malloc(sizeof(PQueueNode));
    assert(new != NULL);
    new->value = it;
    new->distance = distance;
    new->next = NULL;
    if (PQ->head == NULL) {
        PQ->head = new;
        PQ->tail = new;
        return;
    }
    PQueueNode *curr = PQ->head;
    if (distance <= PQ->head->distance) {
        new->next = PQ->head;
        PQ->head = new;
    } else {
        while (curr->next != NULL && distance > curr->next->distance) {
            curr = curr->next;
        }
    }
    new->next = curr;
    PQ->head = new;
}

// remove LocationID from front of Queue
LocationID PQueueLeave(PQueue PQ)
{
    assert(PQ != NULL);
    assert(PQ->head != NULL);
    LocationID it = PQ->head->value;
    PQueueNode *old = PQ->head;
    PQ->head = old->next;
    if (PQ->head == NULL)
        PQ->tail = NULL;
    //free(old);
    return it;
}

// check for no LocationIDs
int PQueueIsEmpty(PQueue PQ)
{
    return (PQ->head == NULL);
}

// find the minimum number of turns required for a player to get to dest from src
int findPathLength(LocationID src, LocationID dest, PlayerID player, Round round, LocationID *path) {
    assert(validPlace(src));
    assert(validPlace(dest));

    Map map = newMap();
    path[0] = src;
    if (src == dest) return 0;

    int numLoc;
    int *visited = calloc(map->nV,sizeof(int));
    int *tmp = calloc(map->nV,sizeof(int));
    int *preVisited = calloc(map->nV,sizeof(int));
    LocationID *currVisited;
    LocationID *tmpPath = calloc(map->nV, sizeof(LocationID));
    int isFound = 0;
    int length = 0;
    LocationID x, i;

    preVisited[src] = 1;
    visited[src] = 1;
    while (isFound != 1) {
        for (x = 0; x < map->nV && isFound != 1; x++) {
            if (preVisited[x]) {
                currVisited = connectedLocations(&numLoc, x, player,
                                                round, TRUE, TRUE, TRUE);
//int a;
//for (a=0;a<numLoc;a++) {
  //  printf("adjLoc[%d] = %s\n",a,idToName(currVisited[a]));
//}
                preVisited[x] = 0;
                tmp[x] = 0;
                for (i = 0; i < numLoc; i++) {
                    if (currVisited[i] == dest) isFound = 1;
                    if (!visited[currVisited[i]]) {
                        tmpPath[currVisited[i]] = x;
                        tmp[currVisited[i]] = 1;
                        visited[currVisited[i]] = 1;
                    }
                }
                free(currVisited);
            }
        }
        for (x = 0; x < map->nV && isFound != 1; x++) {
            if (tmp[x]) preVisited[x] = 1;
        }
        round++;
        length++;
    }
    i = length;
//int a;
//for (a=0;a<70;a++) {
  //  printf("tmppath[%d] = %s\n",a,idToName(tmpPath[a]));
//}
    for (x = dest; x != src; x = tmpPath[x]) {
        path[i] = x;
        i --;
//printf("%s >", idToName(x));
        //*nextLoc = x;
    }
//printf("%s ", idToName(x));
    //*nextLoc = path[1];
    //printf("nextLoc = %s\n", idToName(*nextLoc));
//printf("findpathlength = %d\n", length);
    free(tmp);
    free(preVisited);
    free(visited);
    free(tmpPath);
    disposeMap(map);

    return length;
}


int hunterPathLength(Map map, Map railMap, LocationID src, LocationID dest, PlayerID player, Round round, LocationID *path) {
    assert(validPlace(src));
    assert(validPlace(dest));

    if (src == dest) return 0;

    int *visited = calloc(map->nV, sizeof(int));
    int *previsited = calloc(map->nV, sizeof(int));
    int *newlyvisited = calloc(map->nV, sizeof(int));
    int isFound = 0;
    int length = 0;
LocationID *tmpPath = calloc(map->nV, sizeof(LocationID));
LocationID x;
//        printf("railstop = %d\n", railStop);
//    printf("src is %s, dest is %s, player is %d, ronud is %d\n", idToName(src), idToName(dest), player, round);
        previsited[src] = 1;
        newlyvisited[src] = 1;
        visited[src] = 1;

            while (isFound != 1) {
                for (x = 0; x < map->nV; x++) {
                    if (newlyvisited[x]) {
          //              printf("putting in aaa is %s\n", idToName(x));
                        isFound = isReachable(map, railMap, x, dest, player, round, previsited, tmpPath);
                        if (isFound) break;
                        newlyvisited[x] = 0;
                     }
                }

                for (x = 0; x < map->nV && isFound != 1; x++) {
                    if (visited[x] == 0 && previsited[x] == 1) {
                        visited[x] = 1;
                        newlyvisited[x] = 1;
//tmpPath[
        //                printf("newlyvisited is %s\n", idToName(x));
                    }
                }
                round++;
                length++;
            }
int i = length;
for (x = dest; x != src; x = tmpPath[x]) {
        path[i] = x;
        i --;
//printf("%s >", idToName(x));
        //*nextLoc = x;
    }
//printf("%s ", idToName(x));
    free(previsited);
    free(newlyvisited);
    free(visited);
    //printf("hpathlength = %d\n", length);
    return length;
}



// check if dest is reacheable in next move
int isReachable(Map map, Map railMap, LocationID src, LocationID dest, PlayerID player, Round round, LocationID *previsited,  LocationID *tmpPath) {
    assert(validPlace(src));
    assert(validPlace(dest));

    int isFound = 0;
    int railStop = (round + player) % 4;
    LocationID w, y, z;
    VList curr, p, n;
if (player == PLAYER_DRACULA) railStop = 0;
//printf("railstop = %d, src = %s, dest = %s\n", railStop, idToName(src), idToName(dest));
    for (curr = map->connections[src]; curr != NULL && (isFound != 1); curr = curr->next) {
        if ((player == PLAYER_DRACULA && curr->type == RAIL) ||
            (player == PLAYER_DRACULA && curr->v == ST_JOSEPH_AND_ST_MARYS)) continue;
        else {
            y = curr->v;//printf("0/1railstop = %s\n", idToName(y));printf("railstop = %d\n", railStop);
//printf("currtype = %d\n", curr->type);
            if (railStop > 1 && (curr->type == RAIL || curr->type == ANY)) {
//printf("hasrail = %s\n", idToName(y));
                for (p = railMap->connections[y]; p != NULL && isFound != 1; p = p->next) {
                        z = p->v;//printf("2railstop = %s\n", idToName(z));
                        if ((p->type == RAIL || p->type == ANY) && z != src) {
                            if (railStop == 3) {
                                for (n = railMap->connections[z]; n != NULL && isFound != 1; n = n->next) {
                                    w = n->v; //printf("3railstop = %s\n", idToName(w));
                                    if ((n->type == RAIL || n->type == ANY) && w != y) {
                                        if (previsited[w]) continue;
tmpPath[w] = src;
                                        if (w == dest) {
                                            isFound = 1;
                                            break;
                                        }
                                        if (!previsited[w]) {
                                            previsited[w] = 1;
                                        }
                                    }
                                }
                            }
                            if (previsited[z]) continue;
tmpPath[z] = src;
                            if (z == dest) {
                                isFound = 1;
                                break;
                            }
                            if (!previsited[z]) {
                                previsited[z] = 1;
                            }
                        }
                    }
                }
                if (previsited[y]) continue;
tmpPath[y] = src;
                if (y == dest) {
                    isFound = 1;
                    break;
                }
                if (!previsited[y]) {
                    previsited[y] = 1;
                }
            }
        }
    if (isFound) {
        return 1;
    } else {
        return 0;
    }
}


// check if there's a double-back or hide in Dracula's trail
int hasDBOrHI(LocationID trail[TRAIL_SIZE]) {
    int hide = 0;
    int douB = 0;
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (trail[i] == HIDE) {
            hide++;
        } else if (trail[i] == DOUBLE_BACK_1 ||
                   trail[i] == DOUBLE_BACK_2 ||
                   trail[i] == DOUBLE_BACK_3 ||
                   trail[i] == DOUBLE_BACK_4 ||
                   trail[i] == DOUBLE_BACK_5) {
            douB++;
        }
    }
    if ((hide != 0) && (douB != 0)) {
        return BOTH_HIDE_AND_DB;
    } else if (hide != 0) {
        return HAS_HIDE;
    } else if (douB != 0) {
        return HAS_DOUBLE_BACK;
    } else {
        return NO_SPECIAL_MOVE;
    }
}


// check if there's a double-back or hide in Dracula's trail
int posOfDb(LocationID trail[TRAIL_SIZE]) {
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (trail[i] == DOUBLE_BACK_1 ||
            trail[i] == DOUBLE_BACK_2 ||
            trail[i] == DOUBLE_BACK_3 ||
            trail[i] == DOUBLE_BACK_4 ||
            trail[i] == DOUBLE_BACK_5) {
            return i;
        }
    }
    return -1;
}

// copy an array of int from old to new
void copyArray(int *old, int *new, int size) {
  int i;
  for (i = 0; i < size; i++) {
    new[i] = old[i];
  }
  return;
}

// make the trail as a linked list
VList trailAsList(LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE]) {
    VList L = NULL;
    VList tmp = L;
    int type, i;
    
    for (i = TRAIL_SIZE - 1; i >= 0; i--) {
        if (hideTrail[i] == HIDE) type = 1;
        else if (hideTrail[i] > HIDE) type = 2;
        else type = 0;
        tmp = listInsert(L, trail[i], type);
        L = tmp;
    }
    return L;
}

// apppend one location to the head of a list
VList listInsert(VList L, int v, int type) {
    VList n = malloc(sizeof(struct vNode));
    assert(n != NULL);
    n->v = v;
    n->type = type;
    n->next = NULL;
    VList newL = NULL;
    if (L == NULL)
        newL = n;
    else {
        n->next = L;
        newL = n;
    }
    return newL;
}

// free the list
void freeList(VList L) {
    if (L != NULL) {
        VList p = L->next;
        while (L != NULL) {
            free(L);
            L = p;
        }
    }
    return;
}

// check if there's a double-back or hide in Dracula's trail linked list
int hasDBOrHIList(VList trailList) {
    int hide = 0;
    int douB = 0;
    int i;
    VList cur = trailList;
    for (i = 0; i < TRAIL_SIZE; i++) {
        if (cur->type == 1) {
            hide++;
        } else if (cur->type == 2) {
            douB++;
        }
        cur = cur->next;
    }
    if ((hide != 0) && (douB != 0)) {
        return BOTH_HIDE_AND_DB;
    } else if (hide != 0) {
        return HAS_HIDE;
    } else if (douB != 0) {
        return HAS_DOUBLE_BACK;
    } else {
        return NO_SPECIAL_MOVE;
    }
}

// to learn the moving pattern of dracula
void learnDracMove(LocationID trail[TRAIL_SIZE]) {
   int i;
   for (i = 0; i < TRAIL_SIZE; i++) {
      if (trail[i] >= MIN_MAP_LOCATION && trail[i] <= MAX_MAP_LOCATION) {
          //check 1. furthest from furthest
          // 2.furthest from nearest
          // 3. SEA type
          // 4. rail and road connections
          // 5. dealing with health
          // 6. predicting hunters move
      }
   }
   // return different mode value, then use that mode for predicting drac moves
   return;
}



//1. how to escape after meeting a hunter
//2. make the lowest mark (if drac is gonna game over)
 //(also another tecniques: if surrounded by hunters,
   //go to sea and stay still drac has 2 bloods left or found a safe place to land,
    //this can earn a greater round numbers, ie, a lower mark)
//3. if drac has sufficient health and hunter is gonna game over,
  //then drac stay around in sea to avoid meeting with the hunters
//4. conditions for when should drac stay in sea
//5. optimalscore function
//6. when should drac go back to cities from sea
//7. select adjLocs appropriately if drac just met a hunter
//8. limit the use of HI and DB if hunters r far away
//9. things to do when they know one of ur locations
