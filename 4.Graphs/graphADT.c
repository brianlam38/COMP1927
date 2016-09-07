// Graph ADT functions


// graph representation is hidden
typedef struct GraphRep *Graph;			// ptr to an unknown data structure
// vertices denoted by integers 0..N-1
typedef int Vertex;						// valid values of vertex
// validity check
int validV(Graph, Vertex);				// checking if vertex is part of the graph
// edges are pairs of vertices (end-points)
typedef struct { Vertex v; Vertex W; } Edge; // setting up edge data structure
// making an edge
Edge mkEdge(Graph, Vertex, Vertex);
// operations on graph
Graph newGraph(int nV);				// nV = # vertices in the graph
// insert edge
void insertE(Graph, Edge);
// remove edge
void removeE(Graph, Edge);
// copy graph
Graph copy(Graph);
// remove graph
void destroy(Graph);
// display graph
void show(Graph);

// graph representation has to include V and E
typedef struct GraphRep {
	int nV;		// # vertices
	int nE;		// # edges
	// representation for vertices
	// e.g. array of [0 ... nV-1] of Items
	//
	// representation for edges
	// some possibilities to be discussed

} GraphRep;

// VERSION 1
// make an edge for a given Graph
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(validV(g,v) && validV(g,w));		// check validity of creating edge	
	Edge e = {v,w};							// assigning a struct to object e
	// or Edge e; e.v = v; e.w = w
	return e;								// return struct object e
}

// checking if vertex is valid in a given Graph
static int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
	// Is G a valid graph?
	// Is number of vertices < max number?
}

// Checking if 2 edges are the same thing
bool eqE(Edge a, Edge b) 
{
	return (a.v == b.v && a.w == b.w)   // is 2->3 same as 3->2
			|| (a.v=b.w && a.w == b.v);	// is 3->2 same as 2->3
}

// VERSION 2: CANONICAL EDGE REPRESENTATION (First vertex < Second vertex)
// Same check, except we make sure to always put lowest value vertix FIRST
// This makes mkEdge function more complex, as we need to swap V/W if they're in wrong order
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(valid(...));
	Edge e;
	if (v < w)		// guarantees 1st vertex in pair < 2nd vertex in pair
		e = {v,w};	// they are not going to be equal, as vertex can't connect with itself
	else
		e = {w,v};
	return e;
}

bool eqE(Edge a, Edge b) 
{
	return (a.v == b.v && a.w == b.w);
}

// implementation of graph initialisation
Graph newGraph(int nV)
{
	assert(nV > 0); 						// at least have 1 vertex
	int i, j;
	int **e = malloc(nV * sizeof(int *));	// create vertical array
	assert(e != NULL);						// make sure ptr is not empty
	for (i = 0; i < nV; i++) {
		e[i] malloc(nV * sizeof(int));		// creating each row
		assert(e[i] != NULL);				// each rows is an array of nV int values
		for (j = 0; j < nV; j++)
			e[i][j] = 0;					// initialise all to 0
	}
	Graph g = malloc(sizeof(GraphRep));		// create space for graph
	assert(g != NULL);						// check graph has something
	g->nV = nV; g->nE = 0; g->edges = 0;	// initialise graph data
	return g;								// return ptr g to the graph struct
}

// VALIDITY CHECKING
// Checking if the graph is not empty, has at least 1 vertex, has at least 0 edges
int validG(Graph g) {
	return (g != NULL && g->nV > 0 && g->nE >= 0);
}
// Check if valid vertex
int validV(Graph g, Vertex v) {
	return (validG(g) && 0 <= v && v < g->nV);
}
// Check if valid edge
int validE(Graph g, Edge e) {
	return (validV(g,e.v) && validV(g,e.w));
}

// EDGE INSERTION
void insertE(Graph g, Edge e)
{
	assert(validG(g) && validE(g,e));	// check valid graph and valid edge insertion
	if (g->edges[e.v][e.w]) return;		// check if edge already exists
	g->edges[e.v][e.w] = 1;				// set element in matrix to 1
	g->edges[e.w][e.v] = 1;				// set elememt in matrix to 1
	g->nE++;							// increment # edges
}
// EDGE REMOVAL
void removeE(Graph g, Edge e)
{
	assert(validG(g) && validE(g,e));	// check if graph and edge is valid
	if (!g->edges[e.v][e.w]) return;	// check if edge exists
	g->edges[e.v][e.w] = 0;				// set elt in matrix to 0
	g->edges[e.w][e.v] = 0;				// set elt in matrix to 0
	g->nE--;							// decrement # edges
}






