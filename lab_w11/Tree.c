// Tree.h ... implementation of binary search tree ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Tree.h"

// Representation of Trees and Nodes

typedef struct Node *Link;

typedef struct Node {
	Item value;
	Link left, right;
	Tree within;  // which tree contains this Node
} Node;

typedef struct TreeRep {
	Link  root;
	Style insert;
	int   ncompares;
	int   nrotates;
} TreeRep;

// Forward references for private functions

static void doShowTree(Link);
static void drop(Link);
static Link newNode(Item);
static Link insert(Link,Item);
static Link insertAtRoot(Link,Item);
static Link insertRandom(Link,Item);
static Link insertRebalance(Link,Item);
static Link insertSplay(Link,Item);
static Link insertAVL(Link,Item);
static Link search(Link,Key);
static Link searchSplay(Link,Key,int *);
static Link delete(Link,Key);
static Link deleteRoot(Link);
static Link partition(Link,int);
static Link rebalance(Link);
static Link rotateL(Link);
static Link rotateR(Link);
static int depth(Link);
static int size(Link);

// used to hold current tree during insertion

Tree thisTree;

// Helper: make a new node containing a value
static Link newNode(Item v)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->value = v;
	new->left = new->right = NULL;
	new->within = thisTree;
	return new;
}

// Interface: create a new empty Tree
Tree newTree(Style ins)
{
	TreeRep *new = malloc(sizeof(TreeRep));
	assert(new != NULL);
	new->root = NULL;
	new->insert = ins;
	new->ncompares = 0;
	new->nrotates = 0;
	return new;
}

// Interface: free memory associated with Tree
void dropTree(Tree t)
{
	if (t == NULL) return;
	drop(t->root);
	free(t);
}

// Helper: recursive drop
static void drop(Link t)
{
	if (t == NULL) return;
	drop(t->left);
	drop(t->right);
	free(t);
}

// Interface: display a Tree
void showTree(Tree t)
{
	assert(t != NULL);
	printf("#nodes = %d,  depth = %d,  #compares = %d,  #rotates = %d\n",
	       size(t->root), depth(t->root), t->ncompares, t->nrotates);
	if (size(t->root) > 60)
		printf("Tree too large to display\n");
	else
		doShowTree(t->root);
}

// Interface: depth of Tree (max path length)
int TreeDepth(Tree t)
{
	return depth(t->root);
}

// Helper: recursive depth calculation
static int depth(Link t)
{
	if (t == NULL) return 0;
	int ldepth = depth(t->left);
	int rdepth = depth(t->right);
	return 1 + ((ldepth > rdepth) ? ldepth : rdepth);
}

// Interface: count #nodes in Tree
int TreeNumNodes(Tree t)
{
	return size(t->root);
}

// Helper: recursive node counter
static int size(Link t)
{
	if (t == NULL) return 0;
	return 1 + size(t->left) + size(t->right);
}

// Interface: insert a new value into a Tree
void TreeInsert(Tree t, Item it)
{
	thisTree = t;
	switch (t->insert) {
	case InsertAtLeaf:
		t->root = insert(t->root, it); break;
	case InsertAtRoot:
		t->root = insertAtRoot(t->root, it); break;
	case InsertRandom:
		t->root = insertRandom(t->root, it); break;
	case InsertRebalance:
		t->root = insertRebalance(t->root, it); break;
	case InsertSplay:
		t->root = insertSplay(t->root, it); break;
	case InsertAVL:
		t->root = insertAVL(t->root, it); break;
	}
	//printf("After inserting %d, tree is:\n",key(it));
	//showTree(t);
}

// Helpers: various styles of insert
Link insert(Link t, Item it)
{
	if (t == NULL) return newNode(it);
	int diff = cmp(key(it),key(t->value));
	if (diff == 0)
		t->value = it;
	else if (diff < 0)
		t->left = insert(t->left, it);
	else if (diff > 0)
		t->right = insert(t->right, it);
	return t;
}

Link insertAtRoot(Link t, Item it)
{ 
	if (t == NULL) return newNode(it);
	int diff = cmp(key(it), key(t->value));
	if (diff == 0)
		t->value = it;
	else if (diff < 0) {
		t->left = insertAtRoot(t->left, it);
		t = rotateR(t);
	}
	else if (diff > 0) {
		t->right = insertAtRoot(t->right, it);
		t = rotateL(t);
	}
	return t;
}

