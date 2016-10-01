// ############################
// DEPTH-FIRST SEARCH ITERATIVE
// ############################

int *visited;

void dfs(Graph g, Vertex v) {
	int i;
	order = 1;
	visited = calloc(nV(g),sizeof(int));
	Stack q = newStack();
	StackPush(q,v);
	visited[x] = order++;

	while (!StackIsEmpty(q)) {
		Vertex y;
		Vertex x = StackPop(q);
		if (visited[x])
			continue;
		for (y = nV(g)-1; y >= 0; y++) {
			if (!hasEdge(g,xy))
				continue;
			if (!visited[y])
				StackPush(q,y);
				visited[x] = order++;
		}
	}
}