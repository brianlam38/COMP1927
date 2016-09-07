// adjacency list representation
typedef struct vNode *VList;
struct vNode { Vertex v; vList next; };
typedef struct graphRep GraphRep;
struct graphRep {
	int   nV;     // #vertices
	int   nE;     // #edges
	VList *edges; // array of lists
};

// Usage: vs = neighbours(g, x, &n)
// Pre:  valid(g) and valid(x) && isdefined(n)
// Post: vs[0..n-1] contains neighbours of x

Vertex *neighbours(Graph g, Vertex x, int *nv)
{
	assert(validG(g) && validV(g,x) && nv != NULL);
	// TODO
}
