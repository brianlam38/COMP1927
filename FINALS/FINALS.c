// ###################
//
// ###################

// #################
// R E C U R S I O N
// #################

// Recursion to find max val in a[lo..hi]
int max(int a[], int lo, int hi)
{
	if (lo == hi) {					// #1 Max value found
		return a[lo];
	} else {
		if (a[lo] < a[hi])			// #2 LO < HI
			return max(a,lo+1,hi);		// increment lo++
		else 						// #3 LO > HI
			return max(a,lo,hi-1);		// decrement hi--
	}
}


// ###################
// G R A P H S
// ###################

// CONNECTED COMPONENTS: 

// NEIGHBOURS: Returns array of vertices that are neighbours of X
Vertex *neighbours(Graph g, Vertex x, int *nv)
{
	assert(validG(g) && validV(g,x) && nv != NULL);

	int i;
	int nn = 0;
	for (i = 0; i < g->nV; i++) {		   // #1 Find num neighbours
		if (g->edges[x][i])
			n++;
	}
	int *ns = malloc(nn * sizeof(Vertex)); // #2 Alloc neighbours array
	assert(ns != NULL);

	int k;
	for (i = 0; i < g->nV; i++) {		   // #3 Copies neighbour indexes
		if (g->edges[x][i])				   //    into neighbours array
			ns[k++] = i;
	}
}

// BREADTH-FIRST-SEARCH: Adj Matrix
int *visited;	// array [0..V-1] of visiting order

void bfs(Graph g, Vertex v) {
	int i;
	int order = 1;
	visited = calloc(nV(g), sizeof(int));	// allocate visited array + set to 0
	Queue q = newQueue();					// create queue
	QueueJoin(q,v);							// add source to queue
	visited[v] = order++;					// mark source as visited
	while (!QueueIsEmpty(g)) {
		Vertex y;							// declare neighbour vertex
		Vertex curr = QueueLeave(q);			// pop vertex off queue
		if (visited[curr])						// check if vertex is visited
			continue;
		for (y = 0; y < nV(g); y++) {		// look at neighbours (ascending order)
			if (!hasEdge(g,curr,y))				// check if connected
				continue;
			if (!visited[y])					// check if neighbour is visited
				QueueJoin(q,y);					// add neighbour to queue
				visited[curr] = order++;			// mark curr vertice as visited
		}
	}
}

// ###################
// B S T r e e s
// ###################

// HIGHER ORDER FN: Passing & applying a fn to all tree nodes (recursive)
void map(BSTree t, int(*f)(int))
{
	if (t != NULL) {			// While not empty
		t->value = f(t->value); // Apply function to val
		map(t->left,f);			// Recursive on LHS
		map(t->right,f);		// Recursive on RHS
	}
}

