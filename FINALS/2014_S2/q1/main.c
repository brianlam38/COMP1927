// main.c ... driver program for Matrix type

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Matrix.h"

int main(int argc, char **argv)
{
   Matrix x, y, z;
   char   *op;
   Matrix makeMatrix(char *);

   if (argc < 4) {
      printf("Usage %s add|mul File1 File2\n",argv[0]);
      return EXIT_FAILURE;
   }

   // set up two matrices
   x = makeMatrix(argv[2]);
   if (x == NULL) {
      printf("Can't make X\n");
      return EXIT_FAILURE;
   }
   printf("Matrix X\n");
   showMatrix(x);
   y = makeMatrix(argv[3]);
   if (y == NULL) {
      printf("Can't make Y\n");
      return EXIT_FAILURE;
   }
   printf("Matrix Y\n");
   showMatrix(y);

   // perform the operation
   if (strcmp(argv[1],"add") == 0) {
      op = "add";
      z = add(x,y);
   }
   else if (strcmp(argv[1],"mul") == 0) {
      op = "multiply";
      z = multiply(x,y);
   }
   else {
      printf("Usage %s add|mul File1 File2\n",argv[0]);
      return EXIT_FAILURE;
   }

   // display result
   if (z == NULL) {
      printf("Couldn't %s X and Y\n",op);
      return EXIT_FAILURE;
   }
   else {
      printf("%s(X,Y)\n",op);
      showMatrix(z);
   }
   return EXIT_SUCCESS;
}

Matrix makeMatrix(char *file)
{
   FILE *in;
   if ((in = fopen(file,"r")) == NULL) {
      printf("Can't open %s\n", file);
      return NULL;
   }
   int nrows, ncols;
   fscanf(in,"%d %d", &nrows, &ncols);
   Matrix m;
   m = newMatrix(nrows,ncols);
   if (readMatrix(in,m) == 0) {
      printf("Not %dx%d values in %s",
                     nrows,ncols,file);
      dropMatrix(m);
      return NULL;
   }
   return m;
}
