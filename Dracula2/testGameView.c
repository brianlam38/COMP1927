// testGameView.c ... test the GameView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

#define TEST_PLAYS_1 "GED.... SGE.... HZU.... MCA.... DCF.V.. \
                                            GMN.... SCFVD.. HGE.... MLS.... DBOT... \
                                            GLO.... SMR.... HCF.... MMA.... DTOT... \
                      GPL.... SMS.... HMR.... MGR.... DBAT... \
                      GLO.... SBATD.. HMS.... MMA.... DSRT... \
                      GPL.... SSJ.... HBA.... MGR.... DALT... \
                      GPL.... SSJ.... HBA.... MGR.... DMAT... \
                      GLO.... SBE.... HMS.... MMATD.. DHIT... "

#define TEST_PLAYS_2 "GED.... SGE.... HZU.... MCA.... DCF.V.. \
                                            GMN.... SCFVD.. HGE.... MLS.... DBOT... \
                                            GLO.... SMR.... HCF.... MMA.... DTOT.M. \
                      GPL.... SMS.... HMR.... MGR.... DBAT... \
                      GLO.... SBATD.. HMS.... MMA.... DSRT... \
                      GPL.... SSJ.... HBA.... MGR.... DALT... \
                      GPL.... SSJ.... HBA.... MGR.... DMAT... \
                      GLO.... SBE.... HMS.... MMATD.. DSRT... \
                      GEC.... SSO.... HMR.... MMA.... DTPT... \
                      GLE.... SBC.... HCF.... MSN.... DHIT... \
                      GBU.... SKL.... HPA.... MSRTT.. DD4T... "


#define TEST_PLAYS_3 "GED.... SGE.... HZU.... MCA.... DCF.V.. \
                                            GMN.... SPA.... HGE.... MLS.... DBOT... \
                                            GLO.... SMR.... HCF.... MMA.... DTOT.M. \
                      GPL.... SMS.... HMR.... MGR.... DBAT... \
                      GLO.... SBATD.. HMS.... MMA.... DSRT... \
                      GPL.... SSJ.... HBA.... MGR.... DALT... \
                      GPL.... SSJ.... HBA.... MGR.... DMAT.V. \
                      GLI.... SCD.... HSJ.... MMATD.. DALT...   \
                                            GLI.... SKL.... HSJ.... MBA.... DMS.... \
                      GLI.... SCD.... HSJ.... MMS.... DHI.... \
                      GLI.... SCD.... HSJ.... MMS.... DTPT... \
                      GNU.... SCDTD.. HSJ.... MMA.... DD2.... "
                      
int main()
{
    int i;
    GameView gv;

    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    printf("Initialised new game\n");
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
   printf("Got player 1\n");
    assert(getRound(gv) == 0);
   printf("Round correct\n");
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
   printf("health1 correct\n");
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
   printf("health2 correct\n");
    assert(getScore(gv) == GAME_START_SCORE);
   printf("Score correct\n");
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);


    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);
    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    printf("passed\n");
    disposeGameView(gv);
  
  
/*#######################################################
                     OUR TESTS HERE:
Functions:
GameView newGameView(char *pastPlays, PlayerMessage messages[]);
void disposeGameView(GameView toBeDeleted);

Round getRound(GameView currentView);
PlayerID getCurrentPlayer(GameView currentView);
int getScore(GameView currentView);
int getHealth(GameView currentView, PlayerID player);
LocationID getLocation(GameView currentView, PlayerID player);
void getHistory(GameView currentView, PlayerID player,LocationID trail[TRAIL_SIZE]);

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea);
#######################################################*/
    assert(gv == NULL);
    printf("Starting our own tests\n");
  
    // TEST_PLAY_1
    printf("Starting tests on TEST_PLAY_1\n");
    PlayerMessage messages6[] = {"Hello","this","is","a","placeholder","message"};
    gv = newGameView(TEST_PLAYS_1, messages6);
    
    assert(getRound(gv) == 8);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getScore(gv) == 352);
    printf("Testing Player Locations\n");
  
    assert(getLocation(gv,PLAYER_MINA_HARKER) == MADRID);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == BELGRADE);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == MEDITERRANEAN_SEA);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == LONDON);
    assert(getLocation(gv,PLAYER_DRACULA) == MADRID);

        printf("Test for player health\n");
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 9);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == 9);
    assert(getHealth(gv,PLAYER_VAN_HELSING) == 9);
    assert(getHealth(gv,PLAYER_MINA_HARKER) == 3);
    assert(getHealth(gv,PLAYER_DRACULA) == 20);
    printf("passed\n");

    printf("all passed\n");
    disposeGameView(gv);
/////////////////////////////////////////////////////////////////   
    // TEST_PLAY_2
    printf("Starting tests on TEST_PLAY_2\n");
    PlayerMessage messages7[] = {"Hello","this","is","a","placeholder","message"};
    gv = newGameView(TEST_PLAYS_2, messages7);//
    
    assert(getRound(gv) == 10);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getScore(gv) == 366); /* Score is incorrect, just placeholder */
    printf("Testing Player Locations\n");
  
    assert(getLocation(gv,PLAYER_MINA_HARKER) == SARAGOSSA);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == KLAUSENBURG);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == PARIS);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == BRUSSELS);
    assert(getLocation(gv,PLAYER_DRACULA) == SARAGOSSA);

        printf("Test for player health\n");
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 9);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == 9);
    assert(getHealth(gv,PLAYER_VAN_HELSING) == 9);
    assert(getHealth(gv,PLAYER_MINA_HARKER) == 3);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    printf("passed\n");

    printf("all passed\n");
    disposeGameView(gv);
  
  
  ///////////////////// test set 3 ////////////////////////
    printf("Starting tests on TEST_PLAY_3\n");
    PlayerMessage messages8[] = {"Hello","this","is","a","placeholder","message"};
    gv = newGameView(TEST_PLAYS_3, messages8);//
    
    assert(getRound(gv) == 11);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getScore(gv) == 3366); /* Incorrect score, just placeholder */
    printf("Testing Player Locations\n");
  
    assert(getLocation(gv,PLAYER_MINA_HARKER) == MADRID);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == CASTLE_DRACULA);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == SARAJEVO);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == NUREMBURG);
    assert(getLocation(gv,PLAYER_DRACULA) == MEDITERRANEAN_SEA);

        printf("Test for player health\n");
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 9);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == 9);
    assert(getHealth(gv,PLAYER_VAN_HELSING) == 9);
    assert(getHealth(gv,PLAYER_MINA_HARKER) == 3);
    assert(getHealth(gv,PLAYER_DRACULA) == 20);
    printf("passed\n");

    printf("all passed\n");
    disposeGameView(gv);
   
  
    return EXIT_SUCCESS;
}





