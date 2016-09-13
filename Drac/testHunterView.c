// testHunterView.c ... test the HunterView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "HunterView.h"

int main()
{
    int i;
    HunterView hv;
    
    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    hv = newHunterView("", messages1);
    assert(whoAmI(hv) == PLAYER_LORD_GODALMING);
    assert(giveMeTheRound(hv) == 0);
    assert(howHealthyIs(hv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(giveMeTheScore(hv) == GAME_START_SCORE);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeHunterView(hv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    hv = newHunterView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(whoAmI(hv) == PLAYER_LORD_GODALMING);
    assert(giveMeTheRound(hv) == 1);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(whereIs(hv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(whereIs(hv,PLAYER_VAN_HELSING) == ZURICH);
    assert(whereIs(hv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(whereIs(hv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeHunterView(hv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    hv = newHunterView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(whereIs(hv,PLAYER_DRACULA) == GENEVA);
    assert(howHealthyIs(hv,PLAYER_LORD_GODALMING) == 5);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == 30);
    assert(whereIs(hv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    giveMeTheTrail(hv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");        
    disposeHunterView(hv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    hv = newHunterView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                       "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(whereIs(hv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    giveMeTheTrail(hv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(howHealthyIs(hv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(whoAmI(hv) == 0);
    printf("passed\n");
    disposeHunterView(hv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;

    printf("Checking Galatz road connections\n");
    PlayerMessage messages5[] = {"Gone to Galatz"};
    hv = newHunterView("GGA....", messages5);    
    edges = whereCanTheyGo(hv,&size,PLAYER_LORD_GODALMING,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    disposeHunterView(hv);

    printf("Checking Ionian Sea sea connections\n");
    PlayerMessage messages6[] = {"Sailing the Ionian"};
    hv = newHunterView("GIO....", messages6);
    edges = whereCanTheyGo(hv,&size,PLAYER_LORD_GODALMING,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    disposeHunterView(hv);

    printf("Checking Athens rail connections (none)\n");
    PlayerMessage messages7[] = {"Leaving Athens by train"};
    hv = newHunterView("GAT....", messages7);
    edges = whereCanTheyGo(hv,&size,PLAYER_LORD_GODALMING,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    disposeHunterView(hv);

    printf("passed\n");
    return 0;
}

