// useIntList.c - testing IntList data type

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "IntList.h"

int main(int argc, char *argv[])
{
	IntList myList, myOtherList;

	int verbose = (argc == 2 && strcmp(argv[1],"-v") == 0);

	myList = getIntList(stdin);
	if (verbose) {
		printf("Original:\n");
		showIntList(myList);
	}
	assert(IntListOK(myList));

	myOtherList = IntListSortedCopy(myList);
	if (verbose) printf("Sorted:\n");
	showIntList(myOtherList);
	assert(IntListOK(myOtherList));
	assert(IntListIsSorted(myOtherList));

	return 0;
}
