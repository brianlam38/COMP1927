/* Set ADT Exercises */
/*
## Duplicate Elimination ##

Write a program uniq() that
1. reads numbers from stdin, one per line
2. writes numbers to stdout, one per line
3. writes each distinct value once
(basically the function reads in a set of values and returns output with duplicates eliminated)
*/

#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

void uniq(int a[]);
int seenORnot(int seen[], int val);

int main(int argc, char * argv[]) {

	int a[4] = {1,1,2,3};
	printf("Original set: %d %d %d %d \n", a[0], a[1], a[2], a[3]);
	uniq(a);
	printf("New set: %d %d %d\n", a[0], a[1], a[2]);


	return EXIT_SUCCESS;
}

void uniq(int a[]) {

	// For each i
		// If i has been visited
		// Remove from set
		// Shift RHS values down 1 elt
	// Return array

	int i;
	int seen[4];

	for (i = 0; i < 4; i++) {
		seen[i] = a[i];
		if (seenORnot(seen,a[i]) == TRUE) {
			a[i] = a[i-1];
		}
	}
}

int seenORnot(int seen[], int val) {
	int i;
	for (i = 0; i < 4; i++) {
		if (seen[i] == val) {
			return TRUE;
		}
	}
	return FALSE;
}




