// mkprefix.c ... generate sequence in prefix order

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static int ix;
void mkprefix(int *, int, int, int);

int main(int argc, char **argv)
{
	int lo, hi, n, *v;
	assert(argc >= 3);
	assert(sscanf(argv[1],"%d",&lo) == 1);
	assert(sscanf(argv[2],"%d",&hi) == 1);
	assert(lo < hi);
	n = hi - lo + 1;
	v = malloc(n*sizeof(int));

	ix = 0;
	mkprefix(v, n, lo, hi);

	for (int i = 0; i < n; i++)
		printf("%d ",v[i]);
	printf("\n");
	return EXIT_SUCCESS;
}

void mkprefix(int *v, int N, int lo, int hi)
{
	if (ix >= N || lo > hi) return;
	int mid = (lo+hi)/2;
	v[ix++] = mid;
	mkprefix(v,N,lo,mid-1);
	mkprefix(v,N,mid+1,hi);
}
