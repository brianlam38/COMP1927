// adjacency list representation
typedef struct vNode *VList;
struct vNode { Vertex v; vList next; };
typedef struct graphRep GraphRep;
struct graphRep {
	int   nV;     // #vertices
	int   nE;     // #edges
	VList *edges; // array of lists
};

// Usage: isIt = connected(g, x, y)
// Pre:  valid(g) and valid(x) && valid(y)
// Post: res = True,  if edge (x,y) exists in g->edges
//       res = False, otherwise

bool connected(Graph g, Vertex x, Vertex y)
{
	assert(validG(g) && validV(g,x) && validV(g,y));
	VList L = g->edges[x];
	return find(L,y);
}
