// commonFunctions.c ... common functions implementation
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "commonFunctions.h"
#include "Map.h"
#include "Places.h"
#include "Globals.h"

//count the number of nearby cities of a specified location and store the nearby cities in an array
LocationID *NearbyCities(Map map, LocationID from, LocationID *nearby, int *size, int type) {
    VList curr;
    // traverse the neighbour of the city
    for(curr = map->connections[from]; curr != NULL; curr = curr->next) {
        if (curr->type == type) {
            // renew the size and array when a new nearby city of type "type" is found
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

// given an "other" location abbreviation, return its ID number
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

//get the health of hunter in each turn
int hunterTurnHealth(char *pastPlays, int health, LocationID prevLocation, LocationID currLocation) {
    assert(pastPlays != NULL);
    assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    assert(validPlace(prevLocation) || prevLocation == UNKNOWN_LOCATION);
    assert(validPlace(currLocation) || currLocation == UNKNOWN_LOCATION);
    char *p = pastPlays;
    int i;
    //check if the previous and the current location is the same
    //they gain health from rest if the same
    if (currLocation == prevLocation) {
        health += LIFE_GAIN_REST;
        if (health > GAME_START_HUNTER_LIFE_POINTS) {
            health = GAME_START_HUNTER_LIFE_POINTS;
        }
    }
    p += 3;
    //loop each time a hunter has encountered an event and update the health when they lost life
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
    assert(health <= GAME_START_HUNTER_LIFE_POINTS);    // check validity of hunters' health
    return health;
}

//updates player's trail by inserting newLocation at the starts
void updatePlayerTrail(LocationID trail[TRAIL_SIZE], LocationID newLocation) {
    int i;
    for (i = TRAIL_SIZE - 1; i > 0; i--)
        trail[i] = trail[i - 1];    //shift location in the array
    trail[i] = newLocation;         //add current location to the array
}

// get the actual location ID for dracula if his move is a hide or double back
LocationID dracSpecialLocation(LocationID currID, LocationID trail[TRAIL_SIZE]) {
    switch (currID) {
        case HIDE:
            currID = trail[0];
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

void initialiseTrail(LocationID trail[TRAIL_SIZE]) {
    int i;
    //initialise all the trail locations to UNKNOWN_LOCATION
    for (i = 0; i < TRAIL_SIZE; i++) trail[i] = UNKNOWN_LOCATION;
}

// check and update the number of traps and number of immature vampires in a given location
void numEncounter(LocationID trail[TRAIL_SIZE], char c, LocationID where, int *numTraps, int *numVamps) {
    assert(validPlace(where) && idToType(where) != SEA);
    int i;
    for (i = 0; i < TRAIL_SIZE; i++) {        // check if "where" is in the trail
        if (where == trail[i]) i = TRAIL_SIZE;
    }
    if (i != TRAIL_SIZE + 1) {
        // number of all encounters are reset to 0 if "where" is already out of the trail
        *numTraps = 0;
        *numVamps = 0;
    } else {
        if (c == 'T') (*numTraps)++;    // number of traps increases if a trap has placed
        if (c == 'V') (*numVamps)++;    // number of vampires increses if am immature vampire las placed
    }
    return;
}
