int hasCycle(Graph g)
{
   visited = calloc(g->nV,sizeof(int));
   int result = dfsCycleCheck(g, 0, 0);
   free(visited);
   return result;
}

int dfsCycleCheck(Graph g, Vertex v, Vertex prev)
{
   visited[v] = 1;
   Vertex w;
   for (w = 0; w < nV(g); w++) {
      if (!hasEdge(v,w)) continue;
      if (!visited[w])
         return dfsCycleCheck(g, w, v);
      if (w != prev)
         return 1; // found cycle
   }
   return 0; // no cycle at v
}