// #############
// DFS Recursive
// #############

// Global variables
int order;		// visiting order
int *visited;	// array of visiting orders
				// indexed by Vertex 0..V-1

// Wrapper function: Set up initial state to call recursive function
void dfs(Graph g) {
	int i;
	visited = calloc(nV(g), sizeof(int)); 	// Allocate array + set elts to 0
	order = 1;								// init order = 1 (first thing visit = 1)
	dfsR(g, 0);								// depth-first reversal
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

// ############
// MODIFIED DFS
// ############

// Modified dfs to handle non-connected subgraphs
// Difference: Wrapper function checks all vertices to see
// if they have been visited. If not, perform dsfR.
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




