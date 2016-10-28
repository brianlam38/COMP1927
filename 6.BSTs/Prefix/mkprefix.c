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

/* Generates sequence of numbers in PREFIX ORDER
   Optimal order for inserting into binary search tree
   int *v = array
   int N = size of array
   int lo, int hi = bounds
*/
void mkprefix(int *v, int N, int lo, int hi)
{
	if (ix >= N || lo > hi) return;	// Everything generated: IX >= N or lo > hi.
	int mid = (lo+hi)/2;			// Grab middle value of range
	v[ix++] = mid;					// Store val in next slot in array
	// NOTE: ix = global var as recursive calls will reset ix
	mkprefix(v,N,lo,mid-1);			// Recursively make prefix for LHS
	mkprefix(v,N,mid+1,hi);			// Recursively make prefix for RHS
}
