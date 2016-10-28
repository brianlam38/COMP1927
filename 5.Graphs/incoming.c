// example usage:
// Vertex *incoming; int nIncoming;
// ...
// incoming = inBound(g, v, &nIncoming)

// adjacency matrix
Vertex *inBound(Graph g, Vertex v, int *nInBound)
{
	int i,w,n = 0;
	for (w = 0; w < g->nV; w++)
		if (g->edges[w][v]) n++;
	Vertex *vs = malloc(n*sizeof(Vertex));
	for (w = 0, i = 0; w < g->nV; w++)
		if (g->edges[w][v]) vs[i++] = w;
	&nInBound = n;
	return vs;
}

// adjacency list
Vertex *inBound(Graph g, Vertex v, int *nInBound)
{
	int i,w,n = 0;
	for (w = 0; w < g->nV; w++)
		// scan list looking for v; n++ if found
	Vertex *vs = malloc(n*sizeof(Vertex));
	for (w = 0, i = 0; w < g->nV; w++)
		// scan list looking for v; vs[i++] = w if found
	&nInBound = n;
	return vs;
}