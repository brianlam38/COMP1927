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
		if (dist[s]+w < dist[t]) {	// if going to T through S is cheaper
			dist[t] = dist[s]+w;							// update dist
			pred[t] = s;									// update pred = S
		}
		vSet += t;	// add T to vSet
		eSet -= e;	// remove edge from eSet

		// Loop until all edges have been considered
	}
	// dist[] and pred[] are now set appropriately
}

// ######################################
// DJIKSTRA'S ALGO - PQUEUE (REAL) METHOD
// ######################################

#define MAX_WT 			// value larger than any real weight
#define NO_EDGE			// value in adj matrix to indicate no edge

/* PQ INTERFACE */

// use dist[] array to determine PQ priorities
// lookup cost in dist[] to work on highest priority element
PQueue newPQ(float dist[], int nV);
// add v to priority queue
void join(PQueue, Vertex);
// remove v with smallest dist[] + grab value
Vertex leave(PQueue, Vertex);
// re-order queue based on weight change to vertex
void reorder(PQueue, Vertex);
// check whether queue is empty
int empty(PQueue);
// clean up queue data
void disposePQ(PQueue);

/* DJIKSTRA'S SHORTEST PATH */
void shortestPath(Graph g, Vertex start,
				  Vertex pred[], float dist[]) {

	PQueue pq = newPQ(dist,nV(g));			// #1 create PQ based on dist[] -> ptr access to dist[]

	for (Vertex v = 0; v < nV(g); v++) {	// #2 Init dist[] pred[] to base values for each vertex
		pred[v] = -1;						//    Add vertex to Queue
		dist[v] = MAX_WT;
		join(pq,v);
	}

	dist[start] = 0.0;						// #3 Begin at source vertex + reorder PQ
	reorder(pq,start);						//    (moves v to the front)

	while (!empty(pq)) {					// #4 PQ operations
		Vertex S = leave(pq);				//    Grab cheapest/front v in the queue
		/* FIND NEIGHBOURS */
		for (Vertex t = 0; t < nV(g); t++) {//    Iterate through other vertices
			float len = g->adj[s][t];		//    Find connected vertices to S (neighbours) + grab weight
			if (len == NO_EDGE) continue;	//    Length = non-existent, continue / check next T
			/* EDGE RELAXATION */
			if (dist[s]+len < dist[t]) {	//    If S cost so far + edge cost < known cost of path to T
				pred[t] = s;				//    Update new shortest path (previous of T = S)
				dist[t] = dist[s]+len;		//    Update new shortest dist (dist = dist[S] + len)
				reorder(pq,t);
			}
			/* NEXT ITERATION */
		}
	}
}





