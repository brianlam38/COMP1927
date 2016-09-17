// Pairsing Function
    // Read [The Data]
// Play --> Move
// Encounters
    // Health
    // GameScore
    // Possibly others etc.
// Update Trails
// Maps
    // Write an interface function inside the gameView.c file
  // Every we need neighbours, call connected neighbours
    // Think about special cases, such as rail connections etc.

// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h" //... if you decide to use the Map ADT
#include "Places.h" // Using Places ADT
     
typedef struct _playerInfo {
  int playerHealth;
  LocationID playerLocation;    
  LocationID playerTrail[5];
} playerInfo;

struct gameView {
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    char *pastPlays;            // Hunter = 1:Name 2:Loc 4:Encounters     Drac = 1:D 2:Loc 2:Encounters 1:Action
    int gameScore;
    Round roundNumber;
    PlayerID currentPlayer;
    PlayerMessage messages[];
    Map map;
    playerInfo *players[NUM_PLAYERS];

};

// #####################
// FUNCTION DECLARATIONS
// #####################
#define NUM_EVENT_ENCOUNTER 3

static int countChar(char* string);
static int hunterTurnHealth(char *pastPlays, LocationID prevLocation);
static void updatePlayerTrail(GameView g, int player, LocationID newLocation);
//^updates player trail of given player, by inserting newLocation at the starts
static LocationID otherToID(char *abbrev);      // Parse "other" location chars to location ID
static char IDToPlayer(PlayerID player);        // Parse player ID to player char
static PlayerID playerToID(char *name);         // Parse player char to player ID
static int inArray(int *array,int object, int size);
static int getStations(Map map,LocationID from, LocationID *stations, PlayerID player, Round round);
     

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[]) //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
{
    //new ver
    GameView currentView = malloc(sizeof(struct gameView));
    strcpy(currentView->pastPlays, pastPlays);  
    currentView->gameScore = getScore(currentView);
    currentView->roundNumber = getRound(currentView);
    currentView->currentPlayer = getCurrentPlayer(currentView);
    
    
    int i;
    for (i = 0; i < NUM_PLAYERS; i++) {
        getHistory(currentView, i, currentView->player[i]->trail);
      currentView->player[i]->location = getLocation(currentView, i);
    }
  
  
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    GameView gameView = malloc(sizeof(struct gameView));
    gameView->gameScore = GAME_START_SCORE;
    gameView->roundNumber = 0;
    gameView->pastPlays = pastPlays;
    gameView->messages = messages;
    gameView->map = newMap();
    gameView->currentPlayer = PLAYER_LORD_GODALMING;
    int x;
    for (x = PLAYER_LORD_GODALMING; x <= PLAYER_MINA_HARKER ; x++) {
        gameView->players[x]->playerHealth = GAME_START_HUNTER_LIFE_POINTS;
        gameView->players[x]->playerLocation = ST_JOSEPH_AND_ST_MARYS;
        int y;
        for (y = 0; y < TRAIL_SIZE ; y++) {
            gameView->players[x]->playerTrail[y] = UNKNOWN_LOCATION;
        }
    }
    gameView->players[PLAYER_DRACULA]->playerHealth = GAME_START_BLOOD_POINTS;
    gameView->players[PLAYER_DRACULA]->playerLocation = CASTLE_DRACULA;
    int y;
    for (y = 0; y < TRAIL_SIZE ; y++) {
        gameView->players[PLAYER_DRACULA]->playerTrail[y] = UNKNOWN_LOCATION;
    }
    return gameView;
}     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{ //COMPLETE THIS IMPLEMENTATION
    
    // Check game data exists
    assert(toBeDeleted != NULL);         
    // Free game struct data
    free(toBeDeleted->pastPlays);
    toBeDeleted->pastPlays = NULL;
    free(toBeDeleted->messages);
    toBeDeleted->messages = NULL;
    disposeMap(toBeDeleted->map);
    toBeDeleted->map = NULL;
    // Free player struct
    int x;
    for (x = PLAYER_LORD_GODALMING; x <= PLAYER_DRACULA; x++) {
        free(toBeDeleted->players[x]);
        toBeDeleted->players[x] = NULL;
        assert(toBeDeleted->players[x] == NULL);
    }
    // Free struct ptr + set to NULL + check
    free(toBeDeleted);
    toBeDeleted = NULL;
    assert(toBeDeleted == NULL);
}

