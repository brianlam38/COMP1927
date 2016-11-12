// returns array [0..V-1] of component IDs

int *components(Graph g)
{
   int i, comp, ncounted, *componentOf; 
   componentOf = malloc(g->nV*sizeof(int));
   for (i = 0; i < g->nV; i++) componentOf[i] = -1;
   ncounted = 0;  comp = 0;
   while (ncounted < g->nV) {
      Vertex v;
      for (v = 0; v < g->nV; v++)
         if (componentOf[v] == -1) break;
      dfsR(g, v, comp, &ncounted, componentOf);
      comp++;
   }
   return componentOf;
}

void dfsR(Graph g, Vertex v, int c, int *nC, int cOf[])
{
   cOf[v] = c;
   *nC = *nC + 1;  // or (*nC)++, but not *(nC++) ...
   Vertex w;
   for (w = 0; w < g->nV; w++) {
      if (!g->edges[v][w]) continue;
      if (cOf[w] == -1) dfsR(g,w,c,nC,cOf);
   }
   return 1;
}

// Alternate Solution (2013_S2 Q3 Connected Component Qn)
int *componentOf;  // array of component ids
                   // indexed by vertex 0..V-1
int ncounted;      // # vertices included so far

void components(Graph g)
{
   void dfsComponents(Graph,Vertex,int);
   int i, comp = 0;
   componentOf = malloc(g->nV*sizeof(int));
   for (i = 0; i < g->nV; i++) componentOf[i] = -1;
   ncounted = 0;
   while (ncounted < g->nV) {
      Vertex v;
      for (v = 0; v < g->nV; v++)
         if (componentOf[v] == -1) break;
      dfsComponents(g, v, comp);
      comp++;
   }
   // componentOf[] is now set
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
