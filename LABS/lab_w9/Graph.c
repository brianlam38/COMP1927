// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	int  **edges; // matrix of weights (0 == no edge)
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	Edge new = {v,w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w, int wt)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] == 0) {
		g->edges[v][w] = wt;
		g->edges[w][v] = wt;
		g->nE++;
	}
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	if (g->edges[v][w] != 0) {
		g->edges[v][w] = 0;
		g->edges[w][v] = 0;
		g->nE--;
	}
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	int v, w;
	Graph new = malloc(sizeof(GraphRep));
	assert(new != 0);
	new->nV = nV; new->nE = 0;
	new->edges = malloc(nV*sizeof(int *));
	assert(new->edges != 0);
	for (v = 0; v < nV; v++) {
		new->edges[v] = malloc(nV*sizeof(int));
		assert(new->edges[v] != 0);
		for (w = 0; w < nV; w++)
			new->edges[v][w] = 0;
	}
	return new;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	// not needed for this lab
}

// display graph, using names for vertices
void showGraph(Graph g, char **names)
{
	assert(g != NULL);
	printf("#vertices=%d, #edges=%d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d %s\n",v,names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf("\t%s (%d)\n",names[w],g->edges[v][w]);
			}
		}
		printf("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath(Graph g, Vertex src, Vertex dest, int max, int *path)
{
	// Fills in path array with a sequence of vertex numbers, giving the "shortest" path from src --> dest.
		// (no edge in path > MAX)
	// Returns number of vertices stored in path array.
		// No path = zero
		// Path array should have enough space to hold the longest possible path
		// (longest path may include all vertices)
	// Considerations:
		// Length of path = # of edges, NOT sum of edge weights
		// Shortest path = Least # of edges
		// MAX = Maximum KM of an edge.
		// Shorter total KM of path = less connected (less options to fly around)
		// Longer total KM of path = more connected (more options to fly around)

	assert(g != NULL);

	Queue q = newQueue();
	Vertex seen[g->nV];
	Vertex prev[g->nV];

	//Vertex *visited = calloc(g->nV,sizeof(Vertex));
	QueueJoin(q,src);
	seen[src] = 1;	
	prev[src] = -1;

	//BFS
	printf("Starting BFS\n");
	while (!QueueIsEmpty(q)) {
		Vertex curr = QueueLeave(q);	//remove the first element in queue and store it in curr
		if (curr == dest) {
			break;
		}
		//Find neighbours and add them to the queue
		printf("Finding neighbours\n");
		Vertex from = curr;
		for (Vertex to = 0; to < g->nV; to++) { 				//For each vertice in the graph	
			printf("Adding neighbours to queue\n");
			if (g->edges[from][to] < max && !g->edges[from][to] // adding neighbours to queue
				&& !seen[to]) {					
				seen[to] = 1;		//Mark neighbour as visited
				prev[to] = from; 	//Mark the previous of the neighbour as current
				QueueJoin(q, to);
			}
		}
	}
	//Build Path
	printf("Building path\n");
	Vertex curr = dest;
	int length = 0;
	while (curr != -1) {
		printf("Start build\n");
		path[length] = curr;
		curr = prev[curr];
		length++;
	}

	printf("Start reverse\n");
   	int i = 0;       // i will be pointing to first element
   	int j = i - 1;   // j will Point to last Element

   	int temp;
   	while (i < j) {
    	temp = path[i];
    	path[i] = path[j];
    	path[j] = temp;
    	i++;             // increment i
    	j--;          // decrement j
   	}


	printf("Returning length\n");
	return length;
}
