// hunter.c
// Implementation of your "Fury of Dracula" hunter AI
//HunterView functions: 
/*
Round giveMeTheRound(HunterView currentView);
PlayerID whoAmI(HunterView currentView);
int giveMeTheScore(HunterView currentView);
int howHealthyIs(HunterView currentView, PlayerID player);
LocationID whereIs(HunterView currentView, PlayerID player);
void giveMeTheTrail(HunterView currentView, PlayerID player,
                        LocationID trail[TRAIL_SIZE]);

LocationID *whereCanIgo(HunterView currentView, int *numLocations,
                        int road, int rail, int sea);
LocationID *whereCanTheyGo(HunterView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea);

*/

// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include "Places.h"
#include "commonFunctions.h"

#define G_START 60 // Godalming = Stratsbourg  (centre)
#define S_START 37 // Seward = Lisbon          (left)
#define V_START 66 // Van Helsing = Varna      (right)
#define M_START 27 // Mina Harker = Galway     (top)

static int convergeOnDrac(HunterView h);
static int convergeOnLeader(HunterView h);

//static int leader = PLAYER_LORD_GODALMING; // Global leader variable

/*
    Current Strategy:
        (1) Godalming starts in the centre of the map. Remaining players spread out LEFT, RIGHT, TOP.
        (2) Remaining players converge on Godalming, the leader for the first 6 turns.
        (3) If trail is picked up, all hunters converge on Drac
        (4) If 6 rounds pass and no trail is picked up, all hunters research.
        (5) Converge on Drac.
        (6) If dracTrail is lost, all hunters research immediately and converge on Drac.
        (7) Repeat step #6
*/

void decideHunterMove(HunterView gameState)
{  
    int player = whoAmI(gameState);         // store curr player
    int round = giveMeTheRound(gameState);  // store curr round

    LocationID dTrail[TRAIL_SIZE];          // get drac trail
    giveMeTheTrail(gameState,PLAYER_DRACULA,dTrail);

    LocationID hTrail[TRAIL_SIZE];          // get current hunter trail
    giveMeTheTrail(gameState,player,hTrail);

    int locID;
    char *move;

    printf("The current round is: %d\n", round);
    // Round 0 placement
    if (round == 0) {
        if (player == PLAYER_LORD_GODALMING) { locID = G_START; }
        else if (player == PLAYER_DR_SEWARD) { locID = S_START; }
        else if (player == PLAYER_VAN_HELSING) { locID = V_START; }
        else { locID = M_START; }
        move = idToAbbrev(locID);
        registerBestPlay(move,"ROUND 0 PLACEMENT");
    // First 5 turns, converge on Godalming
    } else if (round < 6 && dTrail[0] == CITY_UNKNOWN) {
        locID = convergeOnLeader(gameState);
        move = idToAbbrev(locID);
        registerBestPlay(move,"Converge on GODALMING");        
    }
    // Dracula trail is found
    if (dTrail[0] != CITY_UNKNOWN) {
        if (dTrail[0] == hTrail[0]) {            // Stay in city if drac is here
            locID = hTrail[0];
            move = idToAbbrev(locID);
            registerBestPlay(move,"Attack DRAC");    
        } else {                                 // Converge on drac
            locID = convergeOnDrac(gameState);
            move = idToAbbrev(locID);
            registerBestPlay(move,"Converge on DRAC");
        }
    // Dracula trail not found, all hunters research
    } else {
        locID = hTrail[0];
        move = idToAbbrev(locID);
        registerBestPlay(move,"RESEARCHING");
    }                  
}

// Returns LocationID of whereToGoNext to hunt drac
int convergeOnDrac(HunterView h) {

    // Use pathLength() to determine #turns away Dracula is
    // Use BFS to determine which cities he may currently be in
    // Close him off?

    return 0;
}

// Returns LocationID of whereToGoNext for initial 5 turns
int convergeOnLeader(HunterView h) {

    // For the first 5 turns OR until drac trail is found,
    // converge towards Godalming.

    return 0;
}

// BFS to find length of shortest path (# of turns to dest)
int pathLength(LocationID src, LocationID dest) {

    Map map = newMap();

    LocationID *path = malloc(map->nV * sizeof(LocationID));
    LocationID *visited = calloc(map->nV, sizeof(LocationID));
    Queue q = newQueue();
    QueueJoin(q,src);
    int isFound = 0;

    while (!QueueIsEmpty(q) && !isFound) {
        LocationID x = QueueLeave(q);
        visited[x] = 1;
        VList curr;
        LocationID y;
        for (curr = map->connections[x]; curr != NULL; curr = curr->next) {
            y = curr->v;
            path[y] = x;
            if (y == dest)
                isFound = 1;
                break;
            if (!visited[y])
                QueueJoin(q,y);
                visited[y] = 1;
        }
    }
    int length = 0;
    if (isFound) {
        LocationID v;
        for (v = dest; v != src; v = path[v])
            length++;
    }
    return length;
}
