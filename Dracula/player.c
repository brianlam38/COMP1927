/**
 * Runs a player's game turn ...
 *
 * Can produce either a Hunter player or a Dracula player
 * depending on the setting of the I_AM_DRACULA #define
 *
 * This is a dummy version of the real player.c used when
 * you submit your AIs. It is provided so that you can
 * test whether your code is likely to compile ...
 *
 * Note that it used to drive both Hunter and Dracula AIs.
 * It first creates an appropriate view, and then invokes the
 * relevant decide-my-move function, which should use the
 * registerBestPlay() function to send the move back.
 *
 * The real player.c applies a timeout, so that it will halt
 * decide-my-move after a fixed amount of time, if it doesn't
 * finish first. The last move registered will be the one
 * used by the game engine. This version of player.c won't stop
 * your decide-my-move function if it goes into an infinite
 * loop. Sort that out before you submit.
 *
 * Based on the program by David Collien, written in 2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#ifdef I_AM_DRACULA
#include "DracView.h"
#include "dracula.h"
#else
#include "HunterView.h"
#include "hunter.h"
#endif

// moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

int main(int argc, char *argv[])
{
#ifdef I_AM_DRACULA
   DracView gameState;
   char *plays = "GST.... SLS.... HVR.... MGW.... DED.V.. GZU.... SSN.... HSO.... MDU.... DHIT... GMU.... SSR.... HVA.... MIR.... DNS.... GZA.... STO.... HSJ.... MSW.... DAMT... GVI.... SMR.... HJM.... MLO.... DCOT... GPR.... SZU.... HSZ.... MPL.... DD2T... GPR.... SZU.... HSZ.... MPL.... DBUT.V. GPR.... SZU.... HSZ.... MPL.... DLET.M. GPR.... SZU.... HSZ.... MPL.... DNAT... GPR.... SZU.... HSZ.... MPL.... DBB..M. GPR.... SZU.... HSZ.... MPL.... DSNT.M. GPR.... SZU.... HSZ.... MPL.... DMAT.M. GPR.... SST.... HZA.... MEC.... DALT.M. GPR.... SST.... HZA.... MEC.... DGR.VM. GPR.... SPA.... HMU.... MLE.... DHIT.M. GPR.... SPA.... HMU.... MLE.... DD3T... GPR.... SPA.... HMU.... MLE.... DMS..M. GPR.... SPA.... HMU.... MLE.... DBAT.M. GPR.... SPA.... HMU.... MLE.... DSRT.M. GPR.... SPA.... HMU.... MLE.... DMAT.V. GPR.... SPA.... HMU.... MLE.... DGRT.M. GPR.... SPA.... HMU.... MLE.... DHIT.M. GPR.... SMR.... HZU.... MEC.... DD3T... GPR.... SMR.... HZU.... MEC.... DSNT.M. GPR.... SMR.... HZU.... MEC.... DBB..M. GPR.... SMR.... HZU.... MEC.... DBOT.M. GPR.... SMR.... HZU.... MEC.... DNA.VM. GPR.... SMR.... HZU.... MEC.... DCFT.M. GPR.... SMR.... HZU.... MEC.... DGET.M. GPR.... STO.... HMR.... MAO.... DSTT.M. GPR.... STO.... HMR.... MAO.... DCOT... GPR.... SSR.... HPA.... MBB.... DAMT.M. GPR.... SSR.... HPA.... MBB.... DNS..V. GPR.... SBO.... HNA.... MNA.... DEDT.M. GPR.... SBO.... HNA.... MNA.... DMNT.M. GPR.... SNA.... HPA.... MPA.... DD2T.M. GPR.... SNA.... HPA.... MPA.... DHIT.M. GPR.... SNA.... HPA.... MPA.... DTPT.M. GPR.... SNA.... HPA.... MPA.... DGAT... GPR.... SNA.... HPA.... MPA.... DCN.VM. GPR.... SNA.... HPA.... MPA.... DBS..M. GPR.... SNA.... HPA.... MPA.... DVRT.M. GPR.... SNA.... HPA.... MPA.... DD2..M. GPR.... SNA.... HPA.... MPA.... ";
   PlayerMessage msgs[3] = { "", "", "" };
   gameState = newDracView(plays,msgs);
   decideDraculaMove(gameState);
   disposeDracView(gameState);
#else
   HunterView gameState;
   char *plays = "GZA.... SED.... HZU....";
   PlayerMessage msgs[3] = { "", "", "" };
   gameState = newHunterView(plays,msgs);
   decideHunterMove(gameState);
   disposeHunterView(gameState);
#endif
   printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
   return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay (char *play, PlayerMessage message) {
   strncpy(latestPlay, play, MOVE_SIZE-1);
   latestPlay[MOVE_SIZE-1] = '\0';

   strncpy(latestMessage, message, MESSAGE_SIZE);
   latestMessage[MESSAGE_SIZE-1] = '\0';
}
