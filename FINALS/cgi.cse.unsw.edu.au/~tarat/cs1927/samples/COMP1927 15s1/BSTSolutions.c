#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "BST.h"



struct treeNode {
    TreeItem item; //int
    Treelink left; //struct treeNode *
    Treelink right;
};

static Treelink createNode(TreeItem item);

//PRACTICE QUESTIONS
//Some of these questions would only 
//be doable assuming the tree has int items.
/*
    7
   / \
  3   12
 /   /  \
1   8    13
 \
  2
*/
int sumEven(Treelink tree){
    if(tree == NULL){
        return 0;
    } else {
        if(tree->item %2 == 0){
            return tree->item + sumEven(tree->left)      
                              + sumEven(tree->right);
        } else {
            return sumEven(tree->left) + sumEven(tree->right);
        }
    }
}


Treelink getSmallest(Treelink tree){
    if(tree == NULL) return NULL;
    while(tree->left != NULL){
        tree = tree->left;
    }   
    return tree;
}

//An internal node has at least one non-null child
int numInternalNodes(Treelink tree){
    if(tree == NULL){
        return 0;
    } else {
        if(tree->left != NULL || tree->right != NULL){
             return 1 + numInternalNodes(tree->left) + numInternalNodes(tree->right); 
        } else {
            return 0;
            //return numIternal(tree->left) + numInternal(tree->right); 
        }
    }

}

//Does a path from the root to a leaf node
//(in this case a leaf node can be 
//considered to have at least one null child) 
//have the given sum?
//0 false
//1 true
/*
//      5
//    /    \
//   2     7
//    \   / \
//     3 6 10
*/
int hasPathSum(Treelink tree, int sum){
    if(tree == NULL){
        if(sum == 0) return 1;
        else return 0;
    } else {
        sum = sum - tree->item;
        return (hasPathSum(tree->left,sum) || hasPathSum(tree->right,sum));
    }
}
/*Double tree
//     2        
//   /   \
    2     3
   /     /  
// 1     3
   /
   1     
//
// would become
//        2
//      /  \
//     2    3
//    /    /
//   1    3
//  /
// 1
//
**/    
void doubleTree(Treelink tree){
   if(tree == NULL) return;
  
   
   Treelink newNode = createNode(tree->item);
   Treelink oldLeft = tree->left;
   tree->left = newNode;
   newNode->left = oldLeft;
   
   doubleTree(tree->oldLeft);
   doubleTree(tree->right);
   
}

//Delete and free nodes with items < min  or > max
//trimTree(tree,3,8)
/*      6
//    /   \
//   2     8
//    \   / \ 
//     4 7  9 
*/

/*       6
//      / \
//     4   8
//         /
//        7
*/
Treelink trimTree(Treelink tree, int min, int max){
    if(tree == NULL)return NULL;
    tree->left = trimTree(tree->left,min,max);
    tree->right = trimTree(tree->right,min,max);
    if(tree->item >= min && tree->item <= max){
        return tree;
    }
    Treelink newTree;
    if(tree->item < min){
        newTree = tree->right;
    } else {
        newTree = tree->left;
    }
    free(tree);
    return newTree;
}

//Local helper functions
static Treelink createNode(TreeItem item){
    Treelink n = malloc(sizeof(struct treeNode));   
    assert(n!=NULL);
    n->item = item;
    n->left = NULL;
    n->right = NULL;
    return n;
}


void destroyTree(Treelink tree){
    if( tree != NULL){
        destroyTree(tree->left);
        destroyTree(tree->right);
    
        free(tree);
    }
}


static int max(TreeItem i1, TreeItem i2){
    if(i1 >= i2)  
        return i1;
    return i2;
}

int height(Treelink t){
    if(t == NULL){
        return -1;
    } else {
        int leftHeight = height(t->left);
        int rightHeight = height(t->right);
        int maxHeight = max(leftHeight,rightHeight);
        return maxHeight + 1;

    }
}

