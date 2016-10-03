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

int ncounted;		// # vertices allocated
int *componentOf;	// array of component ids
					// indexed by vertex 0..V-1

void components(Graph g) {
	int i;
	int componentID = 1;
	componentOf = calloc(nV(g), sizeof(int));
	ncounted = 0;
	while (ncounted < nV(g)) {
		Vertex v;
		for (v = 0; v < nV(g); v++)
			if (componentOf[v] == -1)	// If not a component
				break;
		dfsR(g, v, componentID);
		componentID++;
	}
	// componentOf[0..nV-1] is now set
}



