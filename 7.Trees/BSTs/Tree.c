// Tree.h ... implementation of binary search tree ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Tree.h"

typedef struct node *Link;

typedef struct node {
	Item value;
	Link left, right;
} Node;

// make a new node containing a value
static
Link newNode(int v)
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->value = v;
	new->left = new->right = NULL;
	return new;
}

// create a new empty Tree
Tree newTree()
{
	return NULL;
}

// free memory associated with Tree
void dropTree(Tree t)
{
	if (t == NULL) return;
	dropTree(t->left);
	dropTree(t->right);
	free(t);
}

// display a Tree (sideways)
void showTree(Tree t)
{
	void doShowTree(Tree);
	doShowTree(t);
}

// compute depth of Tree
int depth(Tree t)
{
	if (t == NULL)
		return 0;
	else {
		int ld = depth(t->left);
		int rd = depth(t->right);
		return 1 + ((ld > rd)?ld:rd);
	}
}

// count #nodes in Tree
//
int nnodes(Tree t)
{
	if (t == NULL) return 0;
	return 1 + nnodes(t->left) + nnodes(t->right);
}

/* TREE INSERTION */
Tree insert(Tree t, Item it)
{
	if (t == NULL) return newNode(it);		// Empty tree
	int diff = cmp(key(it),key(t->value));	// Store cmp value
	if (diff == 0)							// NO DIFFERENCE
		t->value = it;							// Replace old -> new item
	else if (diff < 0)						// LESS THAN
		t->left = insert(t->left, it);			// Insert new item -> LHS
	else if (diff > 0)						// GREATER THAN
		t->right = insert(t->right, it);		// Insert new item -> RHS
	return t;
}

/* TREE INSERTION AT ROOT NODE */
// STEP 1: Insert new node as leaf in LHS or RHS subtree
// STEP 2: RotateR or RotateL until new node = MAIN ROOT
Tree insertAtRoot(Tree t, Item it)
{
   	if (t == NULL)							// #1: Empty tree
   		return newNode(it);

   	int diff = cmp(key(it), key(t->value));	// #2: Store cmp value

   	if (diff == 0) {						// #3: NO DIFFERENCE - no effect
    	t->value = it;							// Replace old -> new item
 
   	} else if (diff < 0) {					// #4: LESS THAN
    	t->left = insertAtRoot(t->left, it);	// Insert into LHS Subtree
    	//printf("rotateR(%d)\n",t->value);		// Move to MAIN ROOT via.
    	t = rotateR(t);							// rotateR(t)

   	} else if (diff > 0) {					// #5: GREATER THAN
    	t->right = insertAtRoot(t->right, it);	// Insert into RHS Subtree
      	//printf("rotateL(%d)\n",t->value);		// Move to MAIN ROOT via.
      	t = rotateL(t);							// rotateL(t)
   	}
   	return t;
}

Tree insertRandom(Tree t, Item it)
{
   if (t == NULL) return newNode(it);
   int chance = rand() % 3;
   if (chance == 0)
      t = insertAtRoot(t, it);
   else
      t = insert(t, it);
   return t;
}

// check whether a value is in a Tree
int find(Tree t, Key k)
{
	if (t == NULL) return 0;
	int res, diff = cmp(k,t->value);
	if (diff < 0)
		res = find(t->left, k);
	else if (diff > 0)
		res = find(t->right, k);
	else // (diff == 0)
		res = 1;
	return res;
}

// #######################
// DELETE ROOT FROM A TREE
// #######################

// delete a value from a Tree
Tree delete(Tree t, Key k)
{
	Tree deleteRoot(Tree);

	if (t == NULL)
		return NULL;
	int diff = cmp(k,t->value);
	if (diff == 0)
		t = deleteRoot(t);
	else if (diff < 0)
		t->left = delete(t->left, k);
	else if (diff > 0)
		t->right = delete(t->right, k);
	return t;
}

// delete root of tree
Tree deleteRoot(Tree t)
{
	Link newRoot;
	/* CASE #1: NO SUBTREE */
	if (t->left == NULL && t->right == NULL) {
		free(t);
		return NULL;
	}
	/* CASE #2: ONE SUBTREE -> RHS */
	else if (t->left == NULL && t->right != NULL) {
		newRoot = t->right;
		free(t);
		return newRoot;
	}
	/* CASE #3: ONE SUBTREE -> LHS */
	else if (t->left != NULL && t->right == NULL) {
		newRoot = t->left;
		free(t);
		return newRoot;
	/* CASE #4: TWO SUBTREES */
	} else {
		Link parent = t;
		Link succ = t->right; 		 // SUCC = RHS subtree
		while (succ->left != NULL) { // Traverse to SUCC node
			parent = succ;
			succ = succ->left;
		}
		int succVal = succ->value;	 // Grab SUCC value
		t = delete(t,succVal);		 // Delete SUCC node
		t->value = succVal;			 // Set ROOT value = SUCC value
		return t;
	}
}

/* ROTATE TREE RIGHT */
// n1 = node to rotateR
Link rotateR(Link n1)
{
   if (n1 == NULL) return n1;	// #1: Empty tree, no rotation
   Link n2 = n1->left;			// #2: Set up n2 ptr to n1->LHS node
   if (n2 == NULL) return n1;	// #3: n2 = empty, no rotation needed
   									// (No LHS node to take its place)
   n1->left = n2->right;		// #4: Re-arrange ptrs for rotation
   n2->right = n1;
   return n2;					// #5; Return new root node
}

/* ROTATE TREE LEFT */
// n2 = node to rotateL
Link rotateL(Link n2)
{
   if (n2 == NULL) return n2;
   Link n1 = n2->right;
   if (n1 == NULL) return n2;
   n2->right = n1->left;
   n1->left = n2;
   return n1;
}

// Moves i'th node to the `root
Tree partition(Tree t, int i)
{
   if (t == NULL) return NULL;
   assert(0 <= i && i < nnodes(t));

   int n = nnodes(t->left);
   if (i < n) {								// #1: i in LHS
      t->left = partition(t->left, i);			// Recursively rotateR
      t = rotateR(t);
   }
   if (i > n) {								// #2: i in RHS
      t->right = partition(t->right, i-n-1);	// Recursively rotateL
      t = rotateL(t);
   }
   return t;								// #3: Return root node
}

// Indexes a tree and selects item in key order
// The function returns a ptr to an item in the tree
Item *get_ith(Tree t, int i)
{
   	if (t == NULL) return NULL;
   	assert(0 <= i && i < nnodes(t));		// make sure i is valid

   	int n = nnodes(t->left); 	   			// #nodes in LHS of root
   	if (i < n)								// #1: i in LHS
   		return get_ith(t->left, i);
   	if (i > n)								// #2: i in RHS
   		return get_ith(t->right, i-n-1);		// Re-compute index (i-n-1)
   												// (i - #LHS - root node)
   	return &(t->value);
}	

Tree rebalance(Tree t)
{
    if (t == NULL) return NULL;	 // #1: Empty tree
    if (nnodes(t) < 2) return t; // #2: Not enough nodes to rebalance

    t = partition(t, nnodes(t)/2);	// #3: Move median to root

    t->left = rebalance(t->left);	// #4: Rebalance LHS and RHS
    t->right = rebalance(t->right);

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
asciinode *build_ascii_tree(Tree t);
void free_ascii_tree(asciinode *node);

#define MAX_HEIGHT 1000
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
void doShowTree(Tree t)
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

asciinode *build_ascii_tree_recursive(Tree t)
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
asciinode *build_ascii_tree(Tree t)
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
