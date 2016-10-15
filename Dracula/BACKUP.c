int findPathLength(LocationID src, LocationID dest, PlayerID player, Round round) {
    assert(validPlace(src));
    assert(validPlace(dest));

    Map map = newMap();
    if (src == dest) return 0;

    int *visited = calloc(map->nV,sizeof(int));
    int *previsited = calloc(map->nV,sizeof(int));
    int *newlyvisited = calloc(map->nV,sizeof(int));
    int isFound = 0;
    int length = 0;

LocationID x;
//        printf("railstop = %d\n", railStop);
//    printf("src is %s, dest is %s, player is %d, ronud is %d\n", idToName(src), idToName(dest), player, round);
        previsited[src] = 1;
        newlyvisited[src] = 1;
        visited[src] = 1;
            while (isFound != 1) {

                for (x = 0; x < map->nV; x++) {
                    if (newlyvisited[x]) {
          //              printf("putting in aaa is %s\n", idToName(x));
                        isFound = aaa(x, dest, player, round, previsited);
                        if (isFound) break;
                        newlyvisited[x] = 0;
                     }
                }

                for (x = 0; x < map->nV; x++) {
                    if (visited[x] == 0 && previsited[x] == 1) {
                        visited[x] = 1;
                        newlyvisited[x] = 1;
        //                printf("newlyvisited is %s\n", idToName(x));
                    }
                }
                round++;
                length++;
            }

    free(previsited);
    free(newlyvisited);
    free(visited);
    disposeMap(map);
//    printf("finishing findpathlength\n");
    return length;
}



// check if dest is reacheable in next move
int aaa(LocationID src, LocationID dest, PlayerID player, Round round, LocationID *previsited) {
    assert(validPlace(src));
    assert(validPlace(dest));

    Map map = newMap();
    int isFound = 0;
    int railStop = (round + player) % 4;
    LocationID w, y, z;
    VList curr, p, n;
if (player == PLAYER_DRACULA) railStop = 0;
    for (curr = map->connections[src]; curr != NULL; curr = curr->next) {
        if ((player == PLAYER_DRACULA && curr->type == RAIL) ||
            (player == PLAYER_DRACULA && curr->v == ST_JOSEPH_AND_ST_MARYS)) continue;
        else {
            y = curr->v;
            if (railStop > 1 && (curr->type == RAIL || curr->type == ANY)) {
                for (p = map->connections[y]; p != NULL; p = p->next) {
                        z = p->v;
                        if ((p->type == RAIL || p->type == ANY) && z != src) {
                            if (railStop == 3) {
                                for (n = map->connections[z]; n != NULL; n = n->next) {
                                    w = n->v;
                                    if ((n->type == RAIL || n->type == ANY) && w != y) {
                                        if (previsited[w]) continue;
                                        if (w == dest) {
                                            isFound = 1;
                                            break;
                                        }
                                        if (!previsited[w]) {
                                            previsited[w] = 1;
                                        }
                                    }
                                }
                            }
                            if (previsited[z]) continue;
                            if (z == dest) {
                                isFound = 1;
                                break;
                            }
                            if (!previsited[z]) {
                                previsited[z] = 1;
                            }
                        }
                    }
                }
                if (previsited[y]) continue;
                if (y == dest) {
                    isFound = 1;
                    break;
                }
                if (!previsited[y]) {
                    previsited[y] = 1;
                }
            }
        }
disposeMap(map);
    if (isFound) {
        return 1;
    } else {
        return 0;
    }
}



