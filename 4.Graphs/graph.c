// Graph ADT Implementations

// Adjacency Matrix Representation

Graph newGraph(int nV) {
	assert(nV > 0);

	int i, j;
	int **e = malloc(nV * sizeof(int*));	// Allocate "column" (ptr to array)
	assert(e != NULL);
	for (i = 0; i < nV; i++) {
		e[i] = malloc(nV * sizeof(int));	// Allocate "rows"
		assert(e[i] != NULL);
		for (j = 0; j < nV; j++)			// Init array values = 0
			e[i][j] = 0;
	}
	Graph g = malloc(sizeof(GraphRep));		// Allocate struct
	assert (g != NULL);
	g->nV = nV;								// Init vertices
	g->nE = 0;								// Init # edges
	g->edges = e;							// Reference to ptr to first row of edges array

	return g 		// return struct ptr
}

// #####################
// Validity Checking Fns
// #####################

// Checking valid graph
int validG(Graph g) {
	return (g != NULL && g->nV > 0 && g->nE >= 0);
}

// Checking valid vertex
int validV(Graph g, Vertex v) {
	return (validG(g) && 0 <= v && v < g->nV);
}





