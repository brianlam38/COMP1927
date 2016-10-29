// #############################
// Adjacency List Representation
// #############################

typedef struct vNode *VList;
struct vNode {
	Vertex v;
	vList next;
};

typedef struct graphRep GraphRep;

struct graphRep {
   int   nV;     // #vertices
   int   nE;     // #edges
   VList *edges; // array of lists
};


// #############################
// Adjacency List Implementation
// #############################

Graph newGraph(int nV) {
	int i, j;

	VList *e = malloc(nV * sizeof(VList));	// #1 Allocate "Column" array
	assert(e != NULL);						//    (array of lists)

	for (i = 0; i < nV; i++)				// #2 Create "Rows"
		e[i] = newVList();					//    (linked lists)

	Graph g = malloc(sizeof(GraphRep)):		// #3 Allocate GraphRep struct
	assert(g != NULL);						//    + set values
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
	int orig = length(g->edges[e.v]);			// #1 Store original list len
	g->edges[e.v] = insert(g->edges[e.v], e.w)	// #2 Insert
	g->edges[e.w] = insert(g->edges[e.w], e.v)

	if (length(g->edges[e.v]) > orig)			// #3 Adjust #edges / nE
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

// VERSION 1: Store neighbours
bool connected(Graph g, Vertex x, Vertex y) {
	assert(validG(g) && validV(g,x) && validV(g,y));

	VList L = g->edges[x];	// Store list of neighbours of X
	return find(L,y);		// Scan list for Y
}

// VERSION 2: Traverse linked list for connected
bool connected(Graph g, Vertex x, Vertex y) {
	assert(validG(g) && validV(g,x) && validV(g,y));

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

// Creates an array of vertices that are neighbours of X
Vertex *neighbours(Graph g, Vertex x, int *nv) {
	assert(validG(g) && validV(g,x) && validV(g,y));

	VList L = g->edges[x];						// #1 Copy row to new list
	int nn = length(L);							//    + count nn's

	Vertex *ns = malloc(nn * sizeof(Vertex));	// #2 Allocate NS array
	assert(ns != NULL);
	
	int k = 0;									// #3 Scan through list, copy
	VList * curr;								//    v->value into NS array
	for (curr = L->first; curr != NULL; curr = curr->next) {
		ns[k++] = curr->value;
	}

	*nv = nn;	// change value from *nv = nn
	return ns; 	// return ptr to NS array
}





