// Globals.h ... global game definitions for Fury of Dracula
// By: Mina Harker
// Date: 22 September 2014
// Version: 1.0
// Do not alter this file yourself.  You do not submit this file.

#ifndef GLOBALS_H
#define GLOBALS_H

#define FALSE 0
#define TRUE 1

// Globals
typedef int Round;

#define TRAIL_SIZE 6

// Players
typedef int PlayerID;

#define PLAYER_LORD_GODALMING   0
#define PLAYER_DR_SEWARD        1
#define PLAYER_VAN_HELSING      2
#define PLAYER_MINA_HARKER      3
#define PLAYER_DRACULA          4

#define NUM_PLAYERS     5

// Score
#define GAME_START_SCORE                366
#define SCORE_LOSS_DRACULA_TURN         1
#define SCORE_LOSS_HUNTER_HOSPITAL      6
#define SCORE_LOSS_VAMPIRE_MATURES      13

// Dracula Health
#define GAME_START_BLOOD_POINTS         40
#define LIFE_LOSS_HUNTER_ENCOUNTER      10
#define LIFE_LOSS_SEA                   2
#define LIFE_GAIN_CASTLE_DRACULA        10

// Hunter Health
#define GAME_START_HUNTER_LIFE_POINTS   9
#define LIFE_LOSS_TRAP_ENCOUNTER        2
#define LIFE_LOSS_DRACULA_ENCOUNTER     4
#define LIFE_GAIN_REST                  3 

#endif
