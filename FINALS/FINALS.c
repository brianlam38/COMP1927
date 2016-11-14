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

// BFS: Adj Matrix Rep
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

// DFS Recursive: Modified for Connected Components
void dfs(Graph g) {
	int i;
	visited = calloc(nV(g), sizeof(int));
	order = 1;
	while (order < nV(g)) {				// When order = nV, we have visited all vertices
		Vertex v;
		for (v = 0; v < nV(g); v++)		// Check all vertices to see if they are visited
			if (visited[v] == 0)		// If not, do a recursive DFS
				break;
		dfsR(g,v);
	}
}
void dfsR(Graph g, Vertex x) {
	visited[v] = order++;				// Mark curr v as visited / the order visited			
	Vertex w;
	for (w = 0; w < nV(g); w++) {		// Iterate through possible neighbours W
		if (!hasEdge(g,v,w))				// For each W, check if there is an edge btwn
			continue;						// vertex V and W. Continue if no edge exists
		if (!visited[w])				// If W has not been visited, recursively visit W
			dfsR(g,w);						// Recursion will iterate through all possible	
	}										// neighbours Y. Once all neighours Y are visited
}											// then recursion will back-track one level

// DJIKSTRA'S ALGORITHM
oid shortestPath(Graph g, Vertex start,
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
	dispose(PQ);
}

/* Possibly put notes for these aswell? */
// 1. Minimum Spanning Trees -> A subset of edges that connects all vertices together using the
//                              minimum possible weight.

// 2. Cycle Checking -> Does a cycle exist in the graph?

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

