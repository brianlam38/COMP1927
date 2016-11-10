// BSTree.h ... implementation of binary search tree ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "BSTree.h"

typedef struct BSTNode *Link;

typedef struct BSTNode {
	int  value;
	Link left, right;
} BSTNode;

// make a new node containing a value
static
Link newBSTNode(int v)
{
	Link new = malloc(sizeof(BSTNode));
	assert(new != NULL);
	new->value = v;
	new->left = new->right = NULL;
	return new;
}

// create a new empty BSTree
BSTree newBSTree()
{
	return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t)
{
	if (t == NULL) return;
	dropBSTree(t->left);
	dropBSTree(t->right);
	free(t);
}

// display a BSTree (sideways)
static
void doShowBSTree(BSTree t, int level)
{
	int i;
	if (t == NULL) return;
	doShowBSTree(t->right, level+1);
	for (i = 0; i < level; i++) printf("   ");
	printf("%d\n", t->value);
	doShowBSTree(t->left, level+1);
}
void showBSTree(BSTree t)
{
	doShowBSTree(t, 0);
}

// compute depth of BSTree
int BSTreeDepth(BSTree t)
{
	return 0;
}

// count #nodes in BSTree
int BSTreeNodes(BSTree t)
{
	return 0;
}

// check whether a value is in a BSTree
int BSTreeFind(BSTree t, int v)
{
	if (t == NULL)
		return 0;
	else if (v < t->value)
		return BSTreeFind(t->left, v);
	else if (v > t->value)
		return BSTreeFind(t->right, v);
	else // (v == t->value)
		return 1;
}

// insert a new value into a BSTree
BSTree BSTreeInsert(BSTree t, int v)
{
	if (t == NULL)
		return newBSTNode(v);
	else if (v < t->value)
		t->left = BSTreeInsert(t->left, v);
	else if (v > t->value)
		t->right = BSTreeInsert(t->right, v);
	else // (v == t->value)
		/* don't insert duplicates */;
	return t;
}

// delete root of tree
BSTree deleteRoot(BSTree t)
{
	// if no subtrees, tree empty after delete
	if (t->left == NULL && t->right == NULL) {
		free(t);
		return NULL;
	}
	// if only right subtree, make it the new root
	else if (t->left == NULL && t->right != NULL) {
		free(t);
		return t->right;
	}
	// if only left subtree, make it the new root
	else if (t->left != NULL && t->right == NULL) {
		free(t);
		return t->left;
	}
	// else (t->left != NULL && t->right != NULL)
	// so has two subtrees
	// - find inorder successor
	// - move its value to root
	// - delete inorder successor node
	Link parent = t;
	Link succ = t->right; // not null!
	while (succ->left != NULL) {
		parent = succ;
		succ = succ->left;
	}
	t->value = succ->value;
	free(succ);
	if (parent == t)
		parent->right = NULL;
	else
		parent->left = NULL;
	return t;
}

// delete a value from a BSTree
BSTree BSTreeDelete(BSTree t, int v)
{
	if (t == NULL)
		return NULL;
	else if (v < t->value)
		t->left = BSTreeDelete(t->left, v);
	else if (v > t->value)
		t->right = BSTreeDelete(t->right, v);
	else // (v == t->value)
		t = deleteRoot(t);
	return t;
}
