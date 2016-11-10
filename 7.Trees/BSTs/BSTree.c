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

/* INSERT VALUE IN TREE (RECURSIVE) */
BSTree BSTreeInsert(BSTree t, int v)
{
	if (t == NULL)				// #1 Reach end of tree -> Add node
		t = newBSTNode(v);
	else if (v < t->value)		// #2 Insert in LHS subtree
		t->left = BSTreeInsert(t->left,v);
	else if (v > t->value)		// #3 Insert in RHS subtree
		t->right = BSTreeInsert(t->right,v);
	return t;					// Return ptr to tree
}

/* INSERT VALUE IN TREE (ITERATIVE) */
BSTree BSTreeInsertI(BSTree t, int v)
{
	if (t == NULL) return newBSTNode(v);	// #1 Empty tree, insert node

	Link curr = t;
	Link parent = NULL;
	while (curr != NULL) {				    // #2: Find value / end of tree
		if (v < curr->value)					// Go to LHS subtree
			{ parent = curr; curr = curr->left; }
		else if (v > curr->value)				// Go to RHS subtree
			{ parent = curr; curr = curr->right; }
		else
			return t;
	}

	Link new = newBSTNode(v);				// #3 Add node to end of tree
	if (v < parent->value)							// Add to LHS subtree
		parent->left = new;
	else											// Add to RHS subtree
		parent->right = new;

	return t;
}

/* GENERIC TRAVERSAL */
// void (*visit)(Item) = a function param that affects the node
void BSTreeTraverse(BSTree t, void (*visit)(Item), char *style)
{
	if (t == NULL) return;			// Base case, empty treee

	if (strcmp(style,"NLR") == 0)	// #1: PREFIX ORDER - root first
		(*visit)(t->value);
	BSTreeTraverse(t->left, visit, style);	// Recursive visit LHS

	if (strcmp(style,"LNR") == 0)	// #2: INFIX ORDER - root middle
		(*visit)(t->value);
	BSTreeTraverse(t->right, visit, style);	// Recursive visit RHS

	if (strcmp(style,"LRN") == 0)	// #3: POSTFIX ORDER - root last
		(*visit)(t->value);
}

/* PRACTISE LAB TEST */
// Given a tree with nodes and values, write a function map() that
// changes the values inside the nodes according to (*f)(int).
void map(Tree t, int(*f)(int)) {
	if (t != NULL) {			// While tree !end
		t->value = f(t->value); // Change value of node
		Map(t->left,f);			// Recursively traverse LHS + apply changes
		Map(t->right,f);		// Recursively traverse RHS + apply changes
	}
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
