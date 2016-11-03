// Matrix.h ... interface of Matrix library

#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct _matrix_rep *Matrix;

// create an empty NxM matrix
Matrix newMatrix(int, int);

// free memory for a matrix
void dropMatrix(Matrix);

// read values row-by-row from file into a Matrix
int readMatrix(FILE *, Matrix);

// display a matrix on stdout (row-by-row)
void showMatrix(Matrix);

// add two MxN matrices; return new MxN matrix
Matrix add(Matrix, Matrix);

// multiply MxN and NxP matrices; return new MxP matrix
Matrix multiply(Matrix, Matrix);

#endif
