// Space efficient representation for adjacency lists ...

Edge normalise(Edge e)
{
	if (e.v > e.w)
		{ Vertex tmp = e.v;  e.v = e.w;  e.w = tmp; }
	return e;
}

void insertE(Graph g, Edge e)
{
	assert(validG(g) && validE(g,e));
	e = normalise(e);
	int orig = length(g->edges[e.v]);
	g->edges[e.v] = insert(g->edges[e.v], e.w);
	if (length(g->edges[e.v]) > orig) g->nE++;
}
void removeE(Graph g, Edge e)
{
	assert(validG(g) && validE(g,e));
	e = normalise(e);
	int orig = length(g->edges[e.v]);
	g->edges[e.v] = delete(g->edges[e.v], e.w);
	if (length(g->edges[e.v]) < orig) g->nE--;
}
