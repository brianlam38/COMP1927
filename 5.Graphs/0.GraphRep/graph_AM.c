// Graph ADT Implementations

// ##################
// Initialising Graph
// ##################

Graph newGraph(int nV) {
	assert(nV > 0);

	int **e = malloc(nV * sizeof(int*));	// #1 Allocate "Columns"
	assert(e != NULL);						//    (ptr to array of ptrs)

	int i, j;								// #2 Allocate "Rows" arrays
	for (i = 0; i < nV; i++) {				//    Set values in arrays = 0;
		e[i] = malloc(nV * sizeof(int));
		assert(e[i] != NULL);
		for (j = 0; j < nV; j++)
			e[i][j] = 0;
	}
	Graph g = malloc(sizeof(GraphRep));		// #3 Allocate GraphRep struct
	assert (g != NULL);						//    Set up struct values
	g->nV = nV;								//    Set up ptr to edges
	g->nE = 0;
	g->edges = e;

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

// Creates an array of vertices that are neighbours
Vertex *neighbours(Graph g, Vertex x, int *nv) {
	assert(validG(g) && validV(g,x) && nv != NULL);

	int nn = 0;						// #1 Set #neigbours = 0

	int i; 							// #2 Count #neighbours for vertex Xs
	for (i = 0; i < g->nV; i++) {
		if (g->edges[x][i])
			nn++;
	}

	int *ns = malloc(nn * sizeof(Vertex));	// #3 Alloc NS array, where size = nn
	assert(ns != NULL);

	int k = 0;								// #4 Copy neighbour v's index
	for (i = 0; i < g->nV; i++) {			//    values into NS array
		if (g->edges[x][i])
			ns[k++] = i;
	}

	*nv = nn;	// change value from *nv = nn
	return ns; 	// return ptr to NS array
}











