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
int countChar(char* string);
void updatePlayerTrail(GameView g, int player, LocationID newLocation);
//^updates player trail of given player, by inserting newLocation at the starts
LocationID otherToID(char *abbrev);
int inArray(int *array,int object, int size);
int getStations(Map map,LocationID from, LocationID *stations, PlayerID player, Round round);
     

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
    free(toBeDeleted->messages);
    disposeMap(toBeDeleted->map);
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
  
  
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    int round = currentView->roundNumber;
    assert(round >= 0);

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
int getScore(GameView currentView)
{
The game score starts at 366 points.
The score decreases by 1 each time Dracula finishes a turn.
The score decreases by 6 each time a Hunter is teleported to the hospital.
The score decreases by 13 each time a Vampire matures (ie falls off the trail).
Dracula wants the score to be low, the Hunters want the score to be high.
    // new ver
    assert(currentView != NULL);
    int score = GAME_START_SCORE;
    Round round = getRound(currentView);
    char *p;
    //char c;
    
    for (p = currentView->pastPlays; *p != '\0'; p +=6) {
      
        if (p[0] == 'D') {
            score--;
        }
      
        if (p[0] == 'D' && p[4] == 'V') {
            score -= 13;
        }
      
        
        if (p[0] != 'D' && p[1] == 'J' && p[1] == 'M') { // need to keep track of hunters' lifepoints to know whether they teleoported or moved to hospital
            score -= 6;
        }
    }

  
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    return score;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
  4 letters representing, in order, the Encounters that occurred:
one 'T' for each Trap encountered (and disarmed)
'V' if an immature Vampire was encountered (and vanquished)
'D' if, finally, Dracula was confronted
then '.' for the remaining 0..4 characters
  
  2 uppercase characters representing the new location of Dracula
'C?' = City move (unknown city)
'S?' = Sea move (unknown sea)
'HI' = Hide move, if we have discovered it was a hide move
'Dn' = Double Back move: D followed by a single character number (n) from 1-5 specifying the position in his trail to which Dracula is doubling back (eg 1 means staying in his most recent location, 5 means returning to his location 5 moves ago)
'TP' = Teleport to Castle Dracula
any valid location code (see below), if we know Dracula's location (e.g. via research or confrontation)
2 characters representing the encounter Dracula placed:
'T' if a Trap was placed, otherwise '.'
'V' if an immature Vampire was placed, otherwise '.'
1 character representing the action phase of Dracula's turn:
'M' if a Trap has left the trail (malfunctions due to old age), and vanishes without a trace. (yay!)
'V' if a Vampire has matured. (eek!)
'.' if nothing has occurred (e.g. early in game)
a single '.' character
    // new ver
    assert(currentView != NULL);
    assert(player >= 0 && player < NUM_PLAYERS);
    char *entPointer = (currentView->pastPlays + (player * 8) + 3);    //pointer to the first encountered event of the player
    Round round = getRound(currentView);
    int loopCounter;    //counter for each time the same player has acted
    int entCounter;     //counter for the event encountered by each player each turn
    if (player == PLAYER_DRACULAR) {
        int health = GAME_START_BLOOD_POINTS;
        return health if (round == 0);
        char dracLocation[] = {*(entPointer - 2), *(entPointer - 1)};
        for (loopCounter = 0; loopCounter <= round + 1; loopCounter ++) {
            if (strcmp(dracLocation, "S?") == 0) {
                health -= LIFE_LOSS_SEA;
            } else if (strcmp(dracLocation, "TP") == 0) {
                health += LIFE_GAIN_CASTLE_DRACULA;
            }
            entPointer += 40;
        }
        for (char *p = (currentView->pastPlays + 3); 
            for (entCounter = 0; entCounter < 3; entCounter++) {
                if (*entPointer == 'D') {
                    health -= LIFE_LOSS_HUNTER_ENCOUNTER;
                }
                entPointer++;
            }
            p += 8;
    } else {
        int health = GAME_START_HUNTER_LIFE_POINTS;
        return health if (round == 0);  
        char prevLocation[] = {*(entPointer - 2), *(entPointer - 1)};
        for (loopCounter = 0; loopCounter <= round + 1; loopCounter ++) {
            char *p = entPointer;    //pointer to each event that the player encountered
            for (entCounter = 0; entCounter < 3; entCounter++) {
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
            }
            if (loopCounter > 0) {
                char currLocation[] = {*(entPointer - 2), *(entPointer - 1)};
                if (strcmp(currLocation, prevLocation) == 0) {
                    health += LIFE_GAIN_REST;
                    if (health > GAME_START_HUNTER_LIFE_POINTS) {
                            health = GAME_START_HUNTER_LIFE_POINTS;
                    }
                }
                strcpy(prevLocation, currLocation);
            }
            entPointer += 40;
        }
        assert(health <= GAME_START_HUNTER_LIFE_POINTS);
    }
    return health;
  
  
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    int health = currentView->players[currentView->currentPlayer]->playerHealth;
    if (currentView->currentPlayer == PLAYER_DRACULA) {
//        assert(health <= 40 && health > 0); Dracula's health can be above 40
    
    } else {
        assert(health <= 9);
    }
    return health;
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player) {
  
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION
    assert(currentView != NULL);
    return currentView->player[player]->trail[0];
}

// #################
// GET HISTORIC DATA ...done
// #################

void getHistory(GameView currentView, PlayerID player,
                 LocationID trail[TRAIL_SIZE]) {
  int i = 0;
  for (i = 0; i < TRAIL_SIZE; i++) 
    currentView->player[player]->trail[i] = UNKNOWN_LOCATION;
  
  int firstMove = player*8;
  
  for (i = firstMove; i <= getRound(currentView)*40; i += 40) {
    char id[3] = {currentView->pastPlays[i+1],currentView->pastPlays[i+2],'\0'};
    updatePlayerTrail(currentView,player, abbrevToID(id)); 
  }
  
  for (i = 0; i < TRAIL_SIZE; i++) {
    trail[i] = currentView->player[player]->trail[i];
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

    int numNearby = *numLocations;
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


int getStations(Map map,LocationID from, LocationID *stations, PlayerID player, Round round) {

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

int inArray(int *array,int object, int size) {

    int i;
    for(i = 0; i < size; i++) {
        if (array[i] == object) return 0;
    }
    return 1;
}

// #################
// PARSING FUNCTIONS
// #################

    // PARSING CHECKLIST
    // char *pastPlays
// 1. Take in location ABBREV from trail -> Give AI int value   DONE (given by default places.c?)
// 2. Take in special ABBREV (actions etc) -> Give AI int value DONE (gameView.c)
// 3. Take in AI move -> return location ABBREV                 NOT STARTED
// 4. Take in AI action -> return special ABBREV                NOT STARTED
    // PlayerMessage messages[]
// 1. Take in AI message -> Store in messages                   NOT STARTED
// 2. Take messages from storage -> Translate for AI            NOT STARTED

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
             
PlayerID nameAbbrevToID(char *name) {
    if (strmp(name,"G") == 0) {
        return PLAYER_LORD_GODALMING;
    } else if (strcmp(name,"S") == 0) {
        return PLAYER_DR_SEWARD;
    } else if (strcmp(name,"H") == 0) {
        return PLAYER_VAN_HELSING;
    } else if (strcmp(name,"M") == 0) {
        return PLAYER_MINA_HARKER;
    } else if (strcmp(name,"D") == 0) {
        return DRACULA;
}

// Parses pastPlays string and sets current data
void setCurrPlayer(char *pastPlays){
    int turn;
    for (turn = 0; turn < strlen(pastPlays); turn++;) {
        if (
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
          
          
void updatePlayerTrail(GameView g, int player, LocationID newLocation) {
        int counter = 5;
    while (counter > 0) {
        g->player[player]->trail[counter] = g->player[player]->trail[counter];
      counter--;
    }
      g->player[player]->trail[0] = newLocation;  
}


 
/*
    int turn = 0;
  int currPlay = 0;
  
    for (turn = 0; pastPlays != empty; turn++) {
        case('V'):
    VladMove(); break;
    default:
        HunterMove(); break;
  
  }

*/



