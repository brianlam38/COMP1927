// adjacency matrix representation
typedef struct GraphRep {
	int   nV;    // #vertices
	int   nE;    // #edges
	int **edges; // matrix of booleans
} GraphRep;

// Usage: vs = neighbours(g, x, &n)
// Pre:  valid(g) and valid(x) && isdefined(n)
// Post: vs[0..n-1] contains neighbours of x

Vertex *neighbours(Graph g, Vertex x, int *nv)
{
	assert(validG(g) && validV(g,x) && nv != NULL);	// check graph + vertex + ptr is valid
	int i, nn = 0;							// initialise nn = 0 neighbours
	for (i = 0; i < g->nV; i++)				// go to row corresp to X, scan along row then count # of 1's
		if (g->edges[x][i]) nn++;			
	int *ns = malloc(nn*sizeof(Vertex));	// allocate array of neighbours
	assert(ns != NULL);
	// fill array with actual vertex numbers
	int k = 0;					
	for (i = 0; i < g->nV; i++)				// go through rows again
		if (g->edges[x][i]) ns[k++] = i;	// if vertex i is a neighbour of x, then we want to add vertex i to output array
	*nv = nn;								// set up return value for # neighbours
	return ns;								// return ptr to start of the array
}


