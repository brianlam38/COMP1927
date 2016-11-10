#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "BST.h"

struct _dlist{
  struct _node *first;
};

struct _node{
    int item;   
    struct _node * next;
    struct _node * prev;
};

struct treeNode {
    TreeItem item;
    treelink left;
    treelink right;
};

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
     if(t != NULL)
     printf("%d ",t->item);
}

void preorderTraversal (treelink tree, void (*visit) (treelink)) {
    if (tree == NULL) {
       return;
    }
    (*visit)(tree);
    preorderTraversal (tree->left,visit);
    preorderTraversal (tree->right,visit);
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



//################### Linked List stuff #######################
//new node creator
static dlink newDNode(int value){
  dlink new = malloc(sizeof(struct _node));
  new->item = value; //assign the value 
  new->next = NULL; // point next to null
  new->prev = NULL;
  return new;
}


void printDList (dlist l) {
  if(l->first == NULL){
      printf ("[X]\n");
      return;
  }
  dlink list = l->first;
  dlink curr = list;
  dlink last = NULL;
  while (curr != NULL) {
    if(curr->next == NULL){
      last = curr;
    }
    printf ("[%d]->", curr->item);
    curr = curr->next;
  }
  printf ("[X]\n");
  printf("Now in reverse\n");
    curr = last;
  // and now in reverse
  while (curr != NULL) {
    if(curr->prev == NULL){
      last = curr;
    }
    printf ("[%d]->", curr->item);
    curr = curr->prev;
  }
  printf ("[X]\n");
}

static void insert(dlist dl, int value){
    if(dl == NULL){
      printf("Woops, empty list detected. Goodbye!\n");
      abort();
    }

    dlink new = newDNode(value);
    
    if(dl->first == NULL){
      dl->first = new;
    }else{
      dl->first->prev = new;
      new->next = dl->first;
      dl->first = new;
    }

}

dlist createList(void){
  dlist new = malloc(sizeof(struct _dlist));
  new->first = NULL;
  return new;

}
//Convert a binary search tree to a sorted double-linked list.
void BSTtoDL(dlist sorted, treelink t){
    if(t == NULL){
        return ;
    } 

    BSTtoDL(sorted,t->right);
    insert(sorted,t->item);
    BSTtoDL(sorted, t->left);

}
