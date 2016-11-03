// Graph.h ... interface to Graph ADT

#ifndef GRAPH_H
#define GRAPH_H

// graph representation is hidden
typedef struct GraphRep *Graph;

// vertices denoted by integers 0..N-1
typedef int Vertex;
int   validV(Graph,Vertex); //validity check

// edges are pairs of vertices (end-points)
typedef struct { Vertex v; Vertex w; } Edge;
Edge mkEdge(Graph, Vertex, Vertex);

// operations on graphs
Graph newGraph(int nV);  // #vertices
void  insertE(Graph, Edge);
void  removeE(Graph, Edge);
void  dropGraph(Graph);
void  show(Graph);

// DFS Algorithms on Graphs
void dfs(Graph, Vertex);
int dfsHasPath(Graph, Vertex, Vertex);
void dfsFindPath(Graph, Vertex, Vertex);

// BFS Algorithms on Graphs
void bfs(Graph, Vertex);
int hasPath(Graph, Vertex, Vertex);
void findPath(Graph, Vertex, Vertex);

// all vertices within distance d of Vertex v
int within(Graph, Vertex, int, Vertex *);

#endif
