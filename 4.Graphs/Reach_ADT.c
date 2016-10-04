// ################################
// REACHIBILITY MATRIX - ADT FORMAT
// ################################

int reachable(Graph g, Vertex src, Vertex dest) {	// Check reachability
	if (g->tc == NULL) makeClosure(g);
	return g->tc[src][dest];						// Do lookup on matrix O(1)
}

void makeClosure(Graph g) {
	int i, s, t;
	int V = g->nV;
	int **tc = makeMatrix(V, V, 0);					// Set up matrix, initialise values 0

	for (s = 0; s < V; s++) {						// Copy adj matrix values to tc matrix
		for (t = 0; t < V; t++) {
			tc[s][t] = g->edges[s][t];
		}
	}
	for (i = 0; i < V; i++) {						// Warshall's Algorithm
		for (s = 0; s < V; s++) {
			if (tc[i][s] == 0) continue;
			for (t = 0; t < V; t++) {
				if (tc[i][t] == 1) tc[s][t] = 1;
			}
		}
	}
	g->tc = tc;										// Set graph tc ptr = tc matrix
}
