typedef Graph MSTree;

// sum of edge weights
// non-directional edges, adjacency matrix

float cost(MSTree t)
{
	int i, j;
	float sum = 0.0;
	for (i = 0; i < t->nV; i++) {
		for (j = i+1; j < t->nV; j++)
			sum += t->edges[i][j];
	}
	return sum;
}
