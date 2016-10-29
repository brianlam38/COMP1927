#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct GraphRep *Graph;

typedef struct GraphRep {
	int nV;		 // # vertices
	int nE;		 // # edges
	int **edges; // edges matrix
} GraphRep;

Graph newGraph(int nV);
void disposeGraph(Graph g);

int main(int argc, char *argv[]) {

	printf("Creating new graph...\n");
	Graph g = newGraph(20);
	printf("Graph complete!\n\n");

	printf("Disposing graph...\n\n");
	disposeGraph(g);
	printf("Graph diposed!\n\n");

	return EXIT_SUCCESS;
}

Graph newGraph(int nV) {
	Graph g = malloc(sizeof(struct GraphRep));
	assert(g != NULL);

	int **e = malloc(nV * sizeof(int*));
	assert(e != NULL);
	int i, j;
	for (i = 0; i < nV; i++) {
		e[i] = malloc(nV * sizeof(int));
		assert(e[i] != NULL);
		for (j = 0; j < nV; j++) {
			e[i][j] = 0;
		}
	}

	g->nV = nV;
	g->nE = 0;
	g->edges = e;

	return g;
}

void disposeGraph(Graph g) {
	int i;
	for (i = 0; i < g->nV; i++) {
		free(g->edges[i]);
	}
	free(g->edges);
	free(g);
}






