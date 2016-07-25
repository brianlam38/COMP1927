// IntList.c - Lists of integers
// Written by John Shepherd, July 2008

#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "IntList.h"

// data structures representing IntList

struct IntListNode {
	int  data;  // value of this list item
	struct IntListNode *next;
	            // pointer to node containing next element
};

struct IntListRep {
	int  size;  // number of elements in list
	struct IntListNode *first;
	            // node containing first value
	struct IntListNode *last;
	            // node containing last value
};

// create a new empty IntList
IntList newIntList()
{
	struct IntListRep *L;

	L = malloc(sizeof (struct IntListRep));
	assert (L != NULL);
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

// free up all space associated with list
void freeIntList(IntList L)
{
	// does nothing ...
}

// display list as one integer per line on stdout
void showIntList(IntList L)
{
	IntListPrint(stdout, L);
}

// create an IntList by reading values from a file
// assume that the file is open for reading
IntList getIntList(FILE *inf)
{
	IntList L;
	int v;

	L = newIntList();
	while (fscanf(inf,"%d",&v) != EOF)
		IntListInsert(L,v);
	return L;
}

// create a new IntListNode with value v
// (this function is local to this ADT)
static struct IntListNode *newIntListNode(int v)
{
	struct IntListNode *n;

	n = malloc(sizeof (struct IntListNode));
	assert(n != NULL);
	n->data = v;
	n->next = NULL;
	return n;
}

// apppend one integer to the end of a list
void IntListInsert(IntList L, int v)
{
	struct IntListNode *n;

	assert(L != NULL);
	n = newIntListNode(v);
	if (L->first == NULL)
		L->first = L->last = n;
	else {
		L->last->next = n;
		L->last = n;
	}
	L->size++;
}

// insert an integer into correct place in a sorted list
void IntListInsertInOrder(IntList L, int v)
{
	// This is INCORRECT
	IntListInsert(L, v);
}

// delete first occurrence of v from a list
// if v does not occur in List, no effect
void IntListDelete(IntList L, int v)
{
	struct IntListNode *curr, *prev;

	assert(L != NULL);

	// find where v occurs in list
	prev = NULL; curr = L->first;
	while (curr != NULL && curr->data != v) {
		prev = curr;
		curr = curr->next;
	}
	// not found; give up
	if (curr == NULL) return;
	// unlink curr
	if (prev == NULL)
		L->first = curr->next;
	else
		prev->next = curr->next;
	if (L->last == curr)
		L->last = prev;
	L->size--;
	// remove curr
	free(curr);
}

// return number of elements in a list
int IntListLength(IntList L)
{
	assert(L != NULL);
	return L->size;
}

// make a physical copy of a list
// new list looks identical to original list
IntList IntListCopy(IntList L)
{
	struct IntListRep *Lnew;
	struct IntListNode *curr;

	Lnew = newIntList();
	for (curr = L->first; curr != NULL; curr = curr->next)
		IntListInsert(Lnew, curr->data);
	return Lnew;
}

// make a sorted physical copy of a list
IntList IntListSortedCopy(IntList L)
{
	struct IntListRep *Lnew;
	struct IntListNode *curr;

	Lnew = newIntList();
	for (curr = L->first; curr != NULL; curr = curr->next)
		IntListInsertInOrder(Lnew, curr->data);
	return Lnew;
}

// check whether a list is sorted in ascending order
// returns 0 if list is not sorted, returns non-zero if it is
int IntListIsSorted(IntList L)
{
	struct IntListNode *curr;

	assert(L != NULL);
	// trivial cases, 0 or 1 items
	if (L->size < 2)
		return 1;
	// scan list, looking for out-of-order pair
	for (curr = L->first; curr->next != NULL; curr = curr->next) {
		if (curr->next->data < curr->data)
			return 0;
	}
	// nothing out-of-order, must be sorted
	return 1;
}

// check sanity of an IntList (for debugging)
int IntListOK(IntList L)
{
	struct IntListNode *p;
	int count;

	if (L == NULL)
		return 1;
	if (L->size == 0)
		return (L->first == NULL && L->last == NULL);

	// scan to (but not past) last node
	count = 1; // at least one node
	for (p = L->first; p->next != NULL; p = p->next)
		count++;

	return (count == L->size && p == L->last);
}

// display list as one integer per line to a file
// assume that the file is open for writing
void IntListPrint(FILE *outf, IntList L)
{
	struct IntListNode *curr;

	assert(L != NULL);
	for (curr = L->first; curr != NULL; curr = curr->next)
		printf("%d\n", curr->data);
}
