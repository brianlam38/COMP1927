#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// #####################################
// REACHABILITY MATRIX - WARSHALL'S ALGO
// #####################################

void showMatrix(int **m, int v);

int main(int argc, char *argv[])
{
	int nV, i, v, w, s, t;
	int **edges;	// array of edges
	int **tc;		// reachilibity

	if (argc < 2 || sscanf(argv[1],"%d",&nV) != 1) {
		fprintf(stderr,"Usage: %s nV\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	edges = malloc(nV * sizeof(int*));	// allocate array of ptrs to arrays
	assert(edges != NULL);
	tc = malloc(nV * sizeof(int*));
	assert(tc != NULL);

	for (i = 0; i < nV; i++) {			// allocate arrays
		edges[i] = calloc(nV,sizeof(int));	// initialise values = 0
		tc[i] = calloc(nV,sizeof(int));
	}
	// Create random matrix, only for display purposes
	for (v = 0; v < nV; v++) {			// initialise randomly
		for (w = 0; w < nV; w++) {
			if (rand()%10 < 4) {
				edges[v][w] = 1;
				tc[v][w] = 1;
			}
		}
	}
	printf("Initial Matrix\n");
	showMatrix(tc,nV);

	// Warshall's Algorithm
	for (v = 0; v < nV; v++) {				// Initial vertex loop
		for (s = 0; s < nV; s++) {			// Intermediate vertex loop
			if (tc[s][v] == 0) continue;		// Continue if no link between init/inter
			for (t = 0; t < nV; t++) {		// Destination vertex loop
				if (tc[v][t]) tc[s][t] = 1;	// Mark inter/dest reachable
			}
		}
	}
	printf("After iteration %d\n", v+1);
	showMatrix(tc,nV);

	return EXIT_SUCCESS;
}

// A matrix print function
void showMatrix(int **m, int nV)
{
	int i, j;
	printf("    ");
	for (i = 0; i < nV; i++)
		printf(" [%2d]",i);
	printf("\n");
	for (i = 0; i < nV; i++) {
		printf("[%2d]",i);
		for (j = 0; j < nV; j++)
			printf("  %2d ", m[i][j]);
		printf("\n");
	}
}

