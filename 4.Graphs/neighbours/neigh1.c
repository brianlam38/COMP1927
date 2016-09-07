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
	assert(validG(g) && validV(g,x) && nv != NULL);
	int i, nn = 0;
	for (i = 0; i < g->nV; i++)
		if (g->edges[x][i]) nn++;
	int *ns = malloc(nn*sizeof(Vertex));
	assert(ns != NULL);
	int k = 0;
	for (i = 0; i < g->nV; i++)
		if (g->edges[x][i]) ns[k++] = i;
	*nv = nn;
	return ns;
}
