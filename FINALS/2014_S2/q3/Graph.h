// Graph.h ... interface to Graph ADT

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct _graph_rep *Graph;

// vertices denoted by integers 0..N-1
typedef int Vertex;
int   validV(Graph,Vertex); //validity check

// connectivity pairs (vertex,nconnections)
typedef struct _conn {
   Vertex  vertx;
   int     nconn;
} Connects;

// edges are pairs of vertices (end-points)
typedef struct { Vertex v; Vertex w; } Edge;
Edge mkEdge(Graph, Vertex, Vertex);

// operations on graphs
Graph newGraph(int nV);
Graph readGraph(FILE *);
void showGraph(Graph);

void insertE(Graph, Edge);
void removeE(Graph, Edge);

// list of highly connected vertices
Connects *wellConnected(Graph, int *);

#endif
