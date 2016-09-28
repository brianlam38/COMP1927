foreach (y in neighbours(x)) {
	if (y == w) return TRUE;
	...
}

for adjacency matrix

for (y = 0; y < g->nV; y++) {
	if (g->edges[x][y] == 0) continue;
	if (y == w) return TRUE;
	...
}

for adjacency list

for (cur = g->edges[x]; cur != NULL; cur = cur->next) {
	y = cur->v;
	if (y == w) return TRUE;
	...
}
	
