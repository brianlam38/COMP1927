// BSTree.h ... interface to binary search tree ADT

#ifndef BSTREE_H
#define BSTREE_H

typedef struct BSTNode *BSTree;

// create an empty BSTree
BSTree newBSTree();
// free memory associated with BSTree
void dropBSTree(BSTree);
// display a BSTree
void showBSTree(BSTree);
// display BSTree root node
void showBSTreeNode(BSTree);

// insert a new value into a BSTree
BSTree insert(BSTree, int);
// delete a value from a BSTree
BSTree delete(BSTree, int);
// check whether a value is in a BSTree
int find(BSTree, int);
// apply a function to each node in BSTree
void map(BSTree, int (*f)(int));

#endif
