// Matrix.c ... implementation of Matrix library

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Matrix.h"

// Function declarations
int validCell(Matrix x, Matrix y, int i, int j);

typedef struct _matrix_rep {
   int nrows;
   int ncols;
   int **val;
} MatrixRep;


// create an empty MxN matrix
Matrix newMatrix(int M, int N)
{
   Matrix new;
   int i, j;
   new = malloc(sizeof(MatrixRep));
   assert(new != NULL);
   new->val = malloc(M*sizeof(int *));
   for (i = 0; i < M; i++) {
      new->val[i] = malloc(N*sizeof(int));
      assert(new->val[i] != NULL);
      for (j = 0; j < N; j++)
         new->val[i][j] = 0;
   }
   new->nrows = M;
   new->ncols = N;
   return new;
}

// free memory for a matrix
void dropMatrix(Matrix m)
{
   assert(m != NULL);
   int i;
   for (i = 0; i < m->nrows; i++)
      free(m->val[i]);
   free(m->val);
   free(m);
}

// read values row-by-row from file into a Matrix
// if enough values to fill Matrix, return 1
// if insufficient values, leave Matrix part-filled and return 0
int readMatrix(FILE *in, Matrix m)
{
   assert(m != NULL);
   int i, j, n = 0, val, nread;
   for (i = 0; i < m->nrows; i++) {
      for (j = 0; j < m->ncols; j++) {
         nread = fscanf(in, "%d", &val);
         if (nread != 1) break;
         m->val[i][j] = val;
         n++;
      }
      if (nread != 1) break;
   }
   if (n == m->nrows*m->ncols)
      return 1;
   else
      return 0;
}

// display a matrix on stdout (row-by-row)
void showMatrix(Matrix m)
{
   int i, j;
   assert(m != NULL && m->val != NULL);
   for (i = 0; i < m->nrows; i++) {
      printf("[");
      for (j = 0; j < m->ncols; j++) {
         printf(" %5d",m->val[i][j]);
      }
      printf(" ]\n");
   }
}

// add two MxN matrices; return new MxN matrix
// returns NULL if matrices are not compatible
Matrix add(Matrix x, Matrix y)
{
   // check validity/compatibility of matrices
   assert(x != NULL && x->val != NULL);
   assert(y != NULL && y->val != NULL);
   if (x->nrows != y->nrows || x->ncols != y->ncols) {
      printf("Incompatible matrices\n");
      return NULL;
   }
   // create output matrix of appropriate dimensions
   int i, j;
   Matrix z;
   z = newMatrix(x->nrows, x->ncols);
   // fill output matrix
   for (i = 0; i < x->nrows; i++) {
      for (j = 0; j < x->ncols; j++) {
         z->val[i][j] = x->val[i][j] + y->val[i][j];
      }
   }
   return z;
}

// multiply MxN and NxP matrices; return new MxP matrix
// returns NULL if matrices are not compatible
Matrix multiply(Matrix x, Matrix y)
{
   assert(x != NULL && x->val != NULL);
   assert(y != NULL && y->val != NULL);
   if (x->ncols != y->nrows) {
      printf("Incompatible matrices\n");
      return NULL;
   }
   // create output matrix of appropriate dimensions
   int i, j;
   Matrix z;
   z = newMatrix(x->nrows, y->ncols);
   // fill output matrix

   for (i = 0; i < z->nrows; i++) {
      for (j = 0; j < z->ncols; j++) {
         if (validCell(x,y,i,j))
            z->val[i][j] = x->val[i][j] * y->val[i][j];
         else
            z->val[i][j] = 0;
      }
   }
   return z;
}

// Checks if the cell exists
int validCell(Matrix x, Matrix y, int i, int j)
{
   if (i > x->nrows)
      return 0;
   else if (j > y->ncols)
      return 0;
   else
      return 1;
}


