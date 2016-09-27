// ccGraph.c ... Graph ADT with connected component map

typedef struct GraphRep {
   int nV;      // # vertices
   int nE;      // # edges
   int **edges; // adjacency matrix
   int nC;      // # connected components
   int *cc;     // vertex->component map
} GraphRep;

// local stuff

static int ncounted;      // # vertices allocated
static int *componentOf;  // array of component ids
                   // indexed by vertex 0..V-1
static void components(Graph g)
{
   int i, componentID = 1; 
   componentOf = g->cc;
   ncounted = 0;
   while (ncounted < nV(g)) {
      Vertex v;
      for (v = 0; v < nV(g); v++)
         if (componentOf[v] == 0) break;
      dfsR(g, v, componentID);
      componentID++;
   }
   g->nC = componentID-1;
   // componentOf[0..nV-1] is now set
}
static void dfsR(Graph g, Vertex v, int c)
{
   componentOf[v] = c;
   ncounted++;
   Vertex w;
   for (w = 0; w < nV(g); w++) {
      if (!hasEdge(g,v,w)) continue;
      if (componentOf[w] == 0) dfsR(g,w,c);
   }
}

// exported stuff

// Create a new graph with nV=nC, nE=0
Graph newGraph(int nV)
{
   // initialise adj matrix
   int **es = malloc(nV*sizeof(int *));
   assert(es != NULL);
   int i,j;
   for (i = 0; i < nV; i++) {
      es[i] = malloc(nV*sizeof(int));
      assert(es[i] != NULL);
      for (j = 0; j < nV; j++) es[i][j] = 0;
   }
    // initialise component map 
   int *cc = malloc(nV*sizeof(int));
   assert(cc != NULL);
   for (i = 0; i < nV; i++) cc[i] = i+1;
   // set up Graph structure to hold above
   Graph new = malloc(sizeof(GraphRep));
   assert(new != NULL);
   new->nV = new->nC = nV;  new->nE = 0;
   new->cc = cc;  new->edges = es;
   return new;
}

// Insert a new edge
void  insertE(Graph g, Edge e)
{
   assert(g != NULL);
   if (g->edges[e.v][e.w]) return;
   if (g->cc[e.v] != g->cc[e.w]) {
      components(g);
   }
   g->nE++;
   g->edges[e.v][e.w] = 1;
   g->edges[e.w][e.v] = 1;
}

// Remove an edge
void  removeE(Graph g, Edge e)
{
   assert(g != NULL);
   if (!g->edges[e.v][e.w]) return;
   g->nE--;
   g->edges[e.v][e.w] = 0;
   g->edges[e.w][e.v] = 0;
   components(g);
}

// How many connected subgraphs are there?
int nComponents(Graph g)
{
   assert(g != NULL);
   return g->nC;
}

// Are two vertices in the same connected subgraph?
int sameComponent(Graph g, Vertex v, Vertex w)
{
   assert(g != NULL);
   return (g->cc[v] == g->cc[w]);
}
