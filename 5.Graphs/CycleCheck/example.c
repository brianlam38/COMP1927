trace buggy cycle check on the following graph:

   A --- B --- C --- D

visited = {A=>0, B=>0, C=>0, D=>0}


int dfsCycleCheck(Graph g, Vertex v)
{
   visited[v] = 1;
   Vertex w;
   for (w = 0; w < nV(g); w++) {
      if (!hasEdge(g,v,w)) continue;
      if (visited[w]) return 1; // found cycle
      return dfsCycleCheck(g, w);
   }
   return 0; // no cycle at v
}