//LocationID *whereCanPlayerGo(DracView gameState, PlayerID player, Round move) {
//    whereCanTheyGo(DracView currentView, int *numLocations,
//                   LocationID from, PlayerID player, Round round,
//                   int road, int rail, int sea)
//    //////needa modify round num if used by hunters
//    Round round = giveMeTheRound(gameState);
//    int priNumLoc, secNumLoc, numLoc;
//    LocationID *adjLoc = whereCanTheyGo(gameState, &numLoc,
//                                        whereIs(gameState, player),
//                                        player, round+1, TRUE, TRUE, TRUE);
//    LocationID *priAdjLoc = adjLoc;
//    priNumLoc = numLoc;
//    LocationID *secAdjLoc;
//
//    LocationID from = whereIs(gameState, player);
//
//    for (x = 0; x < NUM_MAP_LOCATIONS; x++) {
//        if (findPathLength(from, x, player) <= move) {
//
//        }
//    }
//
//    while (move != 0) {
//        if (move == 1) return adjLoc;
//
//        for (x = 0; x < priNumLoc; x++) {
//            tmpAdjLoc = whereCanTheyGo(gameState, &tmpNumLoc,
//                                       priAdjLoc[x],
//                                       player, round+1, TRUE, TRUE, TRUE);
//            if (secAdjLoc == NULL) {
//                secAdjLoc = tmpAdjLoc;
//                secNumLoc = tmpNumLoc;
//            } else {
//                for (i = 0; i < tmpNumLoc; i++) {
//                    if (inArray(secAdjLoc, tmpAdjLoc[i], secNumLoc) != -1) {
//                        secNumLoc++;
//                        adjLoc = realloc(adjLoc, numLoc * sizeof(LocationID));
//                        assert(adjLoc != NULL);
//                        adjLoc[numLoc - 1] = secAdjLoc[i];
//                    }
//            }
//            //put the nearby cities of type RAIL to
//            // the connections array without duplicates
//            for (i = 0; i < secNumLoc; i++) {
//                for (j = 0; j < numLoc; j++) {
//                    if (adjLoc[j] == secNumLoc[i]) j = numLoc;
//                }
//                //only the "new" locations are added to the connections array
//                if (j != numLoc + 1) {
//                    numLoc++;
//                    adjLoc = realloc(adjLoc, numLoc * sizeof(LocationID));
//                    assert(adjLoc != NULL);
//                    adjLoc[numLoc - 1] = secAdjLoc[i];
//                }
//            }
//            priAdjLoc = secAdjLoc;
//            priNumLoc = secNumLoc;
//        }
//        round--;
//    }
//
//}



LocationID howToGetTo(LocationID dest, LocationID from, int round,
                             int player, int *pathLength, int sea, int train) {

    if (dest == from) return dest;

    LocationID seenList[NUM_MAP_LOCATIONS] = {0};
    LocationID prevList[NUM_MAP_LOCATIONS] = {0};
    LocationID stepList[NUM_MAP_LOCATIONS] = {0};
    Queue toVisit = newQueue();
    Map map = newMap();
//    int steps = (round+player)%4;

    seenList[from] = 1 + train - train;
    prevList[from] = -1;

    if (train)                                    // move by train
        stepList[from] = (round+player)%4;
    QueueJoin(toVisit,from);
    VList i;

    while(!QueueIsEmpty(toVisit) && !seenList[dest]) {      // while queue !empty & dest !reached

        LocationID curr = QueueLeave(toVisit);
//    printf("Curr = %d\n",curr);

        for (i = map->connections[curr]; i != NULL; i=i->next) {                // loop through adj cities (next moves)

            if ((i->type == BOAT && sea) || i->type == ROAD || (train && stepList[curr]+ RAIL >= i->type)) {
                if (!seenList[i->v]) {                // if location has not been seen:
                    seenList[i->v] = 1;                   // Mark location as seen = 1
                    prevList[i->v] = curr;                // Store location in prevList
                    if (train)
                        stepList[i->v] = (stepList[curr] + 1)%4;
                }
                if (seenList[dest]) break;                      // if dest is found, break loop
                QueueJoin(toVisit,i->v);              // add connection to queue
            }
        }
    }
    dropQueue(toVisit);
    LocationID curr = dest;
    while (prevList[curr] != from) {    // Traversing back
        curr = prevList[curr];
    }

    return curr;
}


