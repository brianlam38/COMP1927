// #############################
// Adjacency List Implementation
// #############################

Graph newGraph(int nV) {
	int i;
	int j;

	VList *e = malloc(nV * sizeof(VList));	// Alloc ptr to array of L's
	assert(e != NULL);
	for (i = 0; i < nV; i++)
		e[i] = newVList();					// Init array with L's

	Graph g = malloc(sizeof(GraphRep)):		// Alloc graph struct
	assert(g != NULL);
	g->nV = nV;
	g->nE = 0;
	g->edges = e;

	return g;
}

// ######################
// Insert and Remove Edge
// ######################

void insertE(Graph g, Edge e) {
	assert(validG(g) && validE(g,e));
	int orig = length(g->edges[e.v]);			// Store orig list len
	g->edges[e.v] = insert(g->edges[e.v], e.w)	// Add new value to list
	g->edges[e.w] = insert(g->edges[e.w], e.v)	// Add new value to list

	if (length(g->edges[e.v]) > orig)			// Check if nE changed
		g->nE++;								
}

void removeE(Graph g, Edge e) {
	assert(validG(g) && validE(g,e));
	int orig = length(g->edges[e.v]);
	g->edges[e.v] = delete(g->edges[e.v], e.w);
	g->edges[e.w] = delete(g->edges[e.w], e.v);

	if (length(g->edges[e.v]) < orig)
		g->nE--;
}

// ########################
// Check connected vertices
// ########################

// Is Vertex x connected with Vertex y?
bool connected(Graph g, Vertex x, Vertex y) {
	assert(validG(g) && validV(g,x) && validV(g,y));
	VList L = g->edges[x];	// Store list of neighbours of x
	return find(L,y);		// Scan to see if Y occurs in list
}
// OR FOR LINKED LIST
bool connected(Graph g, Vertex x, Vertex y) {
	assert(validG(g) && validV(g,x)
		   && validV(g,y));
	VNode * curr;
	for (curr = g->edges[x]; curr != NULL; curr = curr->next) {
		if (curr->v == y)
			return 1;
	}
	return 0;
}

// ########################
// Check neighbour vertices
// ########################

// What are the neighbours of vertex x?
Vertex *neighbours(Graph g, Vertex x, int *nv) {
	assert(validG(g) && validV(g,x) && validV(g,y));
	VList L = g->edges[x];						// init list to check
	int nn = length(L);							// count #neighbours
	Vertex *ns = malloc(nn * sizeof(Vertex));	// alloc array neighbours
	assert(ns != NULL);
	
	int k = 0;
	VList * curr;
	for (curr = L->first; curr != NULL; curr = curr->next) { // Scan through list, store value
		ns[k++] = curr->value;								 // store value of neighbour
	}														 // into neighbours array
	*nv = nn;		// set *nv (ptr to integer varable) = #neighbours
	return ns;		// return ptr to neighbours array
}





