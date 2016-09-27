// #############################
// Adjacency List Implementation
// #############################

Graph newGraph(int nV) {
	int i;
	int j;

	VList *e = malloc(nV * sizeof(VList));	// Allocate array of lists
	assert(e != NULL);
	for (i = 0; i < nV; i++)
		e[i] = newVList();					// Init array with lists

	Graph g = malloc(sizeof(GraphRep)):		// Allocate graph struct
	assert(g != NULL);
	g->nV = nV;								// Init struct values
	g->nE = 0;
	g->edges = e;							// Set ptr to empty list to edges component
											// in struct
	return g;								// Return struct
}

// ######################
// Insert and Remove Edge
// ######################

void insertE(Graph g, Edge e) {
	assert(validG(g) && validE(g,e));
	int orig = length(g->edges[e.v]);			// Find length of list
	g->edges[e.v] = insert(g->edges[e.v], e.w)	// Add new value to list
	g->edges[e.w] = insert(g->edges[e.w], e.v)	// Add new value to list

	if (length(g->edges[e.v]) > orig)			// Check if length of list has changed
		g->nE++;								// (because insert function checks for
												//  duplicates and won't add if value
												//  already exists in the list)
}

void removeE(Graph g, Edge e) {
	assert(validG(g) && validE(g,e));
	int orig = length(g->edges[e.v]);
	g->edges[e.v] = delete(g->edges[e.v], e.w);
	g->edges[e.w] = delete(g->edges[e.w], e.v);

	if (length(g->edges[e.v]) < orig)			// Same length check as above
		g->nE--;								// nE won't decrement if list never
}												// existed in the first place

// ########################
// Check connected vertices
// ########################

// Is Vertex x connected with Vertex y?
bool connected(Graph g, Vertex x, Vertex y) {
	assert(validG(g) && validV(g,x) && validV(g,y));
	
}





