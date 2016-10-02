// commonFunctions.c ... common functions implementation
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "commonFunctions.h"
#include "Map.h"
#include "Places.h"
#include "Globals.h"

//count the number of nearby cities of a specified location and
// store the nearby cities in an array
LocationID *NearbyCities(Map map, LocationID from,
                         LocationID *nearby, int *size, int type) {
    VList curr;

    // traverse the neighbour of the city and renew the size and
    //  array when a new nearby city of type "type" is found
    for(curr = map->connections[from]; curr != NULL; curr = curr->next) {
        if (curr->type == type) {
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
        if (array[i] == object) return 1;
    }
    return 0;
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
    if (currLocation == prevLocation) {
        health += LIFE_GAIN_REST;
        if (health > GAME_START_HUNTER_LIFE_POINTS) {
            health = GAME_START_HUNTER_LIFE_POINTS;
        }
    }

    p += 3;
    // loop each time a hunter has encountered an event and
    //  update the health when they lost life
    for (i = 0; i < NUM_EVENT_ENCOUNTER; i++) {
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
