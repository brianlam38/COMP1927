#include <stdlib.h>
#include <stdio.h>
#include "BST.h"
#include <assert.h>

//To compile please type
//gcc -Wall -Werror -O -o testBST BST.c testBST.c
//To run please type
//./testBST

void printStuff(Treelink tree);

   

int main(int argc, char * argv[]){
    Treelink t1 = createTree();
    t1 = insertIterative(t1,5);
    t1 = insertIterative(t1,2);
    t1 = insertIterative(t1,3);
    t1 = insertIterative(t1,7);
    t1 = insertIterative(t1,6);
    t1 = insertIterative(t1,10); 
   
    assert(hasPathSum(t1,7));
    assert(hasPathSum(t1,10));
    assert(!hasPathSum(t1,5));
   
    destroyTree(t1);
     
    return 0;
}


void printStuff(Treelink tree){
    
    printf("\nPlease turn head on side to the left to see the tree\n");
    showBSTree(tree);
    printf("\n");
    if(size(tree) > 0)
        printf("Root of tree is %d\n",getItem(tree));
    

    printf("\nheight is %d\n",height(tree));
    
    printf("size is %d\n",size(tree));

   
    printf("Pre order traversal: ");
    preorderTraversal(tree);
    printf("\nIn order traversal: ");
    inorderTraversal(tree);
   
    printf("\nPost order traversal: ");
    postorderTraversal(tree); 
    printf("\n");
    
}