// ################
// GET FUNCTIONS
// ################

// Get the current round
Round getRound(GameView currentView)
{
    // new ver
    assert(currentView != NULL);
    Round round;
    int nChar = countChar(currentView->pastPlays);
        if (nChar < 40) {
            round = 0;
    } else {
        round = ((nChar - 39)/40) + 1;
    }
    return round;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
        // new ver
    assert(currentView != NULL);
    PlayerID currentPlayer;
    int nChar = countChar(currentView->pastPlays);
    currentPlayer = ((nChar + 1)/8) % 5
    return currentPlayer;
}

// Get the current score
int getScore(GameView currentView) {
    // new ver
    assert(currentView != NULL);
    int score = GAME_START_SCORE;                  //the current score to be returned
    Round round = getRound(currentView);           //current round number
    int dracTurnCount = 0;                         //number of times Dracula finishes a turn 
    int matureCount = 0;                           //number of times a Vampire matures
    int teleportCount = 0;                         //number of times a hunter is teleported to the hospital
    PlayerID player;
    int health = GAME_START_HUNTER_LIFE_POINTS;    //hunters' health in each turn
    LocationID prevLocation = NUM_MAP_LOCATIONS;   //hunters' previous location in each turn
    int loopCounter;                               //counter for loops
    //count the number of times a hunter is teleported to the hospital
    for (player = 0; player < NUM_PLAYERS - 1; player++) {
        char *entPointer = (currentView->pastPlays + (player * 8)); 
        char currLocation[] = {*(entPointer + 1), *(entPointer + 2)};
        for (loopCounter = 0; loopCounter <= round + 1; loopCounter ++) {
            health = hunterTurnHealth(entPointer, health, prevLocation);
            prevLocation = abbrevToID(currLocation);
            if (health <= 0) {
                health = GAME_START_HUNTER_LIFE_POINTS;
                teleportCount++;
            }
            entPointer += 40;
        }
    }
    //count the number of times Dracula finishes a turn
    if ((countChar(currentView->pastPlays) + 1) % 40 == 0) {
        dracTurnCount = round + 1;
    } else {
        dracTurnCount = round;
    }
    //count the number of times a Vampire matures
    entPointer = (currentView->pastPlays + (PLAYER_DRACULA * 8));
    for (loopCounter = 0; loopCounter <= round + 1; loopCounter ++) {
        if (entPointer[5] == 'V') {
            matureCount++;
        }
        entPointer += 40;
    }
    //calculate the final score
    score -= (teleportCount * 6 + dracTurnCount + matureCount * 13);
    assert(score <= GAME_START_SCORE);
    return score;
  
  
  
    for (p = currentView->pastPlays; *p != '\0'; p +=6) { ////u cant put '\0' as u r stepping 6 , not 1
      
        if (p[0] == 'D') {
            score--;
        }
      
        if (p[0] == 'D' && p[4] == 'V') {//// mature is in the 6th place, not 5
            score -= 13;
        }
      
        ////should be p[2] 'M', but hunter acn go to the hospital without being teleport, so it cant be the case 
        if (p[0] != 'D' && p[1] == 'J' && p[1] == 'M') { // need to keep track of hunters' lifepoints to know whether they teleoported or moved to hospital
            score -= 6;
        }
    }
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    // new ver
    assert(currentView != NULL);
    assert(player >= 0 && player < NUM_PLAYERS);
    char *p = (currentView->pastPlays + (player * 8));
    Round round = getRound(currentView);
    LocationID prevLocation = NUM_MAP_LOCATIONS;
    int loopCounter;    //counter for each time the same player has acted
    int entCounter;     //counter for the event encountered by each player each turn
    if (player == PLAYER_DRACULAR) {
        int health = GAME_START_BLOOD_POINTS;
        return health if (round == 0);
        char dracLocation[] = {*(p + 1), *(p + 2)};
        for (loopCounter = 0; loopCounter <= round + 1; loopCounter ++) {
            if (otherToID(dracLocation) == SEA_UNKNOWN) {
                health -= LIFE_LOSS_SEA;
            } else if (otherToID(dracLocation) == TELEPORT) {
                health += LIFE_GAIN_CASTLE_DRACULA;
            }
            entPointer += 40;
        }
        char *p = (currentView->pastPlays + 3);
        for (loopCounter = 0; loopCounter < ((round + 1) * 5); loopCounter++) {
            for (entCounter = 0; entCounter < 3; entCounter++) {
                if (*entPointer == 'D') {
                    health -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
                entPointer++;
            }
            p += 8;
        }
    } else {
        int health = GAME_START_HUNTER_LIFE_POINTS;
        return health if (round == 0);   
        char currLocation[] = {*(p + 1), *(p + 2)};
        for (loopCounter = 0; loopCounter <= round + 1; loopCounter ++) {
            health = hunterTurnHealth(p, health, prevLocation);
            prevLocation = abbrevToID(currLocation);
            if (health <= 0) {
                health = GAME_START_HUNTER_LIFE_POINTS;
            }
            p += 40;
        }
        assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    }
    return health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player) {
  for (player = 0; player < NUM_PLAYERS - 1; player++) {
        char *entPointer = (currentView->pastPlays + (player * 8)); 
        char currLocation[] = {*(entPointer + 1), *(entPointer + 2)};
        for (loopCounter = 0; loopCounter <= round + 1; loopCounter ++) {
            health = hunterTurnHealth(entPointer, prevLocation);
            prevLocation = abbrevToID(currLocation);
            if (health <= 0) {  
                health = GAME_START_HUNTER_LIFE_POINTS;
                teleportCount++;
            }
            entPointer += 40;
        }
    }
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    return currentView->player[player]->playerTrail[0];
}

// #################
// GET HISTORIC DATA ...done
// #################

void getHistory(GameView currentView, PlayerID player,
                 LocationID trail[TRAIL_SIZE]) {
  int i = 0;
  for (i = 0; i < TRAIL_SIZE; i++) 
    currentView->player[player]->playerTrail[i] = UNKNOWN_LOCATION;
  
  int firstMove = player*8;
  
  for (i = firstMove; i <= getRound(currentView)*40; i += 40) { ////////u will get 0 for round 0 and i is <= 0
    char id[3] = {currentView->pastPlays[i+1],currentView->pastPlays[i+2],'\0'};
    updatePlayerTrail(currentView,player, abbrevToID(id)); 
  }
  
  for (i = 0; i < TRAIL_SIZE; i++) {
    trail[i] = currentView->player[player]->playerTrail[i];
  }
}

// ###################
// MAP QUERY FUNCTIONS ...done
// ###################

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION

    int numNearby = *numLocations; ////////it is the value we have to determine, not a given value, cant use it directly
    LocationID *connections = malloc(numNearby*sizeof(LocationID));
    int counter = 0;

    if (road) {
        LocationID nearby[NUM_LOCATIONS];
        int numNeighbours = 0;
        numNeighbours = NearbyCities(currentView->map, from, nearby, ROAD);
        int i = 0;

    while (counter < numNeighbours) {
            connections[counter] = nearby[i];
            i++; counter++;
        }
  }
    
    if (sea) {
        LocationID nearby[NUM_LOCATIONS];
        int numNeighbours = 0;
        numNeighbours = NearbyCities(currentView->map, from, nearby, ROAD);

    int i = 0;
        while (numNeighbours > 0) {
            if(!inArray(connections,nearby[i], counter)) {
                connections[counter] = nearby[i];
                counter++;
        }
            numNeighbours--; i++;
        }
    }

    if (rail) {
        LocationID stations[NUM_LOCATIONS];
        int numStations = getStations(currentView->map, from, stations, player, round);
        if (numStations != 0) {
            while(numStations > 0) {
                if (!inArray(connections,stations[numStations],counter)) {
                    connections[counter] = stations[numStations];
                    counter++;
                }
                numStations--;
            }
        }
    }
    return connections;
}


static int getStations(Map map,LocationID from, LocationID *stations, PlayerID player, Round round) {

    int stationsAllowed = (player+round)%4;
    if (player == PLAYER_DRACULA || stationsAllowed == 0) return 0;

    int counter = NearbyCities(map, from, stations, RAIL);
    if (stationsAllowed == 1) return counter;

    int numNearby = counter;

  int i;
    for(i = counter; i > 0; i--) {
        LocationID secondaryStations[NUM_LOCATIONS];
        int j = NearbyCities(map,stations[i],secondaryStations,RAIL);

    while (j > 0) {
            if (!inArray(stations,secondaryStations[j],counter)) {
                stations[counter] = secondaryStations[j];
                counter++;
            }
            j--;
        }
    }

    if (stationsAllowed == 2) return counter;

    int k;
  for(k = counter; k > numNearby; k--) {
  
    LocationID tertiaryStations[NUM_LOCATIONS];
    int j = NearbyCities(map,stations[k],tertiaryStations,RAIL);
    
    while (j > 0) {
        if (!inArray(stations,tertiaryStations[j],counter)) {
        stations[counter] = tertiaryStations[j];
        counter++;
      }
      j--;
        }
 }
  
    return counter;
}

static int inArray(int *array,int object, int size) {

    int i;
    for(i = 0; i < size; i++) {
        if (array[i] == object) return 0;
    }
    return 1;
}

