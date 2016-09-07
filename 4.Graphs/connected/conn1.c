// adjacency matrix representation
typedef struct GraphRep {
	int   nV;    // #vertices
	int   nE;    // #edges
	int **edges; // matrix of booleans
} GraphRep;

// Usage: isIt = connected(g, x, y)
// Pre:  valid(g) and valid(x) && valid(y)
// Post: res = True,  if edge (x,y) exists in g->edges
//       res = False, otherwise

bool connected(Graph g, Vertex x, Vertex y)
{
	assert(validG(g) && validV(g,x) && validV(g,y));
	return g->edges[x][y];
}