Link insertRandom(Link t, Item it)
{
	if (t == NULL) return newNode(it);
	int chance = rand()%2;
	if (chance != 0)
		t = insertAtRoot(t, it);
	else
		t = insert(t, it);
	return t;
}

Link insertRebalance(Link t, Item it)
{
	t = insert(t, it);
	float lsize = size(t->left), rsize = size(t->right);
	float ratio = lsize/rsize;
	if (ratio < 1.0) ratio = rsize/lsize;
	if (ratio > 1.1) t = rebalance(t);
	return t;
}

Link insertSplay(Link t, Item it)
{
	Key v = key(it);
	if (t == NULL) return newNode(it);
	int diff = cmp(v,key(t->value));
	if (diff == 0)
		t->value = it;
	else if (diff < 0) { 
		if (t->left == NULL) {
			t->left = newNode(it);
			return t;
		}
		if (less(v,key(t->left->value))) {
			t->left->left = insertSplay(t->left->left, it);
			t = rotateR(t); 
		} else {
			t->left->right = insertSplay(t->left->right, it);
			t->left = rotateL(t->left);
		}
		t = rotateR(t);
	}
	else if (diff > 0) {
		if (t->right == NULL) {
			t->right = newNode(it);
			return t;
		}
		if (less(key(t->right->value),v)) {
			t->right->right = insertSplay(t->right->right, it);
			t = rotateL(t);
		} else {
			t->right->left = insertSplay(t->right->left, it);
			t->right = rotateR(t->right);
		}
		t = rotateL(t);
	}
	return t;
}

static Link insertAVL(Link t, Item it)
{
	if (t == NULL) return newNode(it);
	int diff = cmp(key(it), key(t->value));
	if (diff == 0)
		t->value = it;
	else if (diff < 0)
		t->left = insertAVL(t->left, it);
	else if (diff > 0)
		t->right = insertAVL(t->right, it);
	int dL = depth(t->left);
	int dR = depth(t->right);
	if ((dL - dR) > 1) t = rotateR(t);
	else if ((dR - dL) > 1) t = rotateL(t);
	return t;
}

// Interface: check whether a value is in a Tree
int TreeFind(Tree t, Key k)
{
	Link res;  int found = 0;
	if (t->insert == InsertSplay) {
		t->root = searchSplay(t->root, k, &found);
		res = found ? t->root : NULL;
	}
	else
		res = search(t->root, k);
	return (res != NULL);
}

// Helpers: search functions to return Node containing key
static Link search(Link t, Key k)
{
	if (t == NULL) return NULL;
	Link res = NULL;
	int diff = cmp(k,t->value);
	if (diff == 0)
		res = t;
	else if (diff < 0)
		res = search(t->left, k);
	else if (diff > 0)
		res = search(t->right, k);
	return res;
}

static Link searchSplay(Link t, Key k, int *found)
{
	Link res;
	if (t == NULL) {
		// item not found
		*found = 0;
		res = NULL;  
	}
	else if (eq(key(t->value),k)) {
		*found = 1; // item found, store true  
		res =  t;  
	}
	else if (less(k,key(t->value))) {
		if (t->left == NULL){
			*found = 0;// item not found
			//res = rotateRight(t); 
			res = t;
		}
		else if (eq(key(t->left->value),k)) {
			*found = 1;
			res = rotateR(t);
		}
		else {
			if (less(k,key(t->left->value))) {
				// left-left
				t->left->left = searchSplay(t->left->left, k, found);
				t = rotateR(t);
			}
			else {
				// left-right
				t->left->right = searchSplay(t->left->right, k, found);
				t->left = rotateL(t->left);
			}
			res = rotateR(t);   
		}
	}
	else { // k > key(t->value)
		if (t->right == NULL) {
			*found = 0;// item not found
			//res = rotateLeft(t);
			res = t;
		}
		else if (eq(key(t->right->value),k)) {
			*found = 1;
			res = rotateL(t);
		}
		else{
			if (less(key(t->right->value),k)) {
				/* right-right */
				t->right->right = searchSplay(t->right->right, k, found);
				t = rotateL(t);   
			}
			else {
				/* right-left */
				t->right->left = searchSplay(t->right->left, k, found);
				t->right = rotateR(t->right);
			}
			res = rotateL(t);
		}
   }
   return res;
}

// Interface: delete a value from a Tree
void TreeDelete(Tree t, Key k)
{
	t->root = delete(t->root,k);
}

