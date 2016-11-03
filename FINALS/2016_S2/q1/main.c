// q1.c ... load values into Tree and then "map" them

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "BSTree.h"

// Operations
int ident(int val) { return val; }
int decr(int val) { return val-1; }
int duble(int val) { return 2*val; }
int square(int val) { return val*val; }

int main(int argc, char *argv[])
{
   BSTree t;  // Tree to hold values
   char   k;  // next key from stdin
   int    n;  // next value from stdin
   int    op; // code for operation to apply

   // Determine which operation (from argv)
   op = (argc < 2) ? 0 : atoi(argv[1]);

   // Build tree from values in stdin
   t = newBSTree();
   while (scanf("%c %d\n",&k,&n) == 2)
      t = insert(t,k,n);
   printf("Original Tree:\n");
   showBSTree(t);

   // Apply specified operation to Tree

   switch (op) {
   case 1: map(t, decr); break;
   case 2: map(t, duble); break;
   case 3: map(t, square); break;
   default: map(t, ident); break;
   }

   // Display updated tree
   printf("\nAfter Mapping:\n");
   showBSTree(t);

   return 0;
}
