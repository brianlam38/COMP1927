typedef int Vertex;

typedef struct edge{ 
  Vertex v; 
  Vertex w; 
} Edge;

Edge EDGE (Vertex v, Vertex w);

typedef struct graph *Graph;

//operations on graphs
Graph GRAPHinit (int nV);
 void GRAPHinsertE (Graph g, Edge e);
int GRAPHvalidEdge(Graph g, Edge e);

//returns 1 if there is an edge from v to w
int GRAPHisAdjacent(Graph g, Vertex v, Vertex w);
int GRAPHadjacentVertices(Graph g, Vertex v, Vertex adj[]);

//Returns #vertices and array of edges
  int GRAPHedges (Edge [], Graph g);

int GRAPHnumV(Graph g);
int GRAPHnumE(Graph h);
void GRAPHEdgePrint (Edge);
void GRAPHshow(Graph g);

Graph GRAPHcopy(Graph);
void GRAPHdestroy(Graph);
int hasEuler(Graph g);

