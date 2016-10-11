void shortestPath(Graph g, Vertex s) {
	VertexSet vSet = {s};				// init start vertex S
	float dist[nV(g)] = {inf,inf,...};	// malloc dist[] and init as infinity
	dist[s] = 0;						// init S distance = 0
	Vertex pred[nV(g)] = {-1,-1,...};	// malloc pred[] and init as -1 (impossible vertex value)
	Edge eSet = edges(g);			// create set of edges

	while (!empty(eSet)) {
		find (Edge e) satisfying {      // find an edge satisfying the following properties:
			e = (s,t,w)						// S within vSet			
		}									// w is mininum weight of all such edges
		/* EDGE RELAXATION */
		if (dist[s]+w < dist[t]) {		// if going to T through S is cheaper
			dist[t] = dist[s]+w;			// update dist
			pred[t] = s;					// update pred = S
		}
		vSet += t;	// add T to vSet
		eSet -= e;	// remove edge from eSet

		// Loop until all edges have been considered
	}
	// dist[] and pred[] are now set appropriately
}