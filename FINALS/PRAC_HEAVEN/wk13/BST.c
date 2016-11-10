#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct treeNode *treelink;
typedef int TreeItem;

struct treeNode {
    TreeItem item;
    treelink left;
    treelink right;
};

// Write a function that takes in two BSTs and determines if they
// have the exact same formation
int isSameBST(treelink t, treelink s){
	// TODO your implementation here.
	return -1;
}

// Return the lowest common ancestor of two nodes
// ie. the parent common to both nodes at the deepest level in the tree
treelink lca(treelink root, int v1, int v2) {
	// TODO your implementation here
	return NULL;
}


treelink createNode(TreeItem item){
     treelink t = (treelink) malloc (sizeof (*t));
     t->item = item;
     t->left = NULL;
     t->right = NULL;
     return t;
}

treelink insertTreeNode (treelink tree, TreeItem item) {
  if (tree == NULL) {
      tree = createNode(item);
      tree->item = item;
      tree->left = NULL;
      tree->right = NULL;
   
  } else if (tree->item < item) {
      tree->right = insertTreeNode (tree->right, item);
  } else {
      tree->left = insertTreeNode (tree->left, item);
  }
  return tree;
}

treelink makeTreeWithKeys(int * keys, int n){
	int i;
	treelink root = NULL;
	for(i=0; i<n; i++){
		root = insertTreeNode(root, keys[i]);
	}
	return root;
}

int main(){
	// write your test cases here
	int a1[] = {2,4,1,3,9,10};
	int a2[] = {2,4,3,1,9,10};

	treelink ta1 = makeTreeWithKeys(a1, sizeof(a1)/sizeof(int));
	treelink ta2 = makeTreeWithKeys(a2, sizeof(a2)/sizeof(int));

	int b1[] = {8,3,6,1,4,7,10,14,13};
	int b2[] = {8,10,14,3,6,4,1,7,13};

	treelink tb1 = makeTreeWithKeys(b1, sizeof(b1)/sizeof(int));
	treelink tb2 = makeTreeWithKeys(b2, sizeof(b2)/sizeof(int));

	int c1[] = {12,2,14,1,7,13,18};
	int c2[] = {12,14,18,13,2,7,1};

	treelink tc1 = makeTreeWithKeys(c1, sizeof(c1)/sizeof(int));
	treelink tc2 = makeTreeWithKeys(c2, sizeof(c2)/sizeof(int));

	assert(isSameBST(ta1,ta2));
	assert(isSameBST(tb1,tb2));
	assert(isSameBST(tc1,tc2));
	printf("Passed all isSameBST() tests!\n");
	treelink a = lca(ta1, 2, 4);
	assert(a != NULL);
	assert(a->item == 2);
	treelink b = lca(tb1, 1, 4);
	assert(b != NULL);
	assert(b->item == 3);
	treelink c = lca(tc1, 18, 13);
	assert(c != NULL);
	assert(c->item == 14);
	
	
	printf("Well done, you passed all of the tests, but you should make some more.\n");
	return 0;

}
