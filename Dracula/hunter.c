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

//static void submitID(LocationID dest);
static int convergeOnDrac(HunterView h);
static int convergeOnLeader(HunterView h);

//static int leader = PLAYER_LORD_GODALMING; // Global leader variable

/*
    Current Strategy:
        (1) Godalming starts in the centre of the map. Remaining players spread out LEFT, RIGHT, TOP.
        (2) Remaining players converge on Godalming, the leader.
        (3) If trail is picked up, all hunters converge on Drac
        (4) If 6 rounds pass without picking up
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
            registerBestPlay(move,"Rest at DRAC");    
        } else {                                 // Converge on drac
            locID = convergeOnDrac(gameState);
            move = idToAbbrev(locID);
            registerBestPlay(move,"Converge on DRAC");
        }
    // Dracula trail not found, all hunters research
    } else {
        locID = hTrail[0];
        move = idToAbbrev(locID);
        registerBestPlay(move,"Resting");
    }                  
}

// Returns LocationID of whereToGoNext
int convergeOnDrac(HunterView h) {
    /*
    LocationID dTrail[TRAIL_SIZE];
    giveMeTheTrail(h,PLAYER_DRACULA,dTrail);

    int loc0 = whereIs(h,PLAYER_LORD_GOLDAMING);
    int loc1 = whereIs(h,PLAYER_DR_SEWARD);
    int loc2 = whereIs(h,PLAYER_VAN_HELSING);
    int loc3 = whereIs(h,PLAYER_MINA_HARKER);
    */
    return 0;
}

// BFS on shortest path to leader
int convergeOnLeader(LocationID src, LocationID dest) {


    return path;
}

// BFS to find length of shortest path
int pathLength(LocationID src, LocationID dest) {
    Map map = newMap();

    LocationID *path = malloc(map->nV * sizeof(LocationID));
    LocationID *visited = calloc(map->nV, sizeof(LocationID));
    Queue q = newQueue();
    QueueJoin(q,src);
    int isFound = 0;

    while (!emptyQ(q) && !isFound) {
        LocationID y;
        LocationID x = QueueLeave(q);
        VList curr;
        for (y = 0; y < nV(g); y++) {
            if (!hasEdge(g,x,y))
                continue;
            path[y] = x;        // Remembering value of prev vertex
            if (y == dest)
                isFound = 1;
                break;
            if (!visited[y])
                QueueJoin(q,y);
                visited[y] = 1;
        }
    }
    if (isFound) {
        LocationID v;
        for (v = dest; v != src; v = path[v])   // Display path from dest -> src (reverse)
            length++
    }

    return length;
}
