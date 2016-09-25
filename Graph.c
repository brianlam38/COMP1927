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

typedef struct list {	
	int *items;
	int size;
	int numItems;

} list;
typedef struct list *List;
 

static List makeList(int size);
//static int numItems(List l);
static void addToList(List l,int x, int tag);
static void disposeList(List l);
static int inList(List l, int x);
static int getTag(List l, int x);

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

// display graph, using names for vertiint getTag(List l, int x)ces
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
	assert(g != NULL);
	Vertex curr = 0;					// Declare curr vertex var

	Queue toVisit = newQueue();			// Declare queue ptr
	List seenList = makeList(g->nE);	// Declare seenList ptr
	
	addToList(seenList, src, -1);		// Add source to seenList
	QueueJoin(toVisit,src);				// Add source to queue
	int len = 0;
	path[0] = src;						// First = path sources

	int i = 0;

	while (!QueueIsEmpty(toVisit)) {

		curr = QueueLeave(toVisit);		// Pop curr of stack	
		
		for (i = 0; i < g->nV; i++) {
			if (g->edges[curr][i] < max && g->edges[curr][i] != 0 &&
				!inList(seenList,i)) {
				addToList(seenList,i, curr);	// AddToList
				if (i == dest) { 
					path[len] = dest;
					break;	
				}				

				QueueJoin(toVisit,i);
			}
		}
		if (inList(seenList,dest)) break;
	}

	if (inList(seenList,dest)) {
		Vertex prev = dest;
		for (i = 0; prev != -1; i++,len++) {
			prev = getTag(seenList,prev);
		}

		prev = dest;
		for (i = len-1; i >= 0; i--) {
			path[i] = prev;
			prev = getTag(seenList,prev);
		}

	} else len = 0;

	disposeList(seenList);
	dropQueue(toVisit);
	return len;
}

static List makeList(int size) {
	List l = malloc(sizeof(struct list));
	l->items = malloc(sizeof(int)*2*size);
	l->size = size;
	l->numItems = 0;
	
	return l;
}
/*
static int numItems(List l) {
	return l->numItems;
}
*/

// Stores prev element + curr element in seenList
static void addToList(List l,int x, int tag) {

	if (l->numItems < l->size) {
		l->items[l->numItems] = x;
		l->numItems++;
		l->items[l->numItems] = tag;
		l->numItems++;
	}
}

static void disposeList(List l) {

	free(l->items);
	free(l);
}
static int inList(List l, int x) {

	int i = 0;
	for (i = 0; i < l->numItems; i+=2) {
		if (l->items[i] == x) return 1;
	}
	return 0;
}
static int getTag(List l, int x) {
	int i = 0;
	for (i = 0; i <l->numItems; i+=2) {
		if (l->items[i] == x) 
			return l->items[i+1];
	}
	return -3;
}

