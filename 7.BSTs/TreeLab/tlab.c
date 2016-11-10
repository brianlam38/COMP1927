// useT.c ... client for Tree ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include "Tree.h"

void usage();
void help();
void mkprefix(int *, int, int, int);
void mkuniq(int *, int);
Tree makeTree(int, char, int);

int ix = 0; // used by mkPrefix

int main(int argc, char *argv[])
{
	Tree mytree;
	int N, seed; char order; // params

	// collect command-line params
	switch (argc) {
	case 1: N = 7; order = 'P';
	        seed = 1234; break;
	case 2: N = atoi(argv[1]); order = 'P';
	        seed = 1234; break;
	case 3: N = atoi(argv[1]); order = argv[2][0];
	        seed = 1234; break;
	case 4: N = atoi(argv[1]); order = argv[2][0];
	        seed = atoi(argv[3]); break;
	default: usage(); exit(0);
	}
	if (N < 0 || N >= 90) usage();

	mytree = makeTree(N,order,seed);

	printf("#nodes = %d\n",TreeNumNodes(mytree));
	printf("Original Tree:\n");showTree(mytree);

	char line[20];  int noShow = 0;
	printf("\n> ");
	while (fgets(line,20,stdin) != NULL) {
		int value = atoi(&line[1]);
		Item *ip;
		switch (line[0]) {
		case 'n':
			dropTree(mytree);
			sscanf(&line[1],"%d %c %d",&N,&order,&seed);
			mytree = makeTree(N,order,seed);
			break;
		case 'i':
			mytree = TreeInsert(mytree,value);
			break;
		case 'I':
			mytree = insertAtRoot(mytree,value);
			break;
		case 'A':
			mytree = insertAVL(mytree,value);
			break;
		case 'J':
			mytree = insertRandom(mytree,value);
			break;
		case 'd':
			mytree = TreeDelete(mytree,value);
			break;
		case 'R':
			mytree = rotateR(mytree);
			break;
		case 'L':
			mytree = rotateL(mytree);
			break;
		case 'g':
			ip = get_ith(mytree, value);
			printf("Item=%d\n",key(*ip));
			noShow = 1;
			break;
		case 'p':
			mytree = partition(mytree, value);
			break;
		case 'r':
			mytree = rebalance(mytree);
			break;
		case 'f':
			if (TreeFind(mytree, value))
				printf("Found!\n");
			else
				printf("Not found\n");
			noShow = 1;
			break;
		case 'q':
			return 0;
			break;
		default:
			help();
			noShow = 1;
			break;
		}
		if (noShow) { noShow = 0; printf("\n> "); continue; }
		printf("New Tree:");
		printf("  #nodes=%d,  ",TreeNumNodes(mytree));
		printf("  depth=%d\n",TreeDepth(mytree));
		if (TreeDepth(mytree) < 8)
			showTree(mytree);
		else
			printf("New Tree is too deep to display nicely\n");
		printf("\n> ");
	}

	return 0;
}

// generate values and insert into tree
Tree makeTree(int N, char order, int seed)
{
	Tree t = newTree();
	int  i, *values;

	if (N < 0 || N >= 90) N = 15;
	values = malloc(N*sizeof(int));
	switch (order) {
	case 'A':
		seed = 10;
		for (i = 0; i < N; i++) values[i] = seed++;
		break;
	case 'D':
		seed = 99;
		for (i = 0; i < N; i++)
			values[i] = seed--;
		break;
	case 'P':
		ix = 0;
		mkprefix(values, N, 10, 10+N);
		break;
	case 'R':
		srand(seed);
		mkuniq(values, N);
		break;
	default:
		usage(); break;
	}
	for (i = 0; i < N; i++) {
		printf("%d ",values[i]);
		//t = insertRandom(t,values[i]);
		t = TreeInsert(t,values[i]);
	}
	printf("\n");

	free(values);
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
	int i, j, x;
	i = 0;
	while (i < N) {
		x = 10+rand()%90;
		for (j = 0; j < i; j++)
			if (v[j] == x) break;
		if (j == i)
			v[i++] = x;
	}
}

void usage()
{
	fprintf(stderr, "Usage: useT N Order Seed\n");
	fprintf(stderr, "0<=N<90, Order = A|D|P|R, Seed = ?\n");
	exit(1);
}

void help()
{
	printf("Commands:\n");
	printf("n N Ord Seed = make a new tree\n");
	printf("i N = insert N into tree\n");
	printf("I N = insert N into tree at root\n");
	printf("d N = delete N from tree\n");
	printf("f N = search for N in tree\n");
	printf("g I = get the i'th element in tree\n");
	printf("p I = partition tree around i'th element\n");
	printf("r = rebalance tree\n");
	printf("R = rotate tree right around root\n");
	printf("L = rotate tree left around root\n");
	printf("q = quit\n");
}
