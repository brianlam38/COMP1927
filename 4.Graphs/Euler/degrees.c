// ###############
// DEGREE CHECKING
// ###############

// Given a vertex v, return number of degrees
int degree (Graph g, Vertex v) {
	int n, i;
	for (i = 0; i < g->nV; i++) {
		if (hasEdge(g,i))
			n++;
	}
	return n;
}

// Compute degree of each vertex and store in ds[]
// ds[] = lookup table
void degrees(Graph g, int ds[]) {
	assert (g != NULL);
#ifdef ADJ_MATRIX
	for (i = 0; i < g->nV; i++) {	  // set up degrees lookup index
		ds[i] = 0;
		int j;
		for (j = 0; j < g->nV; j++) { // count #degrees per vertex
			if (g->edges[i][j] != 0)
				ds[i]++;
		}
	}
#else
	// adj list
	ds[i] = ListLength(g->lists[i]);   // count linked list
#endif
}

int hasEulerPath(Graph g, Vertex v, Vertex w) {
	int degreeOf = calloc(nV(g), sizeof(int));
	degrees(g, degreeOf);
	int t = degreeOf[v] + degreeOf[w];
	if ((t % 2) != 0)						// check if v,w are odd
		return 0;
	Vertex x;
	for (x = 0; x < nV(g); x++) {			// check if remaining are even
		if (x != v && x != w) {
			if ((degreeOf[x] % 2) != 0)
				return 0;
		}
	}
}