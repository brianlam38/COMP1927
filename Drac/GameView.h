// GameView.h
// Official Interface for GameViewADT
// COMP1927 14s2

#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "Globals.h"
#include "Game.h"
#include "Places.h"

typedef struct gameView *GameView;

// newGameView() creates a new game view to summarise the current state of
// the game.
//
// pastPlays is a string of all the plays made in the game so far by all
// players (including Dracula) from earliest to most recent.
//
// messages is an array containing a playerMessage for each play in the game
// so far. It will have exactly the same number of elements as there are plays
// in pastPlays. The message from the first play will be at index 0, and so on.
// The contents of each playerMessage will be exactly as provided by the player.
//
// The "PlayerMessage" type is defined in game.h.
// You are free to ignore messages if you wish.

GameView newGameView(char *pastPlays, PlayerMessage messages[]);


// disposeGameView() frees all memory previously allocated for the GameView
// toBeDeleted. toBeDeleted should not be accessed after the call.

void disposeGameView(GameView toBeDeleted);


// Functions to return simple information about the current state of the game

// Get the current round

Round getRound(GameView currentView);

// Get the id of current player - ie whose turn is it?
// Only returns a 'playerID' which is one of:
//   LORD_GODALMING (0): Lord Godalming's turn
//   DR_SEWARD      (1): Dr. Seward's turn
//   VAN_HELSING    (2): Van Helsing's turn
//   MINA_HARKER    (3): Mina Harker's turn
//   DRACULA        (4): Dracula's turn

PlayerID getCurrentPlayer(GameView currentView);

// Get the current score
// Returns a positive integer [0...366]

int getScore(GameView currentView);

// Get the current health points for a given player
// 'player' specifies which players's life/blood points to return
//    and must be a value in the interval [0...9] for Hunters, or >0 for Dracula

int getHealth(GameView currentView, PlayerID player);

// Get the current location id of a given player
// May be UNKNOWN_LOCATION if the player has not had a turn yet
// (ie at the beginning of the game when the round is 0)
// Otherwise for a hunter it should be an integer in the interval [0..70]
// For dracula it should return his location at the start of the current round
// Possible values for this:
//   in the interval [0...70] if Dracula was known to be in a city or sea
//   CITY_UNKNOWN     if Dracula was in an unknown city
//   SEA_UNKNOWN      if Dracula was in an unknown sea
//   HIDE             if Dracula was known to have made a hide move
//   DOUBLE_BACK_N    where N is [0...5], if Dracula was known to have
//                    made a double back move N positions back in the trail
//                    e.g. DOUBLE_BACK_1 is the last place place he visited
//   TELEPORT         if Dracula apparated back to Castle Dracula
//   LOCATION_UNKNOWN if the round number is 0

LocationID getLocation(GameView currentView, PlayerID player);


//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
// for the given player
// For Dracula this may include integers:
//   in the interval [0...70] if Dracula was known to be in a city or sea
//   CITY_UNKNOWN     if Dracula was in an unknown city
//   SEA_UNKNOWN      if Dracula was in an unknown sea
//   HIDE             if Dracula was known to have made a hide move
//   DOUBLE_BACK_N    where N is [0...5], if Dracula was known to have
//                    made a double back move N positions back in the trail
//                    e.g. DOUBLE_BACK_1 is the last place place he visited
//   TELEPORT         if Dracula apparated back to Castle Dracula
//
// For any player if the move does not exist yet (i.e, the start of the game),
//   the value should be UNKNOWN_LOCATION (-1)
// For example after 2 turns the array may have the contents
//   {29, 12, -1, -1, -1, -1}
// This would mean in the first move the player started on location 12
//   then moved to the current location of 29

void getHistory(GameView currentView, PlayerID player,
                 LocationID trail[TRAIL_SIZE]);


//// Functions that query the map to find information about connectivity

// connectedLocations() returns an array of LocationID that represent
//   all locations that are connected to the given LocationID.
// road, rail and sea are connections should only be considered
//   if the road, rail, sea parameters are TRUE.
// The size of the array is stored in the variable pointed to by numLocations
// The array can be in any order but must contain unique entries
// Your function must take into account the round and player id for rail travel
// Your function must take into account that Dracula can't move to
//   the hospital or travel by rail but need not take into account Dracula's trail
// The destination 'from' should be included in the array

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea);

#endif
