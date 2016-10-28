// List printing

#include <stdlib.h>
#include <stdio.h>
#include "List.h"

// function prototypes
int length(List);
int lengthi(List);
int sum(List);
int sumi(List);
void putList(List);
void putListi(List);
void putListRev(List);
void putListRevi(List);
int doublify(int);
int printOneItem(int);

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr,"Usage: %s a b c d ...\n",argv[0]);
		exit(1);
	}

	int i;  List L = NULL;

	for (i = 1; i < argc; i++)
		L = append(L, atoi(argv[i]));

	//length
	printf("\nList length, recursive\n");
	printf("Length = %d\n", length(L));
	printf("\nList length, iterative\n");
	printf("Length = %d\n", lengthi(L));
	// assert(length(L) == lengthi(L)

	// sum
	printf("\nList sum, recursive\n");
	printf("Sum = %d\n", sum(L));
	printf("\nList sum, iterative\n");
	printf("Sum = %d\n", sumi(L));
	// assert(sum(L) == sumi(L)

	// print
	printf("\nPrint forward, recursive\n");
	putList(L); printf("\n");
	printf("\nPrint forward, iterative\n");
	putListi(L); printf("\n");
	printf("\nPrint backward, recursive\n");
	putListRev(L); printf("\n");
	printf("\nPrint backward, iterative\n");
	putListRevi(L); printf("\n");

#if 0
	// doublify
	printf("\nDouble all list values\n");
	applyToList(L, &doublify);
	putList(L); printf("\n");

	// print
	printf("\nPrint list via applyToList\n");
	applyToList(L, &printOneItem); printf("\n");
#endif

	return 0;
}


// list length (recursive)

int length(List L)
{
	if (empty(L))
		return 0;
	else
		return 1 + length(tail(L));
}

// list length (iterative)

int lengthi(List L)
{
	int count; Link curr;
	count = 0;
	for (curr = L; curr != NULL; curr = tail(curr))
		count++;
	return count;
}


// sum of list values (recursive)

int sum(List L)
{
	if (empty(L))
		return 0;
	else
		return head(L) + sum(tail(L));
}

// list length (iterative)

int sumi(List L)
{
	int sum; Link curr;
	sum = 0;
	for (curr = L; curr != NULL; curr = tail(curr))
		sum += head(curr);
	return sum;
}

// display list forward (recursive)

void putList(List L)
{
   	if (empty(L))							// CASE #1: Empty list (base case)
    	printf("\n");
    else if (empty(tail(L))) {				// CASE #2: 1 elt list
    	show(head(L));							// empty(tail(L)) means 1 elt
    	printf("\n");
    } else {								// CASE #3: More than 1 elt
    	show(head(L));							// Print head
    	printf(", ");							// Print space
    	putList(tail(L));						// Recursively print remaining vals
   	}
}

// display list forward (iterative)

void putListi(List L)
{
	Link curr;
	for (curr = L; curr != NULL; curr = tail(curr)) {
		show(head(curr));
		if (!empty(tail(curr))) printf(", ");
	}
}

// display list backward (recursive)

void putListRev(List L)
{
   if (empty(L))
      /*nothing to print*/;
   else {
      putListRev(tail(L));
      if (!empty(tail(L))) printf(" ");
      show(head(L));
   }
}

// display list backward (iterative)

void putListRevi(List L)
{
	printf("Not yet implemented");
}

// double a value
int doublify(int v)
{
	return 2*v;
}
int printOneItem(int v)
{
	printf("%d ",v);
	return v;
}
