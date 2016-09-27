int *visited; // array [0..V-1] of visiting order

void bfs(Graph g, Vertex v)
{
   int i, order = 0;
   visited = calloc(nV(g),sizeof(int));
   Queue q = newQueue();
   QueueJoin(q,v);
   while (!QueueIsEmpty(q)) {
      Vertex y, x = QueueLeave(q);
      if (visited[x]) continue;
      visited[x] = order++;
      for (y = 0; y < nV(g); y++) {
         if (!hasEdge(g,x,y)) continue;
         if (!visited[y]) QueueJoin(q,y);
      }
   }
   int *new_visited = calloc(nV(g),sizeof(int));
   for (i = 0; i < nV(g); i++) {
      for (j = 0; j < nV(g); j++)
          if (visited[j] == i) break;
      new_visited[i] = j;
   }
   for (i = 0; i < nV(g); i++)
      visited[i] = new_visited[i];
   free(new_visited);
}
