// Tree.h ... interface to binary search tree ADT
// Written by John Shepherd, March 2013

#ifndef TREE_H
#define TREE_H

typedef struct node *Tree;

typedef int Key;
typedef Key Item; // item is just a key
#define key(it) (it)

#define cmp(k1,k2) ((k1) - (k2))
#define lt(k1,k2) (cmp(k1,k2) < 0)
#define eq(k1,k2) (cmp(k1,k2) == 0)
#define gt(k1,k2) (cmp(k1,k2) > 0)

// create an empty Tree
Tree newTree();
// free memory associated with Tree
void dropTree(Tree);
// display a Tree
void showTree(Tree);

// insert a new value into a Tree
Tree insert(Tree, Item);
Tree insertAtRoot(Tree, Item);
Tree insertRandom(Tree, Item);
// delete a value from a Tree
Tree delete(Tree, Key);
// check whether a value is in a Tree
int find(Tree, Key);
// compute depth of Tree
int depth(Tree);
// count #nodes in Tree
int nnodes(Tree);

// normally these are internal to ADT
Tree rotateR(Tree);
Tree rotateL(Tree);
Item *get_ith(Tree,int);
Tree partition(Tree,int);

#endif
