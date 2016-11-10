/*
 * Sample Prac Question
 * Read the question sheet first
 *
 * Any questions email riyasat.saber@student.unsw.edu.au
 * but please ask your tutor first
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int TreeItem;
typedef struct treeNode * treelink;

//Functions that are provided
treelink insertTreeNode (treelink tree, TreeItem item);
void printTreeNode (treelink t);
int size(treelink t);
treelink search(treelink t, TreeItem i);
TreeItem getItem(treelink t);
void removeTree(treelink t);
void inOrderPrint(treelink t);
void printTreeUtil(treelink t, int level);
void printTree(treelink t);

// Functions you need to implement
int isDegenerate(treelink t); 	// q1
int isBST(treelink t);			// q2
