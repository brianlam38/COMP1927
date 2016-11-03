// List.h ... interface of List library

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct _list_rep *List;

// create an empty list
List newList();

// free memory for a list
void dropList(List);

// display a list on stdout
void showList(List);

// read values row-by-row from file into a List
void appendList(List, int);

// get values from list into array
// - array is dynamically allocated to be large enough
// - returns pointer to array as result of function
// - sets second paramter to hold size of array
int *valuesFromList(List, int *);

#endif
