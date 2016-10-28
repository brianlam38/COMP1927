int degree(Graph g, Vertex v)
{
	int n, i;
	for (i = 0; i < g->nV; i++)
		if (hasEdge(v,i)) n++;
	return n;
}

// compute degree of each vertex and store in ds[]

void degrees(Graph g, int ds[])
{
	assert(g != NULL);
#ifdef ADJ_MATRIX
	for (i = 0; i < g->nV; i++) {
		ds[i] = 0;
		for (j = 0; j < g->nV; j++) {
			if (g->edges[i][j] != 0) ds[i]++;
		}
	}
#else
	// adj list
	ds[i] = ListLength(g->lists[i]);
#endif
}

int hasEulerPath(Graph g, Vertex v, Vertex w)
{
   int degreeOf = calloc(nV(g), sizeof(int));
   degrees(g, degreeOf);
   int t = degreeOf[v] + degreeOf[w];
   if ((t % 2) != 0) return 0;
   Vertex x;
   for (x = 0; x < nV(g); x++) {
      if (x != v && x != w) {
         if ((degreeOf[x] % 2) != 0)
            return 0;
      }
   }
   return 1;
}

int sameComponent(Graph g, Vertex v, Vertex w)
{
	return g->componentOf[v] == g->componentOf[w];
}

cc[nV] = {0=>1, 1=>1, 2=>1, 3=>3, 4=>3, 5=>2, 6=>2}

int nComponents(Graph g)
{
	int max = 0, i;
	for (i = 0; i < nV(g); i++)
		if (g->componentOf[i] > max) max = componentOf[i];
	return max;
}