int size(Treelink t){
    
    if(t == NULL){
        return 0;
    } else {
        return  1 + size(t->left) + size(t->right);
    }
   
}

//Interface functions

TreeItem getItem(Treelink node){
    assert(node != NULL);
    return node->item;
}

Treelink createTree(void){
    return NULL;
}




//returns the node of the element with item i
Treelink search(Treelink t, TreeItem i){
    Treelink result = NULL;
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

//returns the node of the element with item i
Treelink searchIterative(Treelink t, TreeItem i){
    Treelink curr = t;
    while(curr != NULL){
        if(curr->item == i){
            return curr;
        } else if ( i < curr->item){
            curr = curr->left;
        } else {
             curr = curr->right;
        }
    }
    return NULL;
}




void inorderTraversal (Treelink tree) {    
   if(tree != NULL){    
      inorderTraversal(tree->left);
      printf("%d ",tree->item);
      inorderTraversal(tree->right);
   }
}

void postorderTraversal (Treelink tree) {
    
   if(tree != NULL){    
      postorderTraversal(tree->left);      
      postorderTraversal(tree->right);
      printf("%d ",tree->item);
   }
}



void preorderTraversal (Treelink tree){
    
   if(tree != NULL){
       TreeItemShow(tree->item);
       //printf("%d ",tree->item);
      preorderTraversal(tree->left);     
      preorderTraversal(tree->right);
    
   }
}


//Returns the root of the tree
//inserts duplicates on theleft hand side of tree
Treelink insertRec (Treelink tree, TreeItem item) {
    if(tree == NULL){
        Treelink newNode = createNode(item);
        return newNode; //now the root of the tree
    } else {
        if(item <= tree->item){
            tree->left = insertRec(tree->left, item); //
        } else {
            tree->right = insertRec(tree->right, item);
        }
    }
    return tree;
}

//Returns the root of the tree
//inserts duplicates on theleft hand side of tree
Treelink insertIterative(Treelink tree, TreeItem key) {
    Treelink curr = tree;
    Treelink prev = NULL;
    Treelink newNode = createNode(key);
    while(curr != NULL){ 
        prev = curr;       
        if ( key <= curr->item){
            curr = curr->left;
        } else {
             curr = curr->right;
        }
    }
    if(prev == NULL){
       return newNode;
    } else if(key <= prev->item){
        prev->left = newNode;
    } else {
        prev->right = newNode;
    }
    return tree;
   
}




//Assume t1 is the left subtree and t2 the right subtree
static Treelink join(Treelink t1, Treelink t2){
    if(t1 == NULL && t2 == NULL){
        return NULL;
    } else if(t1 == NULL){
        return t2;
    } else if(t2 == NULL){
        return t1;
    } else {
        Treelink curr = t2;
        Treelink prev = NULL;
        while(curr->left != NULL){
            prev = curr;
            curr = curr->left;
        }
        if(prev != NULL){
            prev->left = curr->right;
            curr->right = t2;
        }
        curr->left = t1;
       
        return curr;
    }
}

Treelink delete(Treelink tree, TreeItem item){
    if(tree == NULL){
        //I did not find it so I could not delete it
        return tree;
    } else if ( item < tree->item){
        //Try the left side
        tree->left = delete(tree->left,item);
    } else if (item > tree->item){
        //Try the right side
        tree->right = delete(tree->right,item);
    } else {
        //I found it and I need to delete it
        Treelink leftsubtree = tree->left;
        Treelink rightsubtree = tree->right;
        free(tree);
        return join(leftsubtree,rightsubtree); 
    }
    return tree;
}

static
void doShowBSTree(Treelink t, int level)
{
        int i;
        if (t == NULL) return;
        doShowBSTree(t->right, level+1);
        for (i = 0; i < level; i++) printf("   ");
       
        printf("%d\n", t->item);
        doShowBSTree(t->left, level+1);
}

void showBSTree(Treelink t)
{
        doShowBSTree(t, 0);
}



