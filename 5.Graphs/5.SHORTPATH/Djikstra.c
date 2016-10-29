// ###############
// DJIKSTRA'S ALGO - PSUEDOCODE
// ###############

declare + init vSet to {start}    	  // start = source v
declare dist[]
init all dist[] values = infinity 	  // infinity = impossible value
while (any edges left to consider) {
	choose an edge e = {s,t}
		where s is within vSet
		and e is the min weight edge
	relax along e, add t to vSet
}

// ###############
// DJIKSTRA'S ALGO
// ###############

// vSet = set of vertices which we have seen so far	

void shortestPath(Graph g, Vertex s) {
	VertexSet vSet = {s};				// init start vertex S
	float dist[nV(g)] = {inf,inf,...};	// MALLOC dist[] and init as infinity
	dist[s] = 0;						// set dist[s] = 0
	Vertex pred[nV(g)] = {-1,-1,...};	// MALLOC pred[] and init as -1 (impossible vertex value)
	Edge eSet = edges(g);				// create set of edges
	/* FIND NEXT EDGE (while eSet !empty) */
	while (!empty(eSet)) {
		find (Edge e) satisfying {      // find an edge satisfying the following properties:
			e = (s,t,w)						// S within vSet (something already visited)			
		}									// w is mininum weight of all such edges
		/* EDGE RELAXATION */
		if (t !within vSet || dist[s]+w < dist[t]) {	// if going to T through S is cheaper
			dist[t] = dist[s]+w;							// update dist
			pred[t] = s;									// update pred = S
		}
		vSet += t;	// add T to vSet
		eSet -= e;	// remove edge from eSet

		// Loop until all edges have been considered
	}
	// dist[] and pred[] are now set appropriately
}

// ###############################
// DJIKSTRA'S ALGO - PQUEUE METHOD
// ###############################

void shortestPath(Graph g, Vertex start, Vertex pred[], float dist[]) {
	PQueue pq = newPQ(dist,nV(g));			// create PQ based on dist[] -> Inside PQ data structure,
											// store pointer to dist[]. When working out priority,
											// it will access array to find cheapest one.
	for (Vertex v = 0; v < nV(g); v++) {
		pred[v] = -1;						// init pred[] values to -1
		dist[v] = MAX_WT;					// init dist[] values to MAX_WEIGHT
		join(pq,v);							// add V to PQ
	}
	dist[start] = 0.0;						// init dist[S] = 0
	reorder(pq,start);						// reorder PQ -> S moves to the front (has priority)

	while (!empty(pq)) {
		Vertex S = leave(pq);				// pop S off PQueue / grab cheapest vertex
		/* (1) ITERATE THROUGH NEIGHBOURS */
		for (Vertex t = 0; t < nV(g); t++) {// iterate through other vertices
			float len = g->adj[s][t];		// find a vertex T connected with vertex S (neighbours)
			if (len == NO_EDGE) continue;	// no edge found, continue through next iteration
			/* (2) EDGE RELAXATION */
			if (dist[s]+len < dist[t]) {	// if cost of path < known cost of path
				pred[t] = s;				// set pred[t] = S
				dist[t] = dist[s]+len;		// update new shortest dist
				/* (3) FIX PQ AS T-WEIGHT UPDATED */
				reorder(pq,t);
			}
		}
	}
}





