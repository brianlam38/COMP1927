// ####################
// CONNECTED COMPONENTS
// ####################

// Add two extra items to GraphRep struct
struct GraphRep {
	...
	int nC;		// # connected components
	int *cc;	// Connected components array
}				// Index = vertex no. Element = component no.

// How many connected subgraphs are there?
int nComponents(Graph g) {
	return g->nC;
}

// Are two vertices in the same connected subgraph?
int sameComponent(Graph g, Vertex v, Vertex w) {
	return (g->cc[v] == g->cc[w]);
}

// ############################
// IMPLEMENTING COMPONENT ARRAY
// ############################



