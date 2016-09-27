int isPath(Graph g, Vertex v, Vertex w)
{
   Vertex visited[] = {};  Stack s = [v];
   while (!empty(s)) {
      Vertex y, x = pop(s);
      foreach (y in neighbours(x)) {
         if (y == w) return TRUE;
         if (!visited[y]) { push(s,y); visited[y] = 1; }
   }  }
   return FALSE;
}

a -> b c    d -> c e        g -> f      j -> i k
b -> a f    e -> d f i      h -> f k    k -> h j
c -> a d    f -> b e g h    i -> e j

isPath(a,k)  =>  v = a, w = k
init:   x = ?  y = ?  s = [a]  visited = {}
iter1:  x = ?  y = ?  s = [?]  visited = {}
iter2:  x = ?  y = ?  s = [?]  visited = {}
iter3:  x = ?  y = ?  s = [?]  visited = {}
iter4:  x = ?  y = ?  s = [?]  visited = {}
iter5:  x = ?  y = ?  s = [?]  visited = {}
iter6:  x = ?  y = ?  s = [?]  visited = {}
iter7:  x = ?  y = ?  s = [?]  visited = {}
iter8:  x = ?  y = ?  s = [?]  visited = {}
iter9:  x = ?  y = ?  s = [?]  visited = {}
