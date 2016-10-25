// BSTree.h ... implementation of binary search tree ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
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

// show a BSTree
void showBSTree(BSTree t)
{
	void doShowTree(BSTree);
	doShowTree(t);
}

// compute depth of BSTree
int BSTreeDepth(BSTree t)
{
	if (t == NULL)
		return 0;
	else {
		int ldepth = 1+BSTreeDepth(t->left);
		int rdepth = 1+BSTreeDepth(t->right);
		return (ldepth > rdepth) ? ldepth : rdepth;
	}
}

// count #nodes in BSTree
int BSTreeNumNodes(BSTree t)
{
	if (t == NULL)
		return 0;
	else
		return 1 + BSTreeNumNodes(t->left)
		         + BSTreeNumNodes(t->right);
	return -1;
}

// check whether a value is in a BSTree (recursively)
int BSTreeFind(BSTree t, int v)
{
	if (t == NULL)
		return 0;
	else if (v < t->value)
		return BSTreeFind(t->left,v);
	else if (v > t->value)
		return BSTreeFind(t->right,v);
	else // v == t->value
		return 1;
}

// check whether a value is in a BSTree (iteratively)
int BSTreeFindI(BSTree t, int v)
{
	Link curr = t;
	while (curr != NULL) {
		if (v < curr->value)
			curr = curr->left;
		else if (v > curr->value)
			curr = curr->right;
		else // v == curr->value
			return 1;
	}
	return 0;
}

// insert a new value into a BSTree (recursively)
BSTree BSTreeInsert(BSTree t, int v)
{
	if (t == NULL)
		t = newBSTNode(v);
	else if (v < t->value)
		t->left = BSTreeInsert(t->left,v);
	else if (v > t->value)
		t->right = BSTreeInsert(t->right,v);
	return t;
}

// insert a new value into a BSTree (iteratively)
BSTree BSTreeInsertI(BSTree t, int v)
{
	if (t == NULL) return newBSTNode(v);
	Link curr = t, parent = NULL;
	while (curr != NULL) {
		if (v < curr->value)
			{ parent = curr; curr = curr->left; }
		else if (v > curr->value)
			{ parent = curr; curr = curr->right; }
		else
			return t;
	}
	Link new = newBSTNode(v);
	if (v < parent->value)
		parent->left = new;
	else
		parent->right = new;
	return t;
}


// generic traversal
void BSTreeTraverse(BSTree t,
                    Visit visit,
                    char *style)
{
	if (t == NULL) return;
	if (strcmp(style,"NLR") == 0) (*visit)(t);
	BSTreeTraverse(t->left, visit, style);
	if (strcmp(style,"LNR") == 0) (*visit)(t);
	BSTreeTraverse(t->right, visit, style);
	if (strcmp(style,"LRN") == 0) (*visit)(t);
}

// delete a value from a BSTree
BSTree BSTreeDelete(BSTree t, int v)
{
	//BSTree deleteRoot(BSTree);
	printf("Not yet implemented\n");
	return t;
}

// delete root of tree
BSTree deleteRoot(BSTree t)
{
	return NULL;
}

// Pretty tree printer

#include "show.c"
