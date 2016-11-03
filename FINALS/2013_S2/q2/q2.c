// q2.c ... Question 2 in COMP1927 13s1 Final Exam
// Client program for Lists ... read a list and reverse it

#include <stdlib.h>
#include <stdio.h>
#include "List.h"

int main(int argc, char **argv)
{
	List L; // list of integers
	int val;  // value read from stdin
	

	// read array contents from stdin
	L = newList();
	while (fscanf(stdin,"%d",&val) == 1)
		ListAppend(L, val);

	// show list and reversed list
	int L1 = ListLength(L);
	printf("Original List: "); ListShow(L);
	ListReverse(L);
	printf("Reversed List: "); ListShow(L);
	printf("Length before: %d, Length after: %d\n", L1, ListLength(L));

	return 0;
}
