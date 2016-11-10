// write your tests here
// compile with:  gcc -Wall -Werror -O -o testBST testBST.c BST.c q2a.c q2b.c q2c.c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "BST.h"
// insert proper tests here
int main (int argc, const char * argv[]) {


	treelink palm_tree = NULL;
	dlist s = createList();
	palm_tree = insertTreeNode(palm_tree, 5);
	insertTreeNode(palm_tree, 8);
	insertTreeNode(palm_tree, 51);
	insertTreeNode(palm_tree, 15);
	insertTreeNode(palm_tree, 11);
	insertTreeNode(palm_tree, 3);
	insertTreeNode(palm_tree, 2);
	preorderTraversal(palm_tree, printTreeNode);
	printf("\n");
	BSTtoDL(s, palm_tree);
	printDList(s);
  
  printf("If you don't get a sorted tree you did something wrong:(!\n");
 

  return EXIT_SUCCESS;;
}
