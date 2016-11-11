// Tree.h ... interface to binary search tree ADT
// Written by John Shepherd, March 2013

#ifndef BSTREE_H
#define BSTREE_H

// client view of a Tree
typedef struct TreeRep *Tree;

// kinds of insertion supported
typedef enum {
	InsertAtLeaf,
	InsertAtRoot,
	InsertRandom,
	InsertRebalance,
	InsertSplay,
	InsertAVL
} Style;

// Items and operations on Items
typedef int Key;
typedef Key Item; // item is just a key
#define key(it) (it)
#define cmp(k1,k2) ((k1) - (k2))
#define less(k1,k2) (cmp(k1,k2) < 0)
#define eq(k1,k2) (cmp(k1,k2) == 0)

// Interface Functions

// create an empty Tree
Tree newTree(Style);
// free memory associated with Tree
void dropTree(Tree);
// display a Tree + stats about Tree
void showTree(Tree);

// insert a new value into a Tree
void TreeInsert(Tree, Item);
// delete a value from a Tree
void TreeDelete(Tree, Key);
// check whether a value is in a Tree
int TreeFind(Tree, Key);
// compute depth of Tree
int TreeDepth(Tree);
// count #nodes in Tree
int TreeNumNodes(Tree);

#endif
