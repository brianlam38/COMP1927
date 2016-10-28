int *visited; // array [0..V-1] of visiting order

void bfs(Graph g, Vertex v)
{
   int i, order = 0;
   visited = malloc(g->nV * sizeof(int));
   for (i = 0; i < g->nV; i++) visited[i] = -1;
   Queue q = newQueue();
   QueueJoin(q,v);
   while (!QueueIsEmpty(q)) {
      Vertex y, x = QueueLeave(q);
      if (beenVisited(x,visited,g->nV)) continue;
      visited[order++] = x;
      for (y = 0; y < g->nV; y++) {
         if (!g->edges[x][y]) continue;
         if (!beenVisited(x,visited,order))
            QueueJoin(q,y);
      }
   }
}

bool beenVisited(Vertex x, int vis[], int n)
{
   int i;
   for (i = 0; i < n; i++) {
      if (vis[i] == x) return 1;
   }
   return 0;
}