// Helper: recursive delete
static Link delete(Link t, Key k)
{
	if (t == NULL) return NULL;
	int diff = cmp(k,t->value);
	if (diff == 0)
		t = deleteRoot(t);
	else if (diff < 0)
		t->left = delete(t->left, k);
	else if (diff > 0)
		t->right = delete(t->right, k);
	return t;
}

// Helper: delete root of tree
static Link deleteRoot(Link t)
{
	Link newRoot;
	// if no subtrees, tree empty after delete
	if (t->left == NULL && t->right == NULL) {
		free(t);
		return NULL;
	}
	// if only right subtree, make it the new root
	else if (t->left == NULL && t->right != NULL) {
		newRoot = t->right;
		free(t);
		return newRoot;
	}
	// if only left subtree, make it the new root
	else if (t->left != NULL && t->right == NULL) {
		newRoot = t->left;
		free(t);
		return newRoot;
	}
	// else (t->left != NULL && t->right != NULL)
	// so has two subtrees
	// - find inorder successor (grab value)
	// - delete inorder successor node
	// - move its value to root
	Link succ = t->right; // not null!
	while (succ->left != NULL) {
		succ = succ->left;
	}
	int succVal = succ->value;
	t = delete(t,succVal);
	t->value = succVal;
	return t;
}

// Helper: rotate tree right around root
Link rotateR(Link n1)
{
	if (n1 == NULL) return NULL;
	Link n2 = n1->left;
	if (n2 == NULL) return n1;
	n1->left = n2->right;
	n2->right = n1;
	return n2;
}

// Helper: rotate tree left around root
Link rotateL(Link n2)
{
	if (n2 == NULL) return NULL;
	Link n1 = n2->right;
	if (n1 == NULL) return n2;
	n2->right = n1->left;
	n1->left = n2;
	return n1;
}

// Helper: rebalance tree by moving median to root
static Link rebalance(Link t)
{
	if (t == NULL) return NULL;
    if (size(t) < 2) return t;
    // put node with median key at root
    t = partition(t, size(t)/2);
    // then rebalance each subtree
    t->left = rebalance(t->left);
    t->right = rebalance(t->right);
    return t;
}

// Helper: move i'th element to root
static Link partition(Link t, int i)
{
   if (t == NULL) return NULL;
   assert(0 <= i && i < size(t));
   int n = size(t->left);
   if (i < n) {
      t->left = partition(t->left, i);
      t = rotateR(t);
   }
   if (i > n) {
      t->right = partition(t->right, i-n-1);
      t = rotateL(t);
   }
   return t;
}


// ASCII tree printer
// Courtesy: ponnada
// Via: http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

// data structures
typedef struct asciinode_struct asciinode;
struct asciinode_struct
{
	asciinode * left, * right;
	//length of the edge from this node to its children
	int edge_length;
	int height;
	int lablen;
	//-1=I am left, 0=I am root, 1=I am right
	int parent_dir;
	//max supported unit32 in dec, 10 digits max
	char label[11];
};

// functions
void print_level(asciinode *node, int x, int level);
void compute_edge_lengths(asciinode *node);
void compute_lprofile(asciinode *node, int x, int y);
void compute_rprofile(asciinode *node, int x, int y);
asciinode *build_ascii_tree(Link t);
void free_ascii_tree(asciinode *node);

#define MAX_HEIGHT 10000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

#define MIN(X,Y)  (((X) < (Y)) ? (X) : (Y))
#define MAX(X,Y)  (((X) > (Y)) ? (X) : (Y))

int gap = 3;  // gap between left and right nodes

//used for printing next node in the same level, 
//this is the x coordinate of the next char printed
int print_next;    

