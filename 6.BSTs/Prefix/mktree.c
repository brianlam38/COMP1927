// mktree.c ... build a tree read from stdin

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../TreeLab/Tree.h"

int main(int argc, char **argv)
{
	int n;
	Tree t = newTree();
	while (scanf("%d",&n) == 1)
		t = TreeInsert(t,n);
	showTree(t);
	dropTree(t);
	return EXIT_SUCCESS;
}
