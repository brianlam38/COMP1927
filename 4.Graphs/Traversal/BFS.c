// ####################
// BREADTH-FIRST SEARCH
// ####################

int *visited;	// array [0..V-1] of visiting order

void bfs(Graph g, Vertex v) {
	int i;
	int order = 1;
	visited = calloc(nV(g), sizeof(int));	// allocate visited array + set to 0
	Queue q = newQueue();					// create queue
	QueueJoin(q,v);							// add source to queue
	while (!QueueIsEmpty(g)) {				// while queue is not empty
		Vertex y;
		Vertex x = QueueLeave(q);			// pop 1st elt off head of queue
		if (visited[x])						// checked if visited
			continue;
		visited[x] = order++;				// if not visited, mark as visited
		for (y = 0; y < nV(g); y++) {		// look at all nieghbours (ascending order)
			if (!hasEdge(g,x,y))				// check if neighbour
				continue;
			if (!visited[y])					// check if visited
				QueueJoin(q,y);
		}
	}
}


// ######################
// GRAPH ADJACENCY MATRIX
// ######################

Graph newGraph(int nV) {
	int i, j;
	Edge e** = malloc(nV * sizeof(int*));
	assert(e != NULL);

	for (i = 0; i < nV; i++) {
		e[i] = malloc(nV * sizeof(int));
		assert(e[i] != NULL);
		for (j = 0; j < nV; j++) {
			e[i][j] = 0;
		}
	}
	Graph g = malloc(sizeof(GraphRep));
	assert(g != NULL);
	int nE;
	g->nV = nV;
	g->nE = 0;
	g->edges = e;

	return g;
}








