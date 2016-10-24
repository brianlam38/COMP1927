//////////////////////// commonFunctions.h ///////////////////////////////////

// commonFunctions.h
// Interface for commonFuntions.c

#ifndef commonFunctions_h
#define commonFunctions_h

#include <stdio.h>
#include "Map.h"
#include "Globals.h"
#include "GameView.h"


// struct definition of Map
typedef struct vNode *VList;

struct vNode {
    LocationID  v;    // ALICANTE, etc
    TransportID type; // ROAD, RAIL, BOAT
    VList       next; // link to next node
};

struct MapRep {
    int   nV;         // #vertices
    int   nE;         // #edges
    VList connections[NUM_MAP_LOCATIONS]; // array of lists
};


// struct definition of gameView
typedef struct _playerInfo {
    int playerHealth;                      // health of each player
    LocationID playerCurrLocation;         // current loaction of each player
    LocationID playerTrail[TRAIL_SIZE];    // trail location of each player
} playerInfo;

struct gameView {
    char *pastPlays;                       // string of all plays in the past
    int gameScore;                         // current game score
    Round roundNumber;                     // current round number
    PlayerID currentPlayer;                // ID of current player
    PlayerMessage *messages;               // messages provided by players
    playerInfo *players[NUM_PLAYERS];      // array of player info

};


typedef struct QueueRep *Queue;

typedef struct QueueNode {
	LocationID value;
	struct QueueNode *next;
} QueueNode;

typedef struct QueueRep {
	QueueNode *head;  // ptr to first node
	QueueNode *tail;  // ptr to last node
} QueueRep;

typedef struct PQueueRep *PQueue;

typedef struct PQueueNode {
    LocationID value;
    int distance;
    struct PQueueNode *next;
} PQueueNode;

typedef struct PQueueRep {
    PQueueNode *head;  // ptr to first node
    PQueueNode *tail;  // ptr to last node
} PQueueRep;

// number of hunters
#define NUM_HUNTERS         4
//max number of event encountered each play
#define NUM_EVENT_ENCOUNTER 4
//number of chars per round in pastPlays
#define CHARS_PER_ROUND     40
//number of chars per turn in pastPlays
#define CHARS_PER_TURN      8
//indicate it's a dracView
#define DRAC_VIEW           -1
//indicate it's a hunterView
#define HUNTER_VIEW         -2

//types of special moves in Dracula's trail
#define NO_SPECIAL_MOVE     -1
#define HAS_HIDE            -2
#define HAS_DOUBLE_BACK     -3
#define BOTH_HIDE_AND_DB    -4

#define NUM_SEA             10
#define NUM_ISLAND          6
#define NUM_PORT_CITY       29
#define NUM_TOP_AREA        11
#define NUM_LEFT_AREA       13
#define NUM_MIDDLE_AREA     27
#define NUM_RIGHT_AREA      19
#define NUM_BOTTOM_AREA     15

#define CURR_EMPTY          0
#define RIGHT_EMPTY         1
#define LEFT_EMPTY          2
#define BOTTOM_EMPTY        3
#define TOP_EMPTY           4
#define MIDDLE_EMPTY        5



// traverse the map and return an array of nearby cities of "from" with type "type"
// *nearby and *size is the original array and size of the array
// the function updates the *nearby array and size of the array
LocationID *NearbyCities(Map map, LocationID from,
                         LocationID *nearby, int *size, int type);


// check if an object is in the array
// return the index of first occurance of the object
// return -1 if the object is not in the array
int inArray(int *array, int object, int size);


// copy an array of int from old to new
void copyArray(int *old, int *new, int size);


// shift the array to the left
// start: the starting index of array to shift
// end  : the ending index of array to shift
void shiftLeft(LocationID *array, int start, int end);


// shift the array to the right
void shiftRight(LocationID *array, int start, int end);


// given a Place abbreviation, return its ID number
// (with other "locations" included)
LocationID otherToID(char *abbrev);


// count the number of chars in a given string
int countChar(char* string);


// get the abbreviation fr
void strToAbbrev(char *play, char abbrev[]);


// return the health of a hunter in a particula play
// pastPlays    : the string of that particula play
// health       : health from last round
// prevLocation : previous location of the hunter
// currLocation : current location of the hunter
int hunterTurnHealth(char *pastPlays, int health,
                     LocationID prevLocation, LocationID currLocation);


// initialise a given trail to UNKNOWN_LOCATION
void initialiseTrail(LocationID trail[TRAIL_SIZE]);


// Update the trail with newLocation inserted
// trail : trail of a player in last 6 truns
// newLocation : the newest location to be the part of trail
void updatePlayerTrail(LocationID trail[TRAIL_SIZE],
                       char *newLocation, PlayerID player);


// return the actual location ID of Dracula if
//  his move is a hide or double back
// currID : ID of current location
// trail  : tral of Dracula
LocationID dracSpecialLocation(LocationID currID, LocationID trail[TRAIL_SIZE]);


// check and update the number of traps and
//   number of immature vampires in a given location
// trail : trail of a player
// c     : the char of an event encountered in a play
// where : location that you are interested in knowing
//         the number of traps and immature vampires
void numEncounter(LocationID trail[TRAIL_SIZE], char c,
                  LocationID where, int *numTraps, int *numVamps);


int hunterPathLength(Map map, Map railMap, LocationID src, LocationID dest, PlayerID player, Round round, LocationID *path);
int isReachable(Map map, Map railMap, LocationID src, LocationID dest, PlayerID player, Round round, LocationID *previsited,  LocationID *tmpPath);
int simpleFindPathLength(LocationID src, LocationID dest);
int findPathLength(LocationID src, LocationID dest, PlayerID player, Round round, LocationID *path);
LocationID howToGetTo(LocationID dest, LocationID from, int round,
                      int player, int *pathLength, int sea, int train);
int findMostCommon(int *array,int size);

Queue newQueue(); // create new empty queue
void dropQueue(Queue); // free memory used by queue
void showQueue(Queue); // display as 3 > 5 > 4 > ...
void QueueJoin(Queue,LocationID location); // add item on queue
LocationID QueueLeave(Queue); // remove item from queue
int QueueIsEmpty(Queue); // check for no items

PQueue newPQueue(); // create new empty queue
void dropPQueue(PQueue); // free memory used by queue
void showPQueue(PQueue); // display as 3 > 5 > 4 > ...
void PQueueJoin(PQueue PQ, LocationID it, int distance); // add item on queue
LocationID PQueueLeave(PQueue); // remove item from queue
int PQueueIsEmpty(PQueue); // check for no items

// check if there's a double-back or hide in Dracula's trail
// view : DRAC_VIEW or HUNTER_VIEW is passing in?
// return NO_SPECIAL_MOVE  if there are no special moves in Dracula's trail,
//        HAS_HIDE         if there is a hide in Dracula's trail,
//        HAS_DOUBLE_BACK  if there is a double-back in Dracula's trail,
//        BOTH_HIDE_AND_DB if there are both double-back and hide in Dracula's trail
int hasDBOrHI(LocationID trail[TRAIL_SIZE]);
int posOfDb(LocationID trail[TRAIL_SIZE]);


// make the trail as a linked list
VList trailAsList(LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE]);


// apppend one location to the head of a list
VList listInsert(VList L, int v, int type);


// free the list
void freeList(VList L);

// check if there's a double-back or hide in Dracula's trail linked list
int hasDBOrHIList(VList trailList);


#endif /* commonFunctions_h */
