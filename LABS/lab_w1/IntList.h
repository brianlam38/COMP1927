// IntList.h - Lists of integers (interface)
// Written by John Shepherd, July 2008

#ifndef INTLIST_H
#define INTLIST_H

#include <stdio.h>

// External view of IntList
// Implementation given in intList.c

typedef struct IntListRep *IntList;

// create a new empty IntList
IntList newIntList();

// free up all space associated with list
void freeIntList(IntList);

// create an IntList by reading values from a file
// assume that the file is open for reading
IntList getIntList(FILE *);

// display list as one integer per line on stdout
void showIntList(IntList);

// apppend one integer to the end of a list
void IntListInsert(IntList, int);

// insert an integer into correct place in a sorted list
void IntListInsertInOrder(IntList, int);

// delete first occurrence of v from a list
// if v does not occur in List, no effect
void IntListDelete(IntList, int);

// return number of elements in a list
int IntListLength(IntList);

// make a physical copy of a list
// new list looks identical to original list
IntList IntListCopy(IntList);

// make a sorted physical copy of a list
IntList IntListSortedCopy(IntList);

// check whether a list is sorted in ascending order
// returns 0 if list is not sorted, returns non-zero if it is
int IntListIsSorted(IntList);

// check sanity of an IntList
int IntListOK(IntList);

// display list as one integer per line to a file
// assume that the file is open for writing
void IntListPrint(FILE *, IntList);

#endif
