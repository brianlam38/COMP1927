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
	Link new = malloc(sizeof(BSTNode));	// Allocate space for node
	assert(new != NULL);
	new->value = v;					    // Sets key value
	new->left = new->right = NULL;		// Sets LHS and RHS -> NULL
	return new;							// Return ptr to new node
}

// create a new empty BSTree
BSTree newBSTree()
{
	return NULL;
}

// free memory associated with BSTree
void dropBSTree(BSTree t)
{
	if (t == NULL) return;	// check tree exists
	dropBSTree(t->left);	// recursively free ptrs to LHS
	dropBSTree(t->right);	// recursively free ptrs to RHS
	free(t);				// free tree
}

// show a BSTree
void showBSTree(BSTree t)
{
	void doShowTree(BSTree);
	doShowTree(t);
}

// compute depth of BSTree (DEPTH = ROOT -> a LEAF)
int BSTreeDepth(BSTree t)
{
	if (t == NULL)	// empty tree, 0 depth
		return 0;
	else {
		int ldepth = 1+BSTreeDepth(t->left);	// Compute depth of LHS subtree
		int rdepth = 1+BSTreeDepth(t->right);	// Compute depth of RHS subtree
		return (ldepth > rdepth) ? ldepth : rdepth;	// Return depth of longer subtree
	}
}

// count #nodes in BSTree
int BSTreeNumNodes(BSTree t)
{
	if (t == NULL)	// empty tree base case
		return 0;
	else
		return 1 + BSTreeNumNodes(t->left)	// Total nodes = ROOT NODE
		         + BSTreeNumNodes(t->right);//             + all LHS nodes
	return -1;							    //             + all RHS nodes
}

// check whether value exists in BSTree (RECURSIVE)
int BSTreeFind(BSTree t, int v)
{
	if (t == NULL)						// base case (FALSE) + empty tree case
		return 0;
	else if (v < t->value)				// if v < node val, check LHS
		return BSTreeFind(t->left,v);
	else if (v > t->value)				// if v > node val, check RHS
		return BSTreeFind(t->right,v);
	else // v == t->value			    // v = node val, return TRUE
		return 1;
}

// check whether a value is in a BSTree (ITERATIVE)
int BSTreeFindI(BSTree t, int v)
{
	Link curr = t;					// point curr -> root node t
	while (curr != NULL) {			// while curr != end of tree
		if (v < curr->value)		// if v < node val, go LHS of node
			curr = curr->left;
		else if (v > curr->value)	// if v > node val, go RHS of node
			curr = curr->right;
		else // v == curr->value	// v = node val, return TRUE
			return 1;
	}
	return 0;						// otherwise, return FALSE
}									// if NULL is reached (tree end)

// insert a new value into a BSTree (RECURSIVE)
BSTree BSTreeInsert(BSTree t, int v)
{
	if (t == NULL)				// EMPTY TREE or END OF TREE:
		t = newBSTNode(v);			// create NEW NODE w/ value v
	else if (v < t->value)		// if v < node val, insert in LHS subtree
		t->left = BSTreeInsert(t->left,v);
	else if (v > t->value)		// if v > node val, insert in RHS subtree
		t->right = BSTreeInsert(t->right,v);

	return t;					// return ptr to NEW NODE
}

// insert a new value into a BSTree (ITERATIVE)
BSTree BSTreeInsertI(BSTree t, int v)
{
	if (t == NULL) return newBSTNode(v);	// EMPTY TREE, create ROOT NODE
	Link curr = t;
	Link parent = NULL;
	while (curr != NULL) {				/* PART 1: Loop until value found or tree ends */
		if (v < curr->value)				// if v < node val, insert in LHS subtree
			{ parent = curr; curr = curr->left; }	// keep track of parent node (prev)
		else if (v > curr->value)			// if v > node val, go to RHS subtree
			{ parent = curr; curr = curr->right; }	// keep track of parent node (prev)
		else
			return t;						// v = node val, return EXISTING NODE
	}
	Link new = newBSTNode(v);			/* PART 2: Tree ended, append to tree */
	if (v < parent->value)					// if v < parent val, insert node to LHS
		parent->left = new;
	else									// if v > parent val, insert node to RHS 
		parent->right = new;

	return t;								// return NEW NODE
}

/* GENERIC TRAVERSAL */
// Remember, visit is a FN that takes in an Item, does something
// to it and doesn't return anything. We're giving the BSTreeTraverse
// function a ptr to the visit function.
void BSTreeTraverse(BSTree t, void (*visit)(Item), char *style)
{
	if (t == NULL) return;	// base case, empty tree

	if (strcmp(style,"NLR") == 0) (*visit)(t->value); // deref, visit root node FIRST
	BSTreeTraverse(t->left, visit, style); // visit LHS
	if (strcmp(style,"LNR") == 0) (*visit)(t->value); // deref, visit root node MID
	BSTreeTraverse(t->right, visit, style); // visit RHS
	if (strcmp(style,"LRN") == 0) (*visit)(t->value); // deref, visit root node LAST
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
