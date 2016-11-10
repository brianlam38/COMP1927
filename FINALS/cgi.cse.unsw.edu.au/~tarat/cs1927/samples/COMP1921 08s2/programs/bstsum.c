/*  =====================================================================
 *  (Rough) BST solution to Assignment 1 
 *
 *  Based on treeeg.c and treeops.c written by Alistair Moffat, 
 *  as examples for the book "Programming, Problem Solving, and Abstraction 
 *  with C", Pearson SprintPrint, Sydney, Australia, 2003.
 *
 *  See http://www.cs.mu.oz.au/~alistair/ppsaa/ for further information.
 *
 *  This version prepared January 29, 2003.
 *  ==================================================================
 */

/* Use a binary search tree to count words, and print
   a sorted list of words and their frequencies.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define MAXCHARS 20 /* Max chars per word */

typedef char word_t[MAXCHARS+1];
int getword(word_t, int);

typedef struct node node_t;

struct node {
	char *word;              /* distinct word */
	int freq;                /* frequency of word */
	node_t *left;            /* left subtree of node */
	node_t *right;           /* right subtree of node */
};

/* prototypes for the Binary Search Tree operations */
node_t *search_tree(node_t *tree, char *search_word);  
node_t *insert_in_order(node_t *tree, char *new_word);
void print_out_tree(node_t *tree);
void free_tree(node_t *tree);

//-------------------------------------------------------------------
// FUNCTION YOU NEED TO WRITE
int sum_tree(node_t *tree);
//-------------------------------------------------------------------

int
main(int argc, char **argv) {
	node_t *tree = NULL;
	node_t *locn;
	word_t oneword;

    /* insert words into tree and update frequency as they are found */
	while (getword(oneword, MAXCHARS) != EOF) {
		locn = search_tree(tree, oneword);
		if (!locn) {
			/* not in tree, so insert the new word */
			tree = insert_in_order(tree, oneword);
		} else {
			/* already in tree, so increment count */
			locn->freq += 1;
		}
	}

	/* now print all the words */
	print_out_tree(tree);

	// YOUR FUNCTION CALLED HERE ------------------------------------
	// compute and print out the sum of the frequencies of the
	// words in the tree
	//
	printf("\nTotal number of words processed: %d\n", sum_tree(tree));

	/* and release the tree */
	free_tree(tree);
	tree = NULL;

	return 0;
}


/* Extract a single word out of the standard input, but not
   more than limit characters. Argument array W must be
   limit+1 characters or bigger. */
int
getword(char *W, int limit) {
	int c;
	/* first, skip over any non alphabetics */
	while ((c=getchar())!=EOF && !isalpha(c)) {
		/* do nothing more */
	}
	if (c==EOF) {
		return EOF;
	}
	/* ok, first character of next word has been found */
	*W = tolower(c);
	W += 1;
	limit -= 1;
	while (limit>0 && (c=getchar())!=EOF && isalpha(c)) {
		/* another character to be stored */
		*W = tolower(c);
		W += 1;
		limit -= 1;
	}
	/* now close off the string */
	*W = '\0';
	return 0;
}

node_t
*search_tree(node_t *tree, char *search_word) {
/*
 * Search the nodes rooted at *tree for *search_word
 * return a pointer to the node found
 * 
 * Call search_tree recursively if search_word could be
 * in either the left or right subtree
 */

	int compare;

	if (tree == NULL) { // tree is empty
		return NULL;
	}
	else {
		if ((compare = strcmp(search_word, tree->word)) == 0) {
			return tree; // node found
		}
		else if (compare < 0) {
			return search_tree(tree->left, search_word);
		}
		else {
			return search_tree(tree->right, search_word);
		}
	}
}



node_t
*insert_in_order(node_t *tree, char *new_word) {
/*
 * Insert *new_word into tree rooted at *tree and
 * return pointer to updated tree
 * 
 * Call insert_in_order recursively if new_word should be
 * inserted either into the left or right subtree
 */

	if (tree == NULL) {
		tree = (node_t *)malloc(sizeof(node_t));
		assert(tree!=NULL);
		tree->word = (char *)malloc(strlen(new_word)+1);
		strcpy(tree->word, new_word);
		tree->freq = 1;
		tree->left = tree->right = NULL;
	}
	else if (strcmp(new_word, tree->word) < 0) {
		tree->left = insert_in_order(tree->left, new_word);
	}
	else {
		tree->right = insert_in_order(tree->right, new_word);
	}
	return tree;
}

void
print_out_tree(node_t *tree) {
/*
 * Print out the contents of each node in the tree
 * rooted at *tree
 * 
 * Perform an *in-order traversal* of the tree:
 * Call print_out_tree recursively to print out the left
 * subtree of *tree before tree->data and the right
 * subtree of *tree after tree->data
 */
	if (tree) {
		print_out_tree(tree->left);
		printf("%-20s%5d\n", tree->word, tree->freq);
		print_out_tree(tree->right);
	}

	return;
}

void
free_tree(node_t *tree) {
/*
 * Free the memory used by the nodes rooted at *tree.
 * 
 * Perform a *post-order traversal* of the tree:
 * Call free_tree recursively to free the left and right
 * subtrees of *tree before freeing the node *tree itself
 */

	if (tree) {
		free_tree(tree->left);
		free_tree(tree->right);
		free(tree->word);
		free(tree);
	}

	return;
}

int sum_tree(node_t *tree) {
/*
 * Return the sum of the frequencies stored in tree
 */

}
