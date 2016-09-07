// Usage: isIt = connected(g, x, y)
// Pre:  valid(g) and valid(x) && valid(y)
// Post: edge (x,y) exists in g->edges


// Array-of-edges representation

bool connected(Graph g, Vertex x, Vertex y)
{
	assert(validG(g) && validV(g,x) && validV(g,y));
	Edge e = mkEdge(x,y);
	for (i = 0; i < g->nE; i++)
		if (eqE(g->edges[i],e)) break;
	return (i < g->nE);
}


// Adjacency matrix representation

bool connected(Graph g, Vertex x, Vertex y)
{
	assert(validG(g) && validV(g,x) && validV(g,y));
	return g->edges[x][y];
}


// Adjacency list representation

bool connected(Graph g, Vertex x, Vertex y)
{
	assert(validG(g) && validV(g,x) && validV(g,y));
	VNode * cur;
	for (cur = g->edges[x]; cur != NULL; cur = cur->next) {
		if (cur->v == y) return 1;
	}
	return 0;
}
