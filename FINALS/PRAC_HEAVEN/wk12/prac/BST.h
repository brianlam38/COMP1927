#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int TreeItem;
typedef struct treeNode * treelink;
typedef struct _node * dlink;
typedef struct _dlist * dlist;
//Functions that are provided
treelink insertTreeNode (treelink tree, TreeItem item);
void preorderTraversal (treelink tree, void (*visit) (treelink));
void printTreeNode (treelink t);
int size(treelink t);
treelink search(treelink t, TreeItem i);
TreeItem getItem(treelink t);
int countLeaves(treelink tree);
treelink searchInsert(treelink t, TreeItem i); 

// delete a node containing the element 'it'
treelink delete (treelink t, TreeItem it );

// check if a tree is a BST- that is, it has the properties of a BST
int isBST (treelink t);

// check if a tree is balanced
int isBalanced(treelink t);
void printDList (dlist l);
dlist createList(void);
void BSTtoDL(dlist sorted, treelink t);
