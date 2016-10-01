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

// ###################################
// BREADTH-FIRST SEARCH: PATH CHECKING
// ###################################

// Is there a path from vertex source --> destination?
int hasPath(Graph g, Vertex src, Vertex dest) {
	int *visited = calloc(nV(g) * sizeof(int));
	Queue q = newQueue();
	QueueJoin(q,src);
	// Mark source as visited
	int isFound = 0;

	while (!QueueIsEmpty(q) && !isFound) {
		Vertex y;
		Vertex x = QueueLeave(q);
		for (y = 0; y < nV(g); y++) {
			if (!hasEdge(g,x,y))
				continue;
			if (y == dest)
				isFound = 1;
				break;
			if (!visited[y])
				QueueJoin(q,y);
				visited[y] = 1;
		}
	}
	free(visited);
	return isFound;
}

// ###########################################
// BREADTH-FIRST SEARCH: FINDING SHORTEST PATH
// ###########################################

// What is the shortest path from vertex source --> Destination?
// Recording sequence of vertices from src --> dest
int findPath(Graph g, Vertex src, Vertex dest) {
	Vertex *path = malloc(nV(g) * sizeof(Vertex));
	int *visited = calloc(nV(g), sizeof(int));
	Queue q = newQueue();
	QueueJoin(q,src);
	int isFound = 0;

	while (!emptyQ(q) && !isFound) {
		Vertex y;
		Vertex x = QueueLeave(q);
		for (y = 0; y < nV(g); y++) {
			if (!hasEdge(g,x,y))
				continue;
			path[y] = x;		// Remembering value of prev vertex
			if (y == dest)
				isFound = 1;
				break;
			if (!visited[y])
				QueueJoin(q,y);
				visited[y] = 1;
		}
	}
	if (isFound) {
		Vertex v;
		for (v = dest; v != src; v = path[v])	// Display path from dest -> src (reverse)
			printf("%d-",v);
		printf("%d\n",src);
	}
	free(visited);
	free(path);
	return isFound;		// returns if dest = found AND records path
}





