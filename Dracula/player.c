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
   //char *plays = "GST.... SLS.... HVR.... MMN.... DRO.V.. GZU.... SSR.... HCN.... MLO.... DBIT... GMU.... SMR.... HGA.... MEC.... DNPT... GZA.... STO.... HCD.... MLE.... DTS.... GVI.... SCF.... HKL.... MPA.... DGOT... GPR.... SNA.... HBD.... MST.... DMRT... GVE.... SPA.... HZA.... MZU.... DMS..V. GFL.... SNA.... HSZ.... MMRT... DAO..M. GGOT... SBB.... HVI.... MBO.... DCAT.M. GMR.... SNA.... HMU.... MCF.... DD2.... GMS.... SPA.... HZU.... MGE.... DNS.... GMR.... SGE.... HMI.... MST.... DHAT... GMS.... SPA.... HVE.... MPA.... DBRT... GTS.... SSR.... HAS.... MLE.... DPR.V.. GRO.... SLS.... HIO.... MEC.... DNUT.M. GNP.... SSN.... HSA.... MLO.... DLIT... GBI.... SMA.... HVR.... MMN.... DFRT... GAS.... SGR.... HSO.... MLV.... DCOT.M. GVE.... SCA.... HVA.... MSW.... DBUT.M. GMU.... SAO.... HSJ.... MIR.... DPAT.V. GST.... SMS.... HVA.... MAO.... DCFT.M. GZU.... SBA.... HIO.... MMS.... DNAT.M. GMI.... SLS.... HTS.... MTS.... DD3T.M. GRO.... SSN.... HIO.... MMS.... DSTT.M. GNP.... SSR.... HSA.... MAO.... DMUT.M. GBI.... STO.... HVR.... MNS.... DZAT.M. GAS.... SMR.... HSO.... MED.... DSZ.VM. GVE.... SZU.... HVA.... MMN.... DKLT.M. GMUT... SMU.... HSO.... MED.... DCDT.M. GZAT... SZA.... HBC.... MNS.... DHIT.M. GSZV... SSZ.... HKLT... MHA.... DGAT... GGATD.. SKL.... HGAD... MBR.... DCNT... GSZ.... SBC.... HBC.... MBD.... DBS.... GBE.... SSO.... HSO.... MZA.... DIO.... GVR.... SVR.... HVR.... MSZ.... DVAT.M. GCNT... SBS.... HSO.... MBC.... DD2..M. GGA.... SIO.... HVAT... MCN.... DTS.... GCN.... SBS.... HIO.... MBS.... DGOT... GVR.... SCN.... HAT.... MIO.... DMRT... GSA.... SVR.... HVA.... MVA.... DCF.V.. GVA.... SSO.... HSJ.... MAT.... DNAT... GSJ.... SVA.... HZA.... MIO.... DBOT... GZA.... SIO.... HMU.... MTS.... DSRT... GMU.... STS.... HMI.... MMS.... DTOT.M. GST.... SMS.... HRO.... MAO.... DD5T.M. GZU.... SBA.... HTS.... MNS.... DPAT.V. GMR.... SLS.... HIO.... MED.... DSTT.M. GTOT... SAO.... HTS.... MNS.... DMUT.M. GMR.... SNS.... HGO.... MAM.... DZAT.M. GPAT... SHA.... HVE.... MNS.... DSZT... GSTT... SNS.... HMI.... MHA.... DKLT.M. GMUT... SHA.... HMU.... MLI.... DCDT... GZAT... SLI.... HZA.... MVI.... DGA.V.. GSZT... SBR.... HSZ.... MZA.... DCNT... GBE.... SBD.... HBE.... MSZ.... DBS.... GGAV... SZA.... HSO.... MBE.... DIO.... GCNT... SSZ.... HVR.... MJM....";
   char *plays = "GST.... SLS.... HVR.... MMN.... DRO.V.. GZU.... SSR.... HCN.... MLO.... DBIT... GMU.... SMR.... HGA.... MEC.... DNPT... GZA.... STO.... HCD.... MLE.... DTS.... GVI.... SCF.... HKL.... MPA.... DCGT... GPR.... SNA.... HBD.... MST.... DMS.... GVE.... SPA.... HZA.... MZU.... DALT.V. GFL.... SNA.... HSZ.... MMR.... DMAT.M. GRO.... SBB.... HBE.... MLE.... DGRT.M. GNP.... SSN.... HVR.... MEC.... DCAT... GBI.... SLS.... HSO.... MLO.... DD2T.M. GAS.... SMAT... HVA.... MEC.... DHIT... GVE.... SSR.... HIO.... MAO.... DALT.M. GMI.... SMA.... HTS.... MCAT... DMS.... GMR.... SGRTTT. HMS.... MMA.... DAO.... GMS.... SMA.... HAO.... MLS.... DEC.... GMR.... SALT... HCA.... MMA.... DLOT... GTO.... SZA.... HGR.... MSR.... DMNT... GCF.... SVI.... HAL.... MSN.... DLVT... GPA.... SMU.... HMS.... MBB.... DSWT... GST.... SMI.... HTS.... MAO.... DIR.... GZU.... SRO.... HIO.... MNS.... DDUT... GGE.... STS.... HBS.... MED.... DGWT.M. GMR.... SMS.... HVR.... MMNT... DAO.... GTO.... SAO.... HSO.... MLO.... DMS..M. GSR.... SLS.... HVA.... MSWT... DMRT... GSN.... SMA.... HSJ.... MLV.... DGO.V.. GBB.... SGR.... HVA.... MIR.... DMIT.M. GNA.... SAL.... HIO.... MAO.... DMUT.M. GPA.... SMS.... HTS.... MMS.... DZAT... GST.... STS.... HIO.... MAO.... DSZT... GFL.... SRO.... HSA.... MNS.... DKLT.M. GVE.... STS.... HVR.... MED.... DBDT.V. GMUT... SRO.... HBS.... MNS.... DD2T.M. GVI.... STS.... HCN.... MHA.... DCDT... GBDT... SIO.... HGA.... MBR.... DGAT.M. GKLTT.. SSA.... HGATD.. MBD.... DCNT.M. GBC.... SSO.... HBC.... MSZ.... DBS.... GBE.... SVA.... HSO.... MZA.... DIO.... GCNT... SIO.... HVR.... MSZ.... DTS.... GSZ.... SBS.... HSO.... MBC.... DGOT.M. GKL.... SCN.... HSJ.... MGA.... DMRT... GCD.... SVR.... HJM.... MKL.... DCFT... GKL.... SBS.... HZA.... MBD.... DTOT... GSZ.... SIO.... HVI.... MBR.... DSRT... GZA.... STS.... HVE.... MHA.... DSNT... GMU.... SMS.... HFL.... MNS.... DMAT.M. GST.... SAO.... HRO.... MED.... DALT.M. GZU.... SLS.... HTS.... MMN.... DGRT.M. GMI.... SSNT... HMS.... MLO.... DCAT.M. GMR.... SBB.... HMR.... MEC.... DD2T.M. GBU.... SNA.... HTO.... MLE.... DHIT... GPA.... SCF.... HMR.... MNA.... DMA.VM. GGE.... SBO.... HMS.... MPA.... DSRT.M. GST.... SAL.... HTS.... MST.... DTOT.M. GFL.... SMAV... HMS.... MPA.... DBOT.M. GGO.... SSRT... HBA.... MBOTD.. DBB..M. GTS.... SLS.... HMS.... MZA.... DNAT.M. GMS.... SAO.... HAO.... MMU.... DPAT... GMR.... SCA.... HCA.... MZU.... DD2T... GMS.... SGR.... HCA.... MMR.... DLET.M. GAO.... SAL.... HGR.... MMS.... DBUT... GNS.... SMS.... HAL.... MAO.... DSTT... GHA.... STS.... HMS.... MNS.... DMUT.M. GCO.... SMS.... HTS.... MED.... DZAT.M. GSTT... SMR.... HMS.... MNS.... DSJ.VM. GPA.... SCF.... HMR.... MAM.... DBET.M. GST.... STO.... HMI.... MCO.... DKLT.M. GNU.... SMR.... HVE.... MST.... DGAT... GST.... SBO.... HAS.... MPA.... DCDT.M. GMU.... SMR.... HVE.... MST.... DHIT.M. GZA.... SMI.... HVI.... MMU.... DD3T.V. GSZ.... SVE.... HBD.... MZA.... DCNT.M. GBE.... SGO.... HBE.... MSZ.... DBS..M. GSZ.... STS.... HKL.... MKL.... DIO..M. GGAT... SRO.... HCDTT.. MBC.... DVAT... GCNT... STS.... HGA.... MSO.... DATT... GBS.... SIO.... HCN.... MVR.... DHIT... GCN.... SVAT... HGA.... MSO....";
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