// #################
// PARSING FUNCTIONS
// #################

// Given an "other" location abbreviation, return its ID number
static LocationID otherToID(char *abbrev) {
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

// Given player ID, return player char
static char IDToPlayer(PlayerID player) {
    if (player == PLAYER_LORD_GOLDAMING) {
        return 'G';
    } else if (player == PLAYER_DR_SEWARD) {
        return 'S';
    } else if (player == PLAYER_VAN_HELSING) {
        return 'H';
    } else if (player == PLAYER_MINA_HARKER) {
        return 'M';
    } else if (currentPlayer == PLAYER_DRACULA) {
        return 'D';
    }
}

// Given player char, return their PlayerID
static PlayerID playerToID(char *name) {
    if (strmp(name,'G') == 0) {
        return PLAYER_LORD_GODALMING;
    } else if (strcmp(name,'S') == 0) {
        return PLAYER_DR_SEWARD;
    } else if (strcmp(name,'H') == 0) {
        return PLAYER_VAN_HELSING;
    } else if (strcmp(name,'M') == 0) {
        return PLAYER_MINA_HARKER;
    } else if (strcmp(name,'D') == 0) {
        return DRACULA;
    }
}





//To count the number of chars in a string
static int countChar(char* string) {
    char *p;
    int nChar = 0;
    for (p = string; *p != '\0'; p++) {
        nChar++;
    }
    return nChar;
}
  
  
//get the health of hunter in each turn
static int hunterTurnHealth(char *pastPlays, int health, LocationID prevLocation) {
    assert(pastPlays != NULL);
    assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    assert(prevLocation >= MIN_MAP_LOCATION && prevLocation <= NUM_MAP_LOCATIONS);
    char *p = pastPlays;
    char currLocation[] = {*(p + 1), *(p + 2)};
    if (abbrevToID(currLocation) == prevLocation) {
        health += LIFE_GAIN_REST;
        if (health > GAME_START_HUNTER_LIFE_POINTS) {
            health = GAME_START_HUNTER_LIFE_POINTS;
        }
    }
    p += 3;
    for (loopCounter = 0; loopCounter < NUM_EVENT_ENCOUNTER; loopCounter++) {
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
          
          
static void updatePlayerTrail(GameView g, int player, LocationID newLocation) {
    int counter = 5;
    while (counter > 0) {
        g->player[player]->playerTrail[counter] = g->player[player]->playerTrail[counter-1];
      counter--;
    }
      g->player[player]->playerTrail[0] = newLocation;  
}


