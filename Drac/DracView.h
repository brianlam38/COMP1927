// DracView.h
// Official Interface for DracViewADT
// COMP1927 14s2

#ifndef DRAC_VIEW_H
#define DRAC_VIEW_H

#include "Globals.h"
#include "Game.h"
#include "Places.h"
#include "GameView.h"

typedef struct dracView *DracView;

// newDracView() creates a new game view to summarise the current state of
// the game.
//
// pastPlays is a string of all the plays made in the game so far by all
// players (including Dracula) from earliest to most recent.
//
// For Dracula
// * the pastPlays string should contain full details of all moves
// * the last move should always be by Mina Harker
//
// messages is an array containing a playerMessage for each play in the game
// so far. It will have exactly the same number of elements as there are plays
// in pastPlays. The message from the first play will be at index 0, and so on.
// The contents of each playerMessage will be exactly as provided by the player.
//
// The "PlayerMessage" type is defined in Game.h.
// You are free to ignore messages if you wish.

DracView newDracView(char *pastPlays, PlayerMessage messages[]);


// disposeDracView() frees all memory previously allocated for the DracView
// toBeDeleted. toBeDeleted should not be accessed after the call.

void disposeDracView(DracView toBeDeleted);


// Functions to return simple information about the current state of the game

// Get the current round

Round giveMeTheRound(DracView currentView);

// Get the current score
// Returns a positive integer [0...366]

int giveMeTheScore(DracView currentView);

// Get the current health points for a given player
// 'player' specifies which players's life/blood points to return
//    and must be a value in the interval [0...4] (see 'player' type)

int howHealthyIs(DracView currentView, PlayerID player);

// Get the current location id of a given player
// May be UNKNOWN_LOCATION if the player has not had a turn yet
//   (ie at the beginning of the game when the round is 0)
// Always returns an exact location
//   (assumes that the pastPlays string contains full Dracula
//    locations since Dracula always knows where he's been)

LocationID whereIs(DracView currentView, PlayerID player);

// Get the most recent move of a given player
// Returns the start location and end location of that move
// Since Dracula moves last, the end location for other players is always known,
//   but the start location may be UNKNOWN_LOCATION (for a hunter's first move)
// The start and end locations can be the same

void lastMove(DracView currentView, PlayerID player,
                 LocationID *start, LocationID *end);

// Find out what minions I (Dracula) has placed at the specified location
//   (minions are traps and immature vampires)
// Places counts in the vars referenced by the 3rd and 4th parameters
// If where is not a place where minions can be left (e.g. at sea,
//   or NOWHERE), then set both counts to zero

void whatsThere(DracView currentView, LocationID where,
                         int *numTraps, int *numVamps);


//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
//   for the given player, including Dracula (if he asks about his own trail)
// For any player if the move does not exist yet (i.e, the start of the game),
//   the value should be UNKNOWN_LOCATION (-1)
// For example after 2 turns the array may have the contents
//   {29, 12, -1, -1, -1, -1}
// This would mean in the first move the player started on location 12
//   then moved to the current location of 29
// If Dracula asks about his own trail, he should get precise information
//   about where he has been (real locations, not double-back, etc)

void giveMeTheTrail(DracView currentView, PlayerID player,
                        LocationID trail[TRAIL_SIZE]);


//// Functions that query the map to find information about connectivity

// whereCanIgo() returns an array of LocationIDs giving
//   all locations that Dracula could reach in his next move
// road and/or sea connections should only be considered
//   if the road and/or sea parameters are TRUE.
// The size of the array is stored in the variable pointed to by numLocations
// The array can be in any order but must contain unique entries
// Should not include the hospital nor any locations only reachable by rail
// The current location should be included in the array
// The set of possible locations must be consistent with the rules on Dracula's
//   movement (e.g. can't MOVE to a location currently in his trail)

LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea);

// whereCanTheyGo() returns an array of LocationIDs giving all of the
//   locations that the given Player could reach from their current location
// road, rail and sea are connections should only be considered
//   if the road, rail, sea parameters are TRUE.
// The size of the array is stored in the variable pointed to by numLocations
// The array can be in any order but must contain unique entries
// The function must take into account the round for determining how far
//   a player can travel by rail
// If the given player is Dracula, this function calls whereCanIgo()
//   to produce the answers
// The player's current location should be included in the array

LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea);

#endif
