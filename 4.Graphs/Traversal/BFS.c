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
	visited[x] = order++;					// mark source as visited
	while (!QueueIsEmpty(g)) {
		Vertex y;							// declare neighbour vertex
		Vertex x = QueueLeave(q);			// pop vertex off queue
		if (visited[x])						// check if vertex is visited
			continue;
		for (y = 0; y < nV(g); y++) {		// look at neighbours (ascending order)
			if (!hasEdge(g,x,y))				// check if connected
				continue;
			if (!visited[y])					// check if neighbour is visited
				QueueJoin(q,y);					// add neighbour to queue
				visited[x] = order++;			// mark vertice as visited
		}
	}
}