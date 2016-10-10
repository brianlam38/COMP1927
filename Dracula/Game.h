// Game.h
// Interface to the game engine
// By: TeamDracula
// Date: 1 January 2012
// Version: 1.0
// Do not alter this file yourself.  You do not submit this file.

#ifndef GAME_H
#define GAME_H

#define MESSAGE_SIZE 100
#define LIMIT_LIMIT_MSECS 1500

// a type to contain a message (or taunt) provided by a player

typedef char PlayerMessage[MESSAGE_SIZE];

// registerBestPlay() registers your best move so far with the game engine.
// This overrides any previous moves provided by your player.
//
// the array message will be stored in the game history
// it must contain a null (ie '\0') with only letters, spaces, 
//   digits, and/or punctuation characters before it, as the array 
//   will be passed to printf("%s") when displaying messages to users.

void registerBestPlay( char *play, PlayerMessage message );

#endif