// check if there's a double-back or hide in Dracula's trail
int hasDBOrHI(LocationID trail[TRAIL_SIZE]) {
    int hide = 0;
    int douB = 0;
    int i, j;
    for (i = 0; i < TRAIL_SIZE - 1; i++) {
        if (trail[i] == HIDE) {
            hide++;
        }
    }
    for (j = 1; j < TRAIL_SIZE; j++) {
        for (i = 0; i < TRAIL_SIZE - j; i++) {
            if (trail[i] == (HIDE + j)) {
                douB++;
            }
        }
    }
    if ((hide == 1) && (douB == 1)) {
        return BOTH_HIDE_AND_DB;
    } else if (hide == 1) {
        return HAS_HIDE;
    } else if (douB == 1) {
        return HAS_DOUBLE_BACK;
    } else {
        return NO_SPECIAL_MOVE;
    }
}


// check if there's a double-back or hide in Dracula's trail
int hasDBOrHI(LocationID trail[TRAIL_SIZE]) {
    int hide = 0;
    int douB = 0;
    int i;
    for (i = 0; i < TRAIL_SIZE - 1; i++) {
        if (trail[i] == HIDE) {
            hide++;
    }
    if (hide == 0) {
        for (i = 0; i < TRAIL_SIZE - 1; i++) {
            if (trail[i] == DOUBLE_BACK_1) return DOUBLE_BCAK_1;
            if (trail[i] == DOUBLE_BACK_2) return DOUBLE_BCAK_2;
            if (trail[i] == DOUBLE_BACK_3) return DOUBLE_BCAK_3;
            if (trail[i] == DOUBLE_BACK_4) return DOUBLE_BCAK_4;
            if (trail[i] == DOUBLE_BACK_5) return DOUBLE_BCAK_5;
        }
        return NO_SPECIAL_MOVE;
    } else {
        for (i = 0; i < TRAIL_SIZE - 1; i++) {
            if (trail[i] == DOUBLE_BACK_1 ||
                trail[i] == DOUBLE_BACK_2 ||
                trail[i] == DOUBLE_BACK_3 ||
                trail[i] == DOUBLE_BACK_4 ||
                trail[i] == DOUBLE_BACK_5)
                return BOTH_HIDE_AND_DB;
        }
        return HIDE;
}


void locToDBOrHI(char *locAbbrev, DracView currentView) {
    LocationID trail[TRAIL_SIZE];
    LocationID hideTrail[TRAIL_SIZE];
    giveMeTheTrail(currentView, PLAYER_DRACULA, trail, FALSE);
    giveMeTheTrail(currentView, PLAYER_DRACULA, hideTrail, TRUE);
    shiftRight(trail, 0, TRAIL_SIZE - 1);
    shiftRight(hideTrail, 0, TRAIL_SIZE - 1);
    trail[0] = abbrevToID(locAbbrev);
    hideTrail[0] = abbrevToID(locAbbrev);

    if (inHideTrail(hideTrail) == BOTH_HIDE_AND_DB) return;
    if (trail[0] == trail[1] &&
        (hasDBOrHI(hideTrail) != HAS_HIDE) &&
        idToType(trail[0]) != SEA &&
        inHideTrail(hideTrail) != HAS_HIDE) {
        strcpy(locAbbrev, "HI");
    } else if (trail[0] == trail[1] &&
               inHideTrail(hideTrail) != HAS_DOUBLE_BACK) {
        strcpy(locAbbrev, "D1");
    } else if (trail[0] == trail[2] &&
               inHideTrail(hideTrail) != HAS_DOUBLE_BACK) {
        strcpy(locAbbrev, "D2");
    } else if (trail[0] == trail[3] &&
               inHideTrail(hideTrail) != HAS_DOUBLE_BACK) {
        strcpy(locAbbrev, "D3");
    } else if (trail[0] == trail[4] &&
               inHideTrail(hideTrail) != HAS_DOUBLE_BACK) {
        strcpy(locAbbrev, "D4");
    } else if (trail[0] == trail[5] &&
               inHideTrail(hideTrail) != HAS_DOUBLE_BACK) {
        strcpy(locAbbrev, "D5");
    }
    return;
}

if (dbOrHi == HIDE ||
        idToType(whereIs(currentView, PLAYER_DRACULA)) == SEA) {
printf("st hide\n");
        shiftLeft(adjLoc, 0, (*numLocations) - 1);
        (*numLocations)--;
        if ((*numLocations) > 0) {
            adjLoc = realloc(adjLoc, (*numLocations) * sizeof(LocationID));
            assert(adjLoc != NULL);
        } else {
            free(adjLoc);
            adjLoc = NULL;
        }
printf("fi hide\n");
    } else

//// Functions that query the map to find information about connectivity
// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    assert(currentView != NULL);
    assert(from >= MIN_MAP_LOCATION && from <= MAX_MAP_LOCATION);
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    assert(round >= 0);

    *numLocations = 1;        //initialise the array size
    LocationID *connections = malloc((*numLocations) * sizeof(LocationID));
    assert(connections != NULL);
    connections[0] = from;    //initialise the array
    Map map = newMap();       //get the game map
    int i, j;

    //find the nearby cities of type ROAD
    if (road) {
        connections = NearbyCities(map, from, connections, numLocations, ROAD);

        //case: Dracula can't move to hospital
        // delete the location of hospital if it is in the connections array
        if (player == PLAYER_DRACULA &&
            inArray(connections, ST_JOSEPH_AND_ST_MARYS, *numLocations)) {
            for (i = 0; i < *numLocations; i++) {
                if (connections[i] == ST_JOSEPH_AND_ST_MARYS) {
                    for (j = i; j + 1 < *numLocations; j++) {
                        connections[j] = connections[j + 1];
                    }
                    i = *numLocations;
                }
            }
            //reset the size of array
            (*numLocations)--;
            connections = realloc(connections, *numLocations * sizeof(LocationID));
            assert(connections != NULL);
        }
    }

    //find the nearby cities of type BOAT
    if (sea) {
        connections = NearbyCities(map, from, connections, numLocations, BOAT);
    }

    //find the nearby cities of type RAIL
    if (rail) {
        //number of stations that is allowed to pass through
        int stationsAllowed = (player + round) % 4;
        //number of next stations
        int priNumStations = 0;
        //number of stations next to the "next next stations"
        int secNumStations = 0;
        //number elements(stations) in railConnections array
        int numStations = 1;
        //array of connected rail stations
        LocationID *railConnections = malloc(numStations * sizeof(LocationID));
        assert(railConnections != NULL);
        railConnections[0] = from;

        if (player == PLAYER_DRACULA || stationsAllowed == 0)
            return connections;

        //get the primary rail connections from the given location
        connections = NearbyCities(map, from, connections, numLocations, RAIL);
        if (stationsAllowed == 1) return connections;

        //get the primary rail connections and
        // store it into the railConnections array
        railConnections = NearbyCities(map, from, railConnections,
                                       &numStations, RAIL);
        priNumStations = numStations;
        //loop through the primary rail stations and find their nearby rail stations
        for (i = 0; i < priNumStations; i++) {
            railConnections = NearbyCities(map, railConnections[i],
                                           railConnections, &numStations, RAIL);
        }

        if (stationsAllowed == 3) {
            secNumStations = numStations;
            //loop through only the secondary rail stations and
            // find their nearby rail stations
            for (i = priNumStations - 1; i < secNumStations; i++) {
                railConnections = NearbyCities(map, railConnections[i],
                                               railConnections, &numStations, RAIL);
            }
        }

        //put the nearby cities of type RAIL to
        // the connections array without duplicates
        for (i = 0; i < numStations; i++) {
            for (j = 0; j < *numLocations; j++) {
                if (connections[j] == railConnections[i]) j = *numLocations;
            }
            //only the "new" locations are added to the connections array
            if (j != (*numLocations) + 1) {
                (*numLocations)++;
                connections = realloc(connections,
                                      *numLocations * sizeof(LocationID));
                assert(connections != NULL);
                connections[*numLocations - 1] = railConnections[i];
            }
        }
    }
    return connections;
}


//// Functions that query the map to find information about connectivity
// Returns an array of LocationIDs for all directly connected locations
LocationID *connectedLocations(int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    assert(validPlace(from));
    assert(player >= PLAYER_LORD_GODALMING && player < NUM_PLAYERS);
    assert(round >= 0);

    *numLocations = 1;
    LocationID *connections = malloc((*numLocations) * sizeof(LocationID));
    assert(connections != NULL);
    connections[0] = from;
    Map map = newMap();
    LocationID x, y, z;
    VList curr, sec, thrd;
    int *visited = calloc(map->nV, sizeof(int));
    int railStop = (round + player) % 4;
    if (player == PLAYER_DRACULA) railStop = 0;

    for (curr = map->connections[from]; curr != NULL; curr = curr->next) {
        if ((player == PLAYER_DRACULA && curr->type == RAIL) ||
            (player == PLAYER_DRACULA && curr->v == ST_JOSEPH_AND_ST_MARYS)) continue;
        else if (road != TRUE && curr->type == ROAD) continue;
        else if (sea != TRUE && curr->type == BOAT) continue;
        else if (rail != TRUE && curr->type == RAIL) continue;
        else {
            x = curr->v;
            if (railStop > 1 && (curr->type == RAIL || curr->type == ANY)) {
                for (sec = map->connections[x]; sec != NULL; sec = sec->next) {
                    y = sec->v;
                    if ((sec->type == RAIL || sec->type == ANY) && y != from) {
                        if (railStop == 3) {
                            for (thrd = map->connections[y]; thrd != NULL; thrd = thrd->next) {
                                z = thrd->v;
                                if ((thrd->type == RAIL || thrd->type == ANY) && z != x) {
                                    if (visited[z]) continue;
                                    if (!visited[z]) {
                                        visited[z] = 1;
                                    }
                                }
                            }
                        }
                        if (visited[y]) continue;
                        if (!visited[y]) {
                            visited[y] = 1;
                        }
                    }
                }
            }
            if (visited[x]) continue;
            if (!visited[x]) {
                visited[x] = 1;
            }
        }
    }
    for (x = 0; x < map->nV; x++) {
        if (visited[x]) {
            (*numLocations)++;
            connections = realloc(connections, (*numLocations) * sizeof(LocationID));
            assert(connections != NULL);
            connections[*numLocations - 1] = x;
        }
    }
    free(visited);
    disposeMap(map);
    return connections;
}


int inHideTrail(LocationID trail[TRAIL_SIZE]) {
    int i;
    int hide = 0;
    int douB = 0;
    for(i = 0; i < TRAIL_SIZE; i++) {
        if (trail[i] == HIDE) {
            hide = 1;
        } else if (trail[i] == DOUBLE_BACK_1 ||
                   trail[i] == DOUBLE_BACK_2 ||
                   trail[i] == DOUBLE_BACK_3 ||
                   trail[i] == DOUBLE_BACK_4 ||
                   trail[i] == DOUBLE_BACK_5) {
            douB = 1;
        }
    }
    if ((hide == 1) && (douB == 1)) {
        return BOTH_HIDE_AND_DB;
    } else if (hide == 1) {
        return HAS_HIDE;
    } else if (douB == 1) {
        return HAS_DOUBLE_BACK;
    } else {
        return NO_SPECIAL_MOVE;
    }
}

1927 dryrun ass2b Makefile *.c *.h
