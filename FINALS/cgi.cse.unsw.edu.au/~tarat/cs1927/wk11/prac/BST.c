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
#include <limits.h>
#include "BST.h"

struct treeNode {
    TreeItem item;
    treelink left;
    treelink right;
};

// Write your functions here
// ----------------------------------------------------------------------
// q1
int isDegenerate(treelink t){
  return 0;
}

// q2
int isBST(treelink t){
  return 0;
}
// ----------------------------------------------------------------------

static treelink createNode(TreeItem item){
     treelink t = (treelink) malloc (sizeof (*t));
     t->item = item;
     t->left = NULL;
     t->right = NULL;
     return t;
}

TreeItem getItem(treelink t){
     assert(t != NULL);
     return t->item;
}

void printTreeNode (treelink t) {
     if(t != NULL){
         printf("%d ",t->item);
     }
}

void printTreeUtil(treelink t, int level){
  if(t==NULL) return;
  int i;
  for(i=0; i<level; i++) printf("  ");
  printf("[%d]\n", t->item);
  printTreeUtil(t->left, level+1);
  printTreeUtil(t->right, level+1);
}

void printTree(treelink t){
  printTreeUtil(t, 0);
}

treelink insertTreeNode (treelink tree, TreeItem item) {
  if (tree == NULL) {
      tree = createNode(item);
      tree->item = item;
      tree->left = NULL;
      tree->right = NULL; 
  } else if (tree->item < item) {
      tree->right = insertTreeNode (tree->right, item);
  } else {
      tree->left = insertTreeNode (tree->left, item);
  }
  return tree;
}

//returns the node of the element with item i
treelink search(treelink t, TreeItem i){
    treelink result = NULL;
    if (t == NULL) {
        result = NULL;
    } else if( i < t->item ){
        result = search(t->left,i); 
    } else if( i > t->item ){
        result = search(t->right,i);   
    } else {
        result = t;
    }    
    return result;
}

void removeTree(treelink t){
    if (t != NULL){
        removeTree(t->left);
        removeTree(t->right);
        free(t);
    }
}

void inOrderPrint(treelink t){
    if (t != NULL){
        inOrderPrint(t->left);
        printf("%d\n", t->item);
        inOrderPrint(t->right);
    }
    return;
}
