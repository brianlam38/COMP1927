// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Graph.h"
#include "Queue.h"
#include "Stack.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
   int   nV;    // #vertices
   int   nE;    // #edges
   int **edges; // matrix of booleans
} GraphRep;

// ADT-wide variables (not externally visible)
// - allocated when the graph is set up (and nV known)
// - these variables are used by various algorithms
static int  order;   // counter (how many vertices seen)
static int *visited; // array [0//nV-1] of seen vertices
static int *path;    // array [0..V-1] of vertices in path


// check validity of Vertex
int validV(Graph g, Vertex v)
{
   return (g != NULL && v >= 0 && v < g->nV);
}

// create an Edge value
Edge mkEdge(Graph g, Vertex v, Vertex w)
{
   assert(validV(g,v) && validV(g,w));
   Edge e = {v,w}; // struct assignment
   // or  Edge e; e.v = v; e.w = w;
   return e;
}

// create an empty graph
Graph newGraph(int nV)
{
   assert(nV >= 0);
   int i, j;
   int **e = malloc(nV * sizeof(int *));
   assert(e != NULL);
   for (i = 0; i < nV; i++) {
      e[i] = malloc(nV * sizeof(int));
      assert(e[i] != NULL);
      for (j = 0; j < nV; j++)
         e[i][j] = 0;
   }
   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = nV;  g->nE = 0;  g->edges = e;
    // also allocate visited[] and path[]
    path = malloc(nV*sizeof(Vertex));
    visited = malloc(nV*sizeof(Vertex));
    assert(path != NULL && visited != NULL);
   return g;
}

// add a new edge
void  insertE(Graph g, Edge e)
{
   assert(g != NULL);
   assert(validV(g,e.v) && validV(g,e.w));
   if (g->edges[e.v][e.w]) return;
   g->edges[e.v][e.w] = 1;
   g->edges[e.w][e.v] = 1;
   g->nE++;
}

// delete an edge
void  removeE(Graph g, Edge e)
{
   assert(g != NULL);
   assert(validV(g,e.v) && validV(g,e.w));
   if (!g->edges[e.v][e.w]) return;
   g->edges[e.v][e.w] = 0;
   g->edges[e.w][e.v] = 0;
   g->nE--;
}

// free memory associated with graph
void dropGraph(Graph g)
{
   assert(g != NULL);
   int i;
   for (i = 0; i < g->nV; i++)
      free(g->edges[i]);
   free(g->edges);
   free(g);
}

// display a graph (not pretty)
void show(Graph g)
{
   assert(g != NULL);
   printf("Graph has V=%d and E=%d\n",g->nV,g->nE);
   printf("V    Connected to\n");
   printf("--   ------------\n");
   int v, w;
   for (v = 0; v < g->nV; v++) {
      printf("%-3d ",v);
      for (w = 0; w < g->nV; w++) {
         if (g->edges[v][w]) printf(" %d",w);
      }
      printf("\n");
   }
}

// DFS traversal
void dfs(Graph g, Vertex v)
{
   int i, order = 0;
   for (i = 0; i < g->nV; i++) visited[i] = -1;
   Stack s = newStack();
   StackPush(s,v);
   while (!StackIsEmpty(s)) {
      Vertex y, x = StackPop(s);
      if (visited[x] != -1) continue;
      printf((g->nV < 15)?"x=%-2d":"%d ",x);
      visited[x] = order++;
      for (y = g->nV-1; y >= 0; y--) {
         if (!g->edges[x][y]) continue;
         if (visited[y] == -1) StackPush(s,y);
      }
      if (g->nV < 15) {printf("  s=");showStack(s);}
   }
}

// DFS path checker
static int dfsPathCheck(Graph g, Vertex v, Vertex dest)
{
   visited[v] = 1;
   Vertex w;
   for (w = 0; w < g->nV; w++) {
      if (!g->edges[v][w]) continue;
      if (w == dest) return 1; // found path
     if (!visited[w]) {
         if (dfsPathCheck(g, w, dest))
            return 1;
      }
   }
   return 0; // no path from v to dest
}
int dfsHasPath(Graph g, Vertex src, Vertex dest)
{
   int i;
   for (i = 0; i < g->nV; i++) visited[i] = 0;
   int result = dfsPathCheck(g, src, dest);
   return result;
}

// DFS path finder
static int dfsPathFind(Graph g, Vertex v, Vertex dest, int ord)
{
   visited[v] = 1;
   path[ord] = v;
   if (v == dest) return 1;
   Vertex w;
   for (w = 0; w < g->nV; w++) {
      if (!g->edges[v][w]) continue;
      if (!visited[w]) {
         if (dfsPathFind(g, w, dest, ord+1))
            return 1;
         else
            path[ord+1] = -1;
      }
   }
   return 0;
}
void dfsFindPath(Graph g, Vertex src, Vertex dest)
{
   int i;
   for (i = 0; i <= g->nV; i++) 
        { path[i] = -1; visited[i] = 0; }
   if (!dfsPathFind(g,src,dest,0))
      printf("No path from %d to %d\n",src,dest);
   else {
      printf("Path: %d",path[0]);
      for (i = 1; path[i] != -1; i++)
         printf("->%d",path[i]);
      printf("\n");
   }
   free(visited); free(path);
}


// BFS traversal
void bfs(Graph g, Vertex v)
{
   int i; order = 0;
   for (i = 0; i < g->nV; i++) visited[i] = -1;
   Queue q = newQueue();
   QueueJoin(q,v);
   while (!QueueIsEmpty(q)) {
      Vertex y, x = QueueLeave(q);
      if (visited[x] != -1) continue;
      printf("x=%-2d",x);
      visited[x] = order++;
      for (y = 0; y < g->nV; y++) {
         if (!g->edges[x][y]) continue;
         if (visited[y] == -1) QueueJoin(q,y);
      }
      printf("  q=");showQueue(q);
   }
}

// BFS path checker
int hasPath(Graph g, Vertex src, Vertex dest)
{
   int i;
   for (i = 0; i < g->nV; i++) visited[i] = 0;
   Queue q = newQueue();
   QueueJoin(q,src); 
   int isFound = 0;
   while (!QueueIsEmpty(q) && !isFound) {
      Vertex y, x = QueueLeave(q);
      if (visited[x]) continue;
      visited[x] = 1;
      for (y = 0; y < g->nV; y++) {
         if (!g->edges[x][y]) continue;
         if (y == dest) { isFound = 1; break; }
         if (!visited[y]) { QueueJoin(q,y); }
      }
   }
   return isFound;
}

// iterative BFS algorithm to print path src...dest
void findPath(Graph g, Vertex src, Vertex dest)
{
   int i;
   for (i = 0; i < g->nV; i++) 
        { path[i] = -1; visited[i] = 0; }
   Queue q = newQueue();
   QueueJoin(q,src);
   int isFound = 0;
   while (!QueueIsEmpty(q) && !isFound) {
      Vertex y, x = QueueLeave(q);
      if (visited[x]) continue;
      visited[x] = 1;
      for (y = 0; y < g->nV; y++) {
         if (g->edges[x][y] == 0 || visited[y]) continue;
         if (path[y] == -1) path[y] = x;
         if (y == dest) { isFound = 1; break; }
         if (!visited[y]) QueueJoin(q,y);
      }
   }
}


// Find all vertices which are <= d edges from v
int within(Graph g, Vertex s, int d, Vertex *vs)
{
   // TODO
   return 0; // replace this statement
}
