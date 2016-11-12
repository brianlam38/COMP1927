// Global Variable
int *visited;

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
	visited[v] = order++;

	while (!StackIsEmpty(q)) {
		Vertex y;
		Vertex x = StackPop(q);
		for (y = nV(g)-1; y >= 0; y--) {	// Reverse order (FILO)
			if (!hasEdge(g,x,y))
				continue;
			if (!visited[y])
				StackPush(q,y);
				visited[y] = order++;
		}
	}
}