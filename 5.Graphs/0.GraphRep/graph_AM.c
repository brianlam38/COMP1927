// Graph ADT Implementations

// ##################
// Initialising Graph
// ##################

Graph newGraph(int nV) {
	assert(nV > 0);

	int i, j;
	int **e = malloc(nV * sizeof(int*));	// Alloc "column" 
	assert(e != NULL);						// (ptr to array of ptrs)
	for (i = 0; i < nV; i++) {
		e[i] = malloc(nV * sizeof(int));	// Allocate "rows"
		assert(e[i] != NULL);
		for (j = 0; j < nV; j++)			// Init array values = 0
			e[i][j] = 0;
	}
	Graph g = malloc(sizeof(GraphRep));		// Alloc struct
	assert (g != NULL);
	g->nV = nV;								// Init vertices
	g->nE = 0;								// Init # edges
	g->edges = e;							// Set edges = ptr e

	return g;
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
	return (validG(g) && v > 0 && v < g->nV);
}

// Checking valid edge is implicit
// 1/0 in matrix to represent edges
int validE(Graph g, Edge e) {
	return (validV(g,e.v) && validV(g,e.w));
}

// ########################
// Edge insertion / removal
// ########################

// Inserting new edge
// This modifies the matrix
void insertE(Graph g, Edge e) {
	assert(validV(g) && validE(g));

	if (g->edges[e.v][e.w])	// If edge already exists, do nothing
		return;

	g->edges[e.v][e.w] = 1;	// Set to 1 in adj matrix
	g->edges[e.w][e.v] = 1; // Set to 1 in adj matrix
	g->nE++;				// Increment #edges
}

void removeE(Graph g, Edge e) {
	assert(validV(g) && validE(g));

	if (!g->edges[e.v][e.w]) // If edges doesn't exist, do nothing
		return;

	g->edges[e.v][e.w] = 0;	// Set to 0 in adj matrix
	g->edges[e.w][e.v] = 0; // Set to 0 in adj matrix
	g->nE--;				// Decrement #edges
}

// ########################
// Check connected vertices
// ########################

bool connected(Graph g, Vertex x, Vertex y) {
	assert(validG(g) && validV(g,x) && validV(g,y)); // Check if x y = 1
	return g->edges[x][y];	// Return if connected
							// Stating edges[x][y] or [y][x] doesn't
							// matter because matrix is symmetric
}

// ########################
// Check neighbour vertices
// ########################

// Check if two vertices are adjacent
// Returns an array of vertices (the ptr to start of it)
// with size = # neighbours
Vertex *neighbours(Graph g, Vertex x, int *nv) {
	assert(validG(g) && validV(g,x) && nv != NULL);

	// Go to row corresponding to x
	// Scan along row, count # of 1's
	int i
	int nn = 0;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[x][i])			// i'th position in row x
			nn++;					// count neighbours
	}
	int *ns = malloc(nn * sizeof(Vertex));	// allocate array of neighbours
	assert(ns != NULL);						// size = # neighbours
	int k = 0;
	for (i = 0; i < g->nV; i++) {			// init the array
		if (g->edges[x][i])					// if vertex i = neighour, it will = 1
			ns[k++] = i;					// add vertex i to index of neighbours
	}
	*nv = nn;	// set *nv (ptr to integer variable) = #neighbours
	return ns; 	// return ptr to neighbours array
}