//prints ascii tree for given Tree structure
static void doShowTree(Link t)
{
	asciinode *proot;
	int xmin, i;
	if (t == NULL) return;
	proot = build_ascii_tree(t);
	compute_edge_lengths(proot);
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		lprofile[i] = INFINITY;
	compute_lprofile(proot, 0, 0);
	xmin = 0;
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		xmin = MIN(xmin, lprofile[i]);
	for (i = 0; i < proot->height; i++) {
		print_next = 0;
		print_level(proot, -xmin, i);
		printf("\n");
	}
	if (proot->height >= MAX_HEIGHT) {
		printf("(Tree is taller than %d; may be drawn incorrectly.)\n",
			MAX_HEIGHT);
	}
	free_ascii_tree(proot);
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *node, int x, int level)
{
	int i, isleft;
	if (node == NULL) return;
	isleft = (node->parent_dir == -1);
	if (level == 0)
	{
		for (i=0; i<(x-print_next-((node->lablen-isleft)/2)); i++)
			printf(" ");
		print_next += i;
		printf("%s", node->label);
		print_next += node->lablen;
	}
	else if (node->edge_length >= level)
	{
		if (node->left != NULL)
		{
		  for (i=0; i<(x-print_next-(level)); i++)
		  {
		    printf(" ");
		  }
		  print_next += i;
		  printf("/");
		  print_next++;
		}
		if (node->right != NULL)
		{
		  for (i=0; i<(x-print_next+(level)); i++)
		  {
		    printf(" ");
		  }
		  print_next += i;
		  printf("\\");
		  print_next++;
		}
	}
	else
	{
		print_level(node->left,
			    x-node->edge_length-1,
			    level-node->edge_length-1);
		print_level(node->right,
			    x+node->edge_length+1,
			    level-node->edge_length-1);
	}
}


//This function fills in the edge_length and
//height fields of the specified tree
void compute_edge_lengths(asciinode *node)
{
	int h, hmin, i, delta;
	if (node == NULL) return;
	compute_edge_lengths(node->left);
	compute_edge_lengths(node->right);

	/* first fill in the edge_length of node */
	if (node->right == NULL && node->left == NULL)
		node->edge_length = 0;
	else
	{
		if (node->left == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
				rprofile[i] = -INFINITY;
			compute_rprofile(node->left, 0, 0);
			hmin = node->left->height;
		}
		if (node->right == NULL)
			hmin = 0;
		else {
			for (i=0; i<node->right->height && i < MAX_HEIGHT; i++)
		    		lprofile[i] = INFINITY;
			compute_lprofile(node->right, 0, 0);
			hmin = MIN(node->right->height, hmin);
		}
		delta = 4;
		for (i=0; i<hmin; i++) {
			int w = gap+1+rprofile[i]-lprofile[i];
			delta = (delta > w) ? delta : w;
		}

		//If the node has two children of height 1, then we allow the
		//two leaves to be within 1, instead of 2
		if (((node->left != NULL && node->left->height == 1) ||
		      (node->right != NULL && node->right->height == 1))
				&& delta > 4)
			delta--;
		node->edge_length = ((delta+1)/2) - 1;
	}

	//now fill in the height of node
	h = 1;
	if (node->left != NULL)
		h = MAX(node->left->height + node->edge_length + 1, h);
	if (node->right != NULL)
		h = MAX(node->right->height + node->edge_length + 1, h);
	node->height = h;
}

asciinode *build_ascii_tree_recursive(Link t)
{
	asciinode * node;

	if (t == NULL) return NULL;
	node = malloc(sizeof(asciinode));
	node->left = build_ascii_tree_recursive(t->left);
	node->right = build_ascii_tree_recursive(t->right);
	if (node->left != NULL) node->left->parent_dir = -1;
	if (node->right != NULL) node->right->parent_dir = 1;
	sprintf(node->label, "%d", t->value);
	node->lablen = strlen(node->label);;
	return node;
}


//Copy the tree into the ascii node structre
asciinode *build_ascii_tree(Link t)
{
	asciinode *node;
	if (t == NULL) return NULL;
	node = build_ascii_tree_recursive(t);
	node->parent_dir = 0;
	return node;
}

//Free all the nodes of the given tree
void free_ascii_tree(asciinode *node)
{
	if (node == NULL) return;
	free_ascii_tree(node->left);
	free_ascii_tree(node->right);
	free(node);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *node, int x, int y)
{
	int i, isleft;
	if (node == NULL) return;
	isleft = (node->parent_dir == -1);
	lprofile[y] = MIN(lprofile[y], x-((node->lablen-isleft)/2));
	if (node->left != NULL) {
		for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
			lprofile[y+i] = MIN(lprofile[y+i], x-i);
	}
	compute_lprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
	compute_lprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

void compute_rprofile(asciinode *node, int x, int y)
{
	int i, notleft;
	if (node == NULL) return;
	notleft = (node->parent_dir != -1);
	rprofile[y] = MAX(rprofile[y], x+((node->lablen-notleft)/2));
	if (node->right != NULL) {
		for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++)
	  		rprofile[y+i] = MAX(rprofile[y+i], x+i);
	}
	compute_rprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
	compute_rprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}
