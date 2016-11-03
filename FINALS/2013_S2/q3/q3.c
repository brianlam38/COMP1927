// q3.c ... Question 3 in COMP1927 13s1 Final Exam
// Client program for Lists ... count connected components in a graph


#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"

int main(int argc, char *argv[])
{
	Graph G;

	G = readGraph(stdin);
	if (G == NULL)
		printf("Invalid graph\n");
	else {
		printf("Graph:\n"); showGraph(G); printf("\n");
		printf("# connected components = %d\n", nComponents(G));
	}

	return 0;
}
