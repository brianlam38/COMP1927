// Graph.c ... implementation of Graph ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"

// ### FUNCTION DECLARATIONS ###
int nComponents(Graph g);
void dfsComponents(Graph g, Vertex v, int c);

// type for small +ve int values
typedef unsigned char bool;

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int    nV;    // #vertices
	int    nE;    // #edges
	bool **edges; // matrix of booleans
} GraphRep;

// validV ... check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// mkEdge ... create an Edge value
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
	assert(validV(g,v) && validV(g,w));
	Edge e; e.v = v; e.w = w;
	return e;
}

// newGraph ... create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	int i, j;
	bool **e = malloc(nV * sizeof(bool *));
	assert(e != NULL);
	for (i = 0; i < nV; i++) {
		e[i] = malloc(nV * sizeof(bool));
		assert(e[i] != NULL);
		for (j = 0; j < nV; j++)
			e[i][j] = 0;
	}
	Graph g = malloc(sizeof(GraphRep));
	assert(g != NULL);
	g->nV = nV;  g->nE = 0;  g->edges = e;
	return g;
}

// readGraph ... read graph contents from file
static void readError()
{
	fprintf(stderr,"Bad graph data file\n");
	exit(1);
}
Graph readGraph(FILE *in)
{
	Graph g;
	char line[100];
	// get #vertices and create graph
	int nV = 0;
	if (fgets(line,100,in) == NULL) readError();
	if (sscanf(line,"%d",&nV) != 1) readError();
	if (nV < 2) readError();
	g = newGraph(nV);
	// read edge data and add edges
	Vertex v, w;
	while (fgets(line,100,in) != NULL) {
		sscanf(line,"%d-%d",&v,&w);
		insertE(g, mkEdge(g,v,w));
	}
	return g;
}

// showGraph ... display a graph
void showGraph(Graph g)
{
	assert(g != NULL);
	printf("# vertices = %d, # edges = %d\n\n",g->nV,g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf("%d is connected to",v);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf(" %d",w);
			}
		}
		printf("\n");
	}
}

// insertE ... add a new edge
void  insertE(Graph g, Edge e)
{
	assert(g != NULL);
	assert(validV(g,e.v) && validV(g,e.w));
	if (g->edges[e.v][e.w]) return;
	g->edges[e.v][e.w] = 1;
	g->edges[e.w][e.v] = 1;
	g->nE++;
}

// removeE ... delete an edge
void  removeE(Graph g, Edge e)
{
	assert(g != NULL);
	assert(validV(g,e.v) && validV(g,e.w));
	if (!g->edges[e.v][e.w]) return;
	g->edges[e.v][e.w] = 0;
	g->edges[e.w][e.v] = 0;
	g->nE--;
}


// ### GLOBAL VARIABLES ###
int ncounted;	   // #vertices included so far
int *componentOf;  // array of component IDs
				   // indexed by vertex 0..V-1

// ### SOLUTION ###
// DEPTH FIRST SEARCH for connected components
// Returns # of connected components in a graph
int nComponents(Graph g)
{
   int i, comp = 0; // Comp = COMPONENT #ID

   componentOf = malloc(g->nV*sizeof(int));			// #1 Allocate component array
   for (i = 0; i < g->nV; i++) componentOf[i] = -1; //    Initialise all = -1
   
   ncounted = 0;
   while (ncounted < g->nV) {				// #2 Search for non-visited vertices
      Vertex v;								//    If !visited, perform dfsR
      for (v = 0; v < g->nV; v++)			//    Increment COMP NO. / ID.
         if (componentOf[v] == -1) break;
      dfsComponents(g, v, comp);
      comp++;
   }										// Recursion in dfsR will make sure that
   // componentOf[] is now set 			    // all vertices in the same subgraph will
   return comp; 							// have the same compID, as they will be assigned
   											// the same ID. Only after searching through all
   											// vertices in the component
}
void dfsComponents(Graph g, Vertex v, int c)
{
   componentOf[v] = c;
   ncounted++;
   Vertex w;
   for (w = 0; w < g->nV; w++) {
      if (g->edges[v][w] && componentOf[w] == -1)
         dfsComponents(g, w, c);
   }
}

// ####################
// ANALYSIS OF CC COUNT
// ####################

// What is the difference between normal recursive DFS and CC dfs?

// In the wrapper DFS function, dfsR is only called on
// vertices which have not been visited (componentOf[v] == -1)
// Also, increment the componentID (occurs after all components
// of Vertex V have been recursively found), so that all vertices
// that are within the same component have the same ID.

// Else, iterate to the next elt in connectedArray.

// In the main dfsR function:
// #1 Assign componentID to the Vertex V being dfsR'd
// #2 Increment #vertices counted
// #3 If Vertex W is connected to V && W has not been visited, perform dfsR on W
   // (with the same componentID)


