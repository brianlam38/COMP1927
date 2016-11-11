// q1.c ... Question 1 in COMP1927 13s1 Final Exam
// Find maximum value in an array via recursive divide-and-conquer

#include <stdlib.h>
#include <stdio.h>

#define MAXARRAY 1000
#define TRUE 1
#define FALSE 0

int less(int a, int b);
// Global variables
int i = 0;
int j = MAXARRAY - 1;
int maxL;	// store max LHS
int maxR;	// store max RHS

int main(int argc, char **argv)
{
	int array[MAXARRAY];  // array storage
	int n;  // count of elements in array
	int val;  // next value read from stdin
	int ArrayMax(int *, int);

	// read array contents from stdin
	n = 0;
	while (fscanf(stdin,"%d",&val) == 1)
		array[n++] = val;

	// display maximum value
	if (n > 0)
		printf("Max = %d\n", ArrayMax(array,n));
	else
		printf("Array is empty\n");

	return 0;
}

// find maximum value in a[0..n-1]
int ArrayMax(int a[], int n)
{
	int max(int *, int, int);

	return max(a, 0, n-1);
}

// recursive function to find maximum in a[lo..hi]
int max(int a[], int lo, int hi)
{
	// TODO

	// find the maximum value in the lower half of the array
	// find the maximum value in the upper half of the array
	// choose the larger of these two values

	int mid = (lo+hi)/2;	// get midpoint of array

	// Take a look at LHS
		// store value of i
		// traverse list
			// if value found > stored i
			// set stored value = new value
			// continue until list is traversed.
				// THE LARGEST INTEGER WILL REMAIN IN THE STORED VAR

	// Repeat steps for RHS

	printf("Start LHS recursion\n");
	if (i < mid) {			// #1 LHS part
		if (less(a[i],a[maxL]) == TRUE) {
			maxL = i;
			i++;
			max(a,lo,hi);
		} else {
			i++;
			max(a,lo,hi);
		}
	}

	printf("Start RHS recursion\n");
	if (j > mid) {			// #2 RHS part
		if (less(a[j],a[maxR]) == TRUE) {
			maxR = j;
			j--;
			max(a,lo,hi);
		} else {
			j--;
			max(a,lo,hi);
		}
	}

	printf("maxL & maxR found, choosing correct value\n");
	if (maxL >= maxR) {
		return maxL;
	} else {
		return maxR;
	}
}

int less(int a, int b) {
	if (a < b)
		return TRUE;
	else
		return FALSE;
}




