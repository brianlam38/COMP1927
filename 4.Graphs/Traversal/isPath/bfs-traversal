int isPath(Graph g, Vertex v, Vertex w)
{
   Vertex visited[] = {};  Queue q = [v];
   while (!empty(q)) {
      Vertex y, x = leave(q);
      foreach (y in neighbours(x)) {
         if (y == w) return TRUE;
         if (!visited[y]) { join(q,y); visited[y] = 1; }
   }  }
   return FALSE;
}

a -> b c    d -> c e        g -> f      j -> i k
b -> a f    e -> d f i      h -> f k    k -> h j
c -> a d    f -> b e g h    i -> e j

isPath(a,k)  =>  v = a, w = k
init:   x = ?  y = ?  q = [a]  visited = {}
iter1:  x = ?  y = ?  q = [?]  visited = {}
iter2:  x = ?  y = ?  q = [?]  visited = {}
iter3:  x = ?  y = ?  q = [?]  visited = {}
iter4:  x = ?  y = ?  q = [?]  visited = {}
iter5:  x = ?  y = ?  q = [?]  visited = {}
iter6:  x = ?  y = ?  q = [?]  visited = {}
iter7:  x = ?  y = ?  q = [?]  visited = {}
iter8:  x = ?  y = ?  q = [?]  visited = {}
iter9:  x = ?  y = ?  q = [?]  visited = {}
