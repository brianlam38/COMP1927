// useT.c ... client for BSTree ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include "BSTree.h"

int main(int argc, char *argv[])
{
	BSTree mytree = newBSTree();

	mytree = BSTreeInsert(mytree,12);
	mytree = BSTreeInsert(mytree,6);
	mytree = BSTreeInsert(mytree,18);
	mytree = BSTreeInsert(mytree,9);
	mytree = BSTreeInsert(mytree,3);
	mytree = BSTreeInsert(mytree,15);
	mytree = BSTreeInsert(mytree,21);
	printf("#nodes = %d,   ",BSTreeNumNodes(mytree));
	printf("depth = %d\n",BSTreeDepth(mytree));
	printf("Original Tree:\n");showBSTree(mytree);

	char line[20];
	printf("\n> ");
	while (fgets(line,20,stdin) != NULL) {
		int value = atoi(&line[1]);
		if (line[0] == 'i')
			mytree = BSTreeInsert(mytree,value);
		else if (line[0] == 'I')
			mytree = BSTreeInsertI(mytree,value);
		else if (line[0] == 'd')
			mytree = BSTreeDelete(mytree,value);
		else if (line[0] == 'f')
			printf("%d\n", BSTreeFind(mytree, value));
		else if (line[0] == 'F')
			printf("%d\n", BSTreeFindI(mytree, value));
		else if (line[0] == 'q')
			break;
		printf("#nodes = %d,   ",BSTreeNumNodes(mytree));
		printf("depth = %d\n",BSTreeDepth(mytree));
		printf("New Tree:\n");
		showBSTree(mytree);
		printf("\n> ");
	}
	return 0;
}
