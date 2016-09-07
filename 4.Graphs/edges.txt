bool eqE(Edge a, Edge b) 
{
	return (a.v == b.v && a.w == b.w)
			|| (a.v=b.w && a.w == b.v);
}

// if guarantee v<w

Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(valid(...));
	Edge e;
	if (v < w)
		e = {v,w};
	else
		e = {w,v};
	return e;
}

bool eqE(Edge a, Edge b) 
{
	return (a.v == b.v && a.w == b.w);
}

int cmpE(Edge a, Edge b)
{
	if (a.v < b.v)
		return -1;
	else if (a.v > b.v)
		return 1;
	else if (a.w < b.w)
		return -1;
	else
		return 1;
}
