// count the number of possible move next round if Dracula choose dest as a location to go
int numOfNextAdj(DracView gameState, LocationID dest)
// count the number of hunters that will possibly go to dest in the next round
int numOfPossHunter(DracView gameState, LocationID dest)
// check if dracula and hunters are in the same city at the start of dracula turn
int meetHunter(DracView gameState)
// count the number of adjLoc that would not result in HI or DB
int needHiding(DracView gameState, LocationID location)
// adjust the optimsl score of a location with special cases considered
int scoreTunner(DracView gameState, LocationID dest)
// find the shortest path to escape to a sea and return the next location in that path
// minLength is the shortest length required to go to that sea
LocationID escapeToSea(DracView gameState, LocationID *adjLoc, int numLoc, int *minLength)
// count the number of hunters in a given location in current round
int numOfHunter(DracView gameState, LocationID location)
// check if dracula is in a dangerous situation
int isDangerous(DracView gameState, LocationID currLoc, LocationID trail[TRAIL_SIZE])
// Dracula version of perdict Dracula move
// to predict the moves of dracula in next few round when one of the dracula's location is known
// predict where dracula will go in round round
LocationID predictDracMove(DracView gameState, LocationID dracSrc, Round round)
// to predict the moves of hunter in next round when one of the dracula's location is known
LocationID predictHunterMove(DracView gameState, PlayerID hunter)
// check which location in dracula's trail is known by the hunters
// return array of trail indexes of that know locations
int *knownByHunter(DracView gameState, int *numLoc, LocationID trail[TRAIL_SIZE])
// check if the hunter is doing a research
int isResearch(DracView gameState)
// calculate the length from destination to the nearest hunter
int furthestLength(DracView gameState, LocationID dest)
// change the location to HI or DB if needed
void locToDBOrHI(char *locAbbrev, LocationID trail[TRAIL_SIZE], LocationID hideTrail[TRAIL_SIZE])
// find the most isolated hunter, return -1 if no isolated hunter is found
PlayerID mostIsolated(DracView gameState)
// check whether the largest island is occupied by a hunter
int islandEmpty(DracView gameState)
// count the number of hunters that are able go to the given place next round
int possibleHunters(DracView gameState, LocationID place)
// higher score if the place suits dracula more
int locationScore(LocationID location, int length, int health)
// calculate an optimal score for a place which deal with all the factors
int optimalLocScore(int numOfNextAdj, int pathLength, int possHunt, int tuneScore)
// register the best play to the game engine
void submitPlay(DracView gameState, LocationID *adjLoc, LocationID best, int isHide, PlayerMessage msg)



// This one need to be modified, but keep it until we have a new version of code in decide dracula move
// find the adjacent location which is furthest away from all the hunters
LocationID furthestLocation(DracView gameState, LocationID *adjLocations,
                       int numLoc)



int isBeingKilled(DracView gameState)
int nextTo(DracView gameState, PlayerID player, LocationID *adjLoc, int numLoc)
LocationID lastStand(DracView gameState, LocationID *adjLoc, int numLoc)
int surroundedNextToSea(DracView gameState, LocationID *adjLoc, int numLoc) {
  //check if drac is surrounded with hunters in adjacent locations excluding his current location
int isSurrounded(DracView gameState, LocationID *adjLoc, int numLoc) {
  //check if drac is surrounded with hunters in adjacent locations excluding his current location

// return distance from nearest sea
int distanceFromSea(DracView gameState, LocationID location);
// return distance from nearest hunter
int distFromNearestHunter(DracView gameState, LocationID location);
// move to adjacent city that is closest to the sea
// if cities are same distance from sea, return city that is furthest from nearest hunter
// go to sea if there are no other locations Drac can go
LocationID stayNearSea(DracView gameState, LocationID *adjLoc, int numLoc);
// check if dracula should stay int city near sea but not at sea
// condition: if closest hunter is at least4 moves away
int shouldStayNearSea(DracView gameState);
// check if dracula has enough health to remain at sea until game over
int canEndAtSea(DracView gameState);
