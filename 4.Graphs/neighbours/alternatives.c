// Usage: vs = neighbours(g, x, &n)
// Pre:  valid(g) and valid(x) && isdefined(n)
// Post: vs[0..n-1] contains neighbours of x


// Array-of-edges representation

Vertex *neighbours(Graph g, Vertex x, int *nv)
{
	int i;
	assert(validG(g) && validV(g,x) && nv != NULL);
	Set neighbours = newSet();
	for (i = 0; i < g->nE; i++) {
		if (g->edges[i].v == x)
			insert(neighbours, g->edge[i].w);
		else if (g->edges[i].w == x)
			insert(neighbours, g->edge[i].v);
	}
	int nneighbours = card(neighbours);
	Vertex *narray = malloc(nneighbours * sizeof(Vertex));
	assert(narray != NULL);
	int j = 0;
	for (i = 0; i < g->nV; i++) {
		if (isElem(neighbours,i))
			narray[j++] = i;
	}
	*nv = nneighbours;
	return narray;
}


// Adjacency matrix representation

Vertex *neighbours(Graph g, Vertex x, int *nv)
{
	assert(validG(g) && validV(g,x));
	int nneighbours = 0;
	for (i = 0; i < g->nV; i++)
		nneighbours += g->edges[x][i];
	Vertex *narray = malloc(nneighbours * sizeof(Vertex));
	assert(narray != NULL);
	int j = 0;
	for (i = 0; i < g->nV; i++) {
		if (g->edges[x][i])
			narray[j++] = i;
	}
	*nv = nneighbours;
	return narray;
}



// Adjacency list representation
// - assume that lengthVList(L) exists

Vertex *neighbours(Graph g, Vertex v, int *nv)
{
	assert(validG(g) && validV(g,v));
	int nneighbours = lengthVList(g->edges[v]);
	Vertex *narray = malloc(nneighbours * sizeof(Vertex));
	assert(narray != NULL);
	int j = 0;  VNode * cur;
	for (cur = g->edges[v]; cur != NULL; cur = cur->next) {
		narray[j++] = cur->v;
	}
	*nv = nneighbours;
	return narray;
}
