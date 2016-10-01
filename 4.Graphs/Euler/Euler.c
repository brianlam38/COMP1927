// ############################
// EULER PATH AND TOUR CHECKING
// ############################

// Check whether graph has an Euler path.
// Euler path = connected + v,w has odd degree
// (graph is assumed to be connected)
// Complexity = Linear O(v)
int hasEulerPath(Graph g, Vertex v, Vertex w) {
	int t = degree(g,v)
	int z = degree(g,w);
	if ((t % 2) == 0 && (z % 2) == 0)		// Check if v,w are odd
		return 0;
	Vertex x;
	for (x = 0; x < nV(g); x++) {			// Check remaining vertices
		if (x != v && x != w) {				// to see if they are even
			if ((degree(g,w) % 2) != 0) {
				return 0;
			}
		}
	}
	return 1;	// Return TRUE if v,w = odd + rest are even
}

