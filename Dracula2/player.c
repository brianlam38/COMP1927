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

// The minimum static globals I
// can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

int main(int argc, char *argv[])
{
#ifdef I_AM_DRACULA
   DracView gameState;
   //char *plays = "GGA.... SNP.... HSZ.... MLO....";
   char *plays = "GGA.... SBI.... HLS.... MED.... DNU.V.. GKL.... SRO.... HSR.... MMN.... DLIT... GGA.... SGE.... HAL.... MLV.... DHAT... GKL.... SST.... HGR.... MSW.... DBRT... GGA.... SCO.... HCA.... MLV.... DPRT... GKL.... SLIT... HAO.... MMN.... DVIT... GBD.... SBRT... HNS.... MED.... DMUT.V. GPRT... SLI.... HHAT... MNS.... DVET... GNU.... SNU.... HBR.... MHA.... DAS.... GMUT... SBR.... HVET... MBR.... DIO.... GVE.... SVE.... HAS.... MPR.... DTS.... GGO.... SGO.... HIO.... MVE.... DMS..M. GTS.... STS.... HTS.... MGO.... DAO.... GTS.... STS.... HTS.... MTS.... DCA.V.. GRO.... SGO.... HNP.... MCG.... DMAT... GFL.... SVE.... HBI.... MMS.... DSNT... GVE.... SVI.... HAS.... MAL.... DBB.... GAS.... SBD.... HIO.... MGR.... DNAT... GIO.... SCN.... HVA.... MCAV... DCFT... GTS.... SBS.... HIO.... MLS.... DGET... GMS.... SIO.... HTS.... MLS.... DSTT.M. GAL.... STS.... HGO.... MMA.... DNUT.M. GSR.... SMS.... HMR.... MLS.... DPRT... GBO.... SBA.... HCFT... MSR.... DVIT.M. GNA.... STO.... HPA.... MBU.... DBDT... GCF.... SMR.... HCO.... MSTT... DKLT.M. GGE.... SZU.... HLI.... MMU.... DCD.V.. GST.... SMU.... HBR.... MVIT... DGAT.M. GMU.... SVI.... HVI.... MBC.... DD2T.M. GVI.... SBDT... HBC.... MKLT... DHIT... GBD.... SKL.... HKL.... MCDTT.. DTPT... GKL.... SCDTVD. HCDD... MBE....";
    //char *plays = "GGE.... SLO.... HGE.... MGE.... DED.V.. GST.... SLO.... HST.... MST.... DMNT... GST.... SLO.... HST.... MST.... DHIT... GGE.... SMNT... HGE.... MGE....";
   PlayerMessage msgs[3] = { "", "", "" };
   gameState = newDracView(plays,msgs);
   decideDraculaMove(gameState);
   disposeDracView(gameState);
#else
   HunterView gameState;
   char *plays = "GCN.... SNA.... HVR.... MGW.... DED.V.. GBC.... SBO.... HBS.... MAO.... DMNT... GBE.... STO.... HIO.... MNS.... DLOT... GKL.... SSR.... HVA.... MHA.... DSWT... GBD.... SSN.... HSO.... MLI.... DD2T... GSZ.... SMA.... HVR.... MNU.... DPLT... GSZ.... SMA.... HVR.... MNU.... DHIT.V. GSZ.... SCA.... HBS.... MBR.... DEC..M. GSZ.... SCA.... HBS.... MBR.... DLET.M. GSZ.... SAO.... HIO.... MHA.... DPAT.M. GSZ.... SAO.... HIO.... MHA.... DSTT.M. GSZ.... SEC.... HTS.... MNS.... DD1T.M. GSZ.... SEC.... HTS.... MNS.... DMUT.M. GSZ.... SEC.... HMS.... MEC.... DC?.V.. GSZ.... SEC.... HMS.... MEC.... DMIT.M. GSZ.... SLE.... HMR.... MLE.... DVET.M. GSZ.... SLE.... HMR.... MLE.... DHIT.M. GSZ.... SLE.... HMR.... MLE.... DD5T.M. GSZ.... SMR.... HMIT... MBU.... DNUT.M. GVETT.. SGO.... HMUT... MST.... DPRT.V. GFL.... SVE.... HVE.... MFL.... DBRT... GVE.... SVE.... HVE.... MVE.... DHAT... GVE.... SVE.... HVE.... MVE.... DCOT... GVE.... SVE.... HVE.... MVE.... DHIT... GVE.... SMU.... HMU.... MMU.... DD1T.M. GVE.... SMU.... HMU.... MMU.... DAMT.M. GVE.... SLI.... HNU.... MNU.... DNS..M. GVE.... SLI.... HNU.... MNU.... DEDT.M. GVE.... SHA.... HBR.... MST.... DMNT.M. GVE.... SHA.... HBR.... MST.... DLOT.M. GVE.... SHA.... HBR.... MST.... DSWT.M. GVE.... SCO.... HHA.... MBU.... DHIT.M. GVE.... SCO.... HHA.... MBU.... DD3T... GVE.... SAM.... HNS.... MAM.... DPLT.M. GVE.... SAM.... HNS.... MAM.... DEC..M. GVE.... SNS.... HED.... MNS.... DLET.M. GVE.... SNS.... HED.... MNS.... DNAT.M. GVE.... SNS.... HED.... MNS.... DCFT.M. GVE.... SNS.... HED.... MNS.... DTOT.M. GVE.... SEC.... HNS.... MEC.... DHI.VM. GVE.... SEC.... HNS.... MEC.... DSRT... GVE.... SLET... HEC.... MLE.... DSNT... GVE.... SLE.... HEC.... MLE.... DD2T.M. GVE.... SNA.... HLE.... MBO.... DMAT.M. GVE.... SNA.... HLE.... MBO.... DCAT.M. GVE.... SNA.... HLE.... MBO.... DLST.V. GVE.... SBO.... HNA.... MTO.... DAO..M. GVE.... SBO.... HNA.... MTO.... DNS..M. GVE.... SBO.... HNA.... MTO.... DD1..M. GVE.... SSR.... HBB.... MSR.... DAMT.M. GVE.... SSR.... HBB.... MSR.... DCOT.M. GVE.... SMA.... HAO.... MSN.... DHIT.M. GVE.... SMA.... HAO.... MSN.... DHA.V.. GVE.... SCA.... HNS.... MBB.... DBRT... GVE.... SCA.... HNS.... MBB.... DLIT... GVE.... SAO.... HAMT... MBO.... DNUT... GVE.... SAO.... HAM.... MBO.... DSTT.M. GVE.... SAO.... HAM.... MBO.... DZUT.M. GVE.... SNS.... HCO.... MPA.... DHIT.V. GVE.... SNS.... HCO.... MPA.... DD1T.M. GVE.... SHA.... HFR.... MCO.... DGET.M. GVE.... SHA.... HFR.... MCO.... DMRT.M. GVE.... SCO.... HNU.... MBU.... DTOT.M. GVE.... SCO.... HNU.... MBU.... DBAT.M. GVE.... SCO.... HNU.... MBU.... DSRT.M. GVE.... SCO.... HNU.... MBU.... DD3.VM. GVE.... SMRT... HST.... MLE.... DCFT.M. GGO.... SZU.... HBU.... MMR.... DNAT... GGO.... SZU.... HBU.... MMR.... DHIT.M. GGO.... SMR.... HSRT... MTOV... DBB..M. GMR.... SBO.... HSN.... MBA.... DAO.... GMR.... SBO.... HSN.... MBA.... DNS.... GMR.... SPA.... HAL.... MBO.... DD1..M. GMR.... SPA.... HAL.... MBO.... DAMT.M. GMR.... SPA.... HAL.... MBO.... DHIT.M. GMR.... SNA.... HMS.... MBB.... DBUT... GMR.... SNA.... HMS.... MBB.... DSTT... GMR.... SBB.... HAO.... MAO.... DMUT... GMR.... SBB.... HAO.... MAO.... DD1.V.. GMR.... SAO.... HNS.... MNS.... DVET.M. GMR.... SAO.... HNS.... MNS.... DHIT.M. GMR.... SNS.... HAM.... MAM.... DFLT.M. GMR.... SNS.... HAM.... MAM.... DROT.M. GMR.... SHA.... HBU.... MCO.... DBIT.M. GMR.... SHA.... HBU.... MCO.... DD2T.V. GMR.... SBR.... HMR.... MST.... DNPT.M. GMR.... SBR.... HMR.... MST.... DTS..M. GMR.... SHA.... HGE.... MGE.... DCGT.M. GMR.... SHA.... HGE.... MGE.... DHIT.M. GMR.... SBR.... HROT... MMR.... DMS..M. GMR.... SBR.... HRO.... MMR.... DALT... GMR.... SLI.... HBI.... MMI.... DSR.VM. GMR.... SLI.... HBI.... MMI.... DD1T... GMR.... SCO.... HNP.... MMR.... DMAT.M. GMR.... SCO.... HNP.... MMR.... DCAT.M. GMR.... SAM.... HTS.... MMS.... DLST... GMR.... SAM.... HTS.... MMS.... DSNT.M. GMR.... SBU.... HMS.... MAL.... DHIT.V. GMR.... SBU.... HMS.... MAL.... DBB..M. GMR.... SPA.... HAL.... MSR.... DD1..M. GMR.... SPA.... HAL.... MSR.... DAO..M. GMR.... SBO.... HSNTT.. MMA.... DNS..M. GBO.... SLS.... HSN.... MSN.... DHAT... GBO.... SLS.... HSN.... MSN.... DCOT... GBO.... SAO.... HBB.... MBB.... DST.V.. GBO.... SAO.... HBB.... MBB.... DZUT... GBO.... SAO.... HAO.... MAO.... DD2T... GBO.... SAO.... HAO.... MAO.... DMUT... GBO.... SNS.... HNS.... MNS.... DZAT.M. GBO.... SNS.... HNS.... MNS.... DHIT.M. GBO.... SAM.... HAM.... MHA.... DSJT.V. GBO.... SAM.... HAM.... MHA.... DVAT.M. GBO.... SAM.... HAM.... MHA.... DD2T.M. GBO.... SCO.... HBU.... MLI.... DSOT.M. GBO.... SCO.... HBU.... MLI.... DSAT.M. GBO.... SCO.... HBU.... MLI.... DIO..M. GBO.... SLI.... HFR.... MMU....";
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
