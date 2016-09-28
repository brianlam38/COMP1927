ToVisit = {V}   Visited = {}
while (still some vertices in ToVisit) {
    X = remove a vertex from ToVisit
    Visited += X
    foreach (Y in neighbours(X)) {
        if (Y == W) return TRUE
        if (Y not in Visited && Y not in ToVisit) ToVisit += Y
}   }

Graph = { a->b,c  b->a,c,d  c->a,b,d  d->b,c,e  e->d }

isPath(a,e)
Iteration ToVisit   Visited
0         a         -
1         b,c       a
2         c,d       a,b
3         d         a,b,c
4                   a,b,c,d
6         ?         ?
7         ?         ?
8         ?         ?
9         ?         ?
