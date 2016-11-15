// Graph.c ... implementation of Graph ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"

// FUNCTION DECLARATION
void swapCount(int a, int b);
void swapVer(Vertex x, Vertex y);

// type for small +ve int values
typedef unsigned char bool;

// graph representation (adjacency matrix)
typedef struct _graph_rep {
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
         if (g->edges[v][w]) printf(" %d",w);
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

// wellConnected ... list of vertices
// - ordered on #connections, most connected first
Connects *wellConnected(Graph g, int *nconns)
{
   assert(g != NULL && nconns != NULL);

   // #1 FN takes in a graph + *n (counts # of connected v)
   // #2 Computes an array of struct "Connects", which holds V# and nconn# for the vertex

   // A well connected vertice is one that has >= 2 connections
   // Arranged from MOST CONNECTED --> LEAST CONNECTED (descending)
   // With vertices that have SAME nconn, order by ascending order of vertices

   // #1 Alllocate array of "Connects" structs
   Connects *nc = malloc(g->nV * sizeof(Connects));
   assert(nc != NULL);
   for (int i = 0; i < g->nV; i++) {
      nc[i].vertx = i;
      nc[i].nconn = 0;
   }
   // #2 Count number of neighbours
   int totalC = 0;
   int v, w;
   for (v = 0; v < g->nV; v++) {
      int nneighbours = 0;
      for (w = 0; w < g->nV; w++) {
         if (g->edges[v][w])
            nneighbours++;
         else
            continue;
      }
      if (nneighbours >= 2) {          // Determine if V is "well-connected"
         nc[v].nconn = nneighbours;
         totalC++;
      }
   }

   // #3 Sort the sequence of "Connects" structs
   int count = 0;
   int i;
   for (count = 0; count < g->nV; count++) {
      for (i = 0; i < (g->nV - 1); i++) {
         if (nc[i].nconn < nc[i+1].nconn) {           // nconn1 < nconn2
            swapCount(nc[i].nconn,nc[i+1].nconn);        // Swap for DESCENDING
            swapVer(nc[i].vertx,nc[i+1].vertx);
         } else if (nc[i].nconn == nc[i+1].nconn) {   // nconn1 = nconn
            if (nc[i].vertx > nc[i+1].vertx)             // Swap for ASCENDING
               swapVer(nc[i].vertx,nc[i+1].vertx);
         }
      }
   }
   // #4 Set *nconns (total number of "well-connected")
   //    + return ptr to sequence of structs
   *nconns = totalC;
   return nc;
}

// Swap ncount
void swapCount(int a, int b)
{
   int tempC = a;
   a = b;
   b = tempC;
}

// Swap vertx
void swapVer(Vertex x, Vertex y)
{
   Vertex tempV = x;
   x = y;
   y = tempV;
}






