// tlab.c ... Tree Lab main program
// Builds trees, collects stats
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include "Tree.h"

// all values are in range 0000..9999
#define RANGE 10000

void usage();
void mkprefix(int *, int, int, int);
void mkuniq(int *, int);
int *makeValues(int, char, int);
Tree makeTree(int *, int, char);
void runTests(Tree, int *, int, char);

char *prog; // used by usage()
int ix = 0; // used by mkprefix()

int main(int argc, char *argv[])
{
	int  N = 0;        // numer values/nodes in tree
	int  seed = 1234;  // random number seed
	char order = 'R';  // order in which values are inserted
	char style = 'L';  // style of insertion
	Tree mytree; // tree
	int *values; // values inserted into tree

	// collect command-line params
	prog = argv[0]; N = 0; order = 'P'; seed = 1234;
	switch (argc) {
	case 2: N = atoi(argv[1]); order = 'P';
	        style = 'L'; break;
	case 3: N = atoi(argv[1]); order = argv[2][0];
	        style = 'L'; break;
	case 4: N = atoi(argv[1]); order = argv[2][0];
	        style = argv[3][0]; break;
	case 5: N = atoi(argv[1]); order = argv[2][0];
	        style = argv[3][0]; seed = atoi(argv[4]); break;
	default: usage(); break;
	}
	if (N < 0 || N >= RANGE) usage();

	values = makeValues(N,order,seed);
	mytree = makeTree(values,N,style);
	if (TreeNumNodes(mytree) != N) {
		fprintf(stderr,"Tree not built correctly\n");
		exit(1);
	}

	printf("Tree:\n");showTree(mytree);

	char line[20];
	printf("\n> ");
	while (fgets(line,20,stdin) != NULL) {
		int i; int show = 1;
		int value = atoi(&line[1]);
		switch (line[0]) {
		case 'i':
			TreeInsert(mytree,value);
			break;
		case 'd':
			TreeDelete(mytree,value);
			break;
		case 'f':
			if (TreeFind(mytree, value))
				printf("Found!\n");
			else
				printf("Not found\n");
			break;
		case 's':
			// nothing to do ... it's displayed below
			break;
		case 'v':
			for (i = 0; i < N; i++) {
				if (i%15 == 0) printf("\n");
				printf("%d ",values[i]);
			}
			printf("\n");
			break;
		case 't':
			runTests(mytree, values, N, order);
			break;
		case '?':
			printf("Commands:\n");
			printf("i N ... insert N into tree\n");
			printf("d N ... delete N from tree\n");
			printf("f N ... search for N in tree\n");
			printf("s ... display tree if not big\n");
			printf("v ... print values in tree\n");
			printf("t ... run tests on tree\n");
			printf("q ... quit\n");
			show = 0;
			break;
		case 'q':
			return 0;
			break;
		}
		if (show) {
			printf("Tree:\n");
			showTree(mytree);
		}
		printf("\n> ");
	}

	return 0;
}

// run search tests on tree
void runTests(Tree t, int *values, int N, char order)
{
	int i, x, ok = 0, NN = 0;  Key not[RANGE];

	printf("Tree search test\n");
	// search for values known to be in tree
	srand(0); // for consistency
	x = rand()%N; // in case the first is reuse
	for (i = 0; i < N; i++) {
		if (rand()%10 < 1) // on 1 in 10 loops
			x = x+1-1; // reuse last search key
		else
			x = rand()%N; // random search key
		// should be found
		if (TreeFind(t,values[x])) ok++;
	}
	printf("Search for %d values known to be in tree\n",N);
	printf("Found %d matches; ",ok);
	printf("%s\n", (ok == N) ? "ok" : "not ok");
	// set up array of values *not* in tree
	int start, incr;
	ok = 0;
	not[NN++] = 0; not[NN++] = RANGE;
	switch (order) {
	case 'A': start = N; incr = 1; break;
	case 'D': start = values[N-1]-1; incr = -1;
	case 'P': start = N; incr = 1;
	case 'R': start = 0; incr = 1; break;
	}
	for (x = start; NN < N/3; x += incr) {
		for (i = 0; i < N; i++) {
			if (x == values[i]) break;
		}
		if (i == N) { // x is not in tree
			not[NN++] = x;
		}
	}
	// search for values *not* in tree
	for (i = 0; i < NN; i++) {
		if (!TreeFind(t, not[i])) ok++;
	}
	printf("Search for %d values known to *not* be in tree\n",NN);
	printf("Found %d matches; ",NN-ok);
	printf("%s\n", (ok == NN) ? "ok" : "not ok");
	
}

// generate array of values to be inserted in tree
int *makeValues(int N, char order, int seed)
{
	int i, *values;

	values = malloc(N*sizeof(int));
	switch (order) {
	case 'A':
		seed = 1;
		for (i = 0; i < N; i++) values[i] = seed++;
		break;
	case 'D':
		seed = N;
		for (i = 0; i < N; i++) values[i] = seed--;
		break;
	case 'P':
		ix = 0;
		mkprefix(values, N, 1, N);
		break;
	case 'R':
		srand(seed);
		mkuniq(values, N);
		break;
	default:
		usage(); break;
	}
	return values;
}

// create the tree and insert the values
Tree makeTree(int *values, int N, char style)
{
	Tree t;  Style ins = InsertAtLeaf;  int i;

	switch (style) {
	case 'L': ins = InsertAtLeaf; break;
	case 'A': ins = InsertAtRoot; break;
	case 'R': ins = InsertRandom; break;
	case 'B': ins = InsertRebalance; break;
	case 'S': ins = InsertSplay; break;
	case 'V': ins = InsertAVL; break;
	default:  usage(); break;
	}
	t = newTree(ins);
	for (i = 0; i < N; i++) TreeInsert(t,values[i]);
	return t;
}

void mkprefix(int *v, int N, int lo, int hi)
{
	if (ix >= N || lo > hi) return;
	int mid = (lo+hi)/2;
	v[ix++] = mid;
	mkprefix(v,N,lo,mid-1);
	mkprefix(v,N,mid+1,hi);
}

void mkuniq(int *v, int N)
{
	int i, x, already[RANGE];
	for (i = 0; i < RANGE; i++)
		already[i] = 0;
	i = 0;
	while (i < N) {
		x = 1 + rand()%(RANGE-1);
		if (already[x]) continue;
		v[i++] = x;
		already[x]++;
	}
}

void usage()
{
	fprintf(stderr, "Usage: %s N Order Insert Seed\n",prog);
	fprintf(stderr, "0 <= N <= 9999, Seed = a random number\n");
	fprintf(stderr, "Order = Ascending|Descending|Prefix|Random\n");
	fprintf(stderr, "Insert = Leaf|At-root|reBalance|Random|Splay|aVl\n");
	fprintf(stderr, "For Order and Insert, use just the upper-case letter\n");
	fprintf(stderr, "e.g. for AVL, use V; for Rebalancing, use B\n");
	exit(1);
}
