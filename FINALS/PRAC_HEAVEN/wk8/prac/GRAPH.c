//This implements an undirected unweighted graph
//This implementation allows parallel edges

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "GRAPH.h"


typedef struct vnode * vlink;
struct vnode { 
  Vertex v; 
  vlink next; 
};

struct graph { 
  int V; 
  int E; 
  vlink *adj; //an array of linked lists 
};


Edge EDGE (Vertex v, Vertex w ) {
  Edge e = {v, w};
  return e;
}

int GRAPHvalidEdge(Graph g, Edge e){
    assert(g != NULL);
    return (e.v >= 0 && e.v < g->V && 
            e.w >= 0 && e.w < g->V );
}

//Creates a new node and sets the next link
vlink NEW (Vertex v, vlink next) { 
  vlink x = malloc (sizeof *x);

  x->v = v; 
  x->next = next;
  return x;                         
}

//O(V)
Graph GRAPHinit (int V) { 
  int v;
  Graph G = malloc (sizeof *G);

  G->V = V; 
  G->E = 0;

  G->adj = malloc (V * sizeof(vlink));
  for (v = 0; v < V; v++){
    G->adj[v] = NULL;
  }
  return G;
}

void GRAPHdestroy (Graph g) {
    int i;
    //First free all linked lists
    for(i = 0; i < g->V; i++){
        vlink curr = g->adj[i];
        vlink prev = NULL;
        while(curr != NULL){
            prev = curr;
            curr = curr->next;
            free(prev);
        }
    }
    //Free array
    free(g->adj);
    //Free the graph struct
    free(g);
}

// NOTE: YOU DO NOT NEED TO COMPLETE THIS FUNCTION
Graph GRAPHcopy(Graph g){
  //not implemented yet
  return NULL;
}

//Undirected graph. So each edge
//is stored twice - once from v
//and once from w
//O(1)
//This does not check for parallel edges
//To do so would make it O(n)
void GRAPHinsertE(Graph G, Edge e){ 
  if(GRAPHvalidEdge(G,e) && e.v != e.w){
     int v = e.v;
     int w = e.w;
  
     G->adj[v] = NEW (w, G->adj[v]);
     G->adj[w] = NEW (v, G->adj[w]); 
     G->E++;
  } else {
     printf("Edge not valid ignoring ");
     GRAPHEdgePrint(e);
     printf("\n");
  }
}

int GRAPHisAdjacent(Graph g,Vertex v, Vertex w){
    vlink tmp = g->adj[v];
    int adjacent = 0;
    while(tmp != NULL && !adjacent){
        if(tmp->v == w){
            adjacent = 1;
        }
        tmp = tmp->next;
    }
    return adjacent;
}

int GRAPHadjacentVertices(Graph g, Vertex v, Vertex adj[]){
    int counter = 0;
    vlink tmp = g->adj[v];
    while(tmp != NULL){
        adj[counter] = tmp->v;
        counter++;
        tmp = tmp->next;
    }
    return counter;
}

void GRAPHEdgePrint (Edge edge) {
    printf ("%d -- %d", edge.v, edge.w);
}

int GRAPHnumV(Graph g){
    return g->V;
}

int GRAPHnumE(Graph g){
    return g->E;
}

void GRAPHshow(Graph g){
    printf("V=%d, E=%d\n", g->V, g->E); 
    int i; 
    for (i = 0; i < g->V; i++) { 
        vlink tmp = g->adj[i]; 
        while (tmp != NULL) { 
            printf("%d-%d ",i,tmp->v); 
            tmp = tmp->next; 
        } 
        if (g->adj[i] != NULL) 
            printf("\n"); 
    }
    printf("\n"); 
}

// Implement a function that checks for an existence of an Euler path in a graph. Return 1, if there is an euler path,
// 0 otherwise.
int hasEuler(Graph g){
    if(g == NULL){
        printf("Oops, no graph:(\n");
        abort();
    }
    //TODO
    return -1;
}
