// all edges weights are all positive
#define NO_EDGE -1.0		// -ve floating pt value

// assume non-directional
typedef struct GraphRep {
	int nV;
	int nE;
	float **edges;			// * to an array, each elt in array is ptr to another array
} GraphRep;					// floating pt values rather than ints

typedef GraphRep *Graph;

float cost(Graph g) {
	int v, w;
	float totalCost = 0.0;
	for (v = 0; v < g->nV; v++) {
		for (w = v+1; w < g-nV; w++) {	// iterate over edges that are connected to v
			if (g->edges[v][w] == NO_EDGE) continue;
			cost += g->edges[v][w];
		}
	}
	return totalCost;
}