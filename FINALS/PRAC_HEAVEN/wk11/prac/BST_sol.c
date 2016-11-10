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
  if(t==NULL) return 1;
  if(t->left != NULL && t->right != NULL) return 0;
  return isDegenerate(t->left) && isDegenerate(t->right);
}

// q2
int isBSTUtil(treelink t, int min, int max){
  if(t==NULL) return 1;
  if(!(min <= t->item && t->item <= max)) return 0;
  if(t->left != NULL && t->left->item <= min) return 0;
  if(t->right != NULL && t->right->item >= max) return 0;
  return isBSTUtil(t->left, min, t->item) && isBSTUtil(t->right, t->item, max);
}

int isBST (treelink t){
  return isBSTUtil(t, INT_MIN, INT_MAX);
}
// ----------------------------------------------------------------------

static void printTreeUtil(treelink t, int level){
  if(t==NULL){
    return;
  }
  int i;
  for(i=0; i<level; i++) printf("  ");
  printf("└[%d]\n", t->item);
  printTreeUtil(t->left, level+1);
  printTreeUtil(t->right, level+1);
}

void printTree(treelink t){
  printTreeUtil(t,0);
}

// stole this code from a stackoverflow user
// doesnt work with big trees 
static int prettyPrintTreeUtil(treelink tree, int is_left, int offset, int depth, char s[20][255]){
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", tree->item);

    int left  = prettyPrintTreeUtil(tree->left,  1, offset,                depth + 1, s);
    int right = prettyPrintTreeUtil(tree->right, 0, offset + left + width, depth + 1, s);

    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {
        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';
        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';
    } else if (depth && !is_left) {
        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';
        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
    return left + width + right;
}

void prettyPrintTree(treelink tree){
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    prettyPrintTreeUtil(tree, 0, 0, 0, s);

    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}

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