#include <stdio.h>
#include <stdlib.h>

void ASCbubbleSort(int a[], int lo, int hi);	// Ascending bubbleSort
void DSCbubbleSort(int a[], int lo, int hi);	// Descending bubbleSort
void swap(int a[], int x, int y);

int main(int argc, char * argv[]) {

	int a[5] = {4,3,2,1,0};
	printf("Before DSC sort: %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
	DSCbubbleSort(a,0,4);
	printf("After DSC sort: %d %d %d %d %d\n\n", a[0], a[1], a[2], a[3], a[4]);

	int b[5] = {10,9,8,7,6};
	printf("Before ASC sort: %d %d %d %d %d\n", b[0], b[1], b[2], b[3], b[4]);
	ASCbubbleSort(b,0,4);
	printf("After ASC sort: %d %d %d %d %d", b[0], b[1], b[2], b[3], b[4]);

	return EXIT_SUCCESS;
}

/* DESCENDING (hi -> lo) */
void DSCbubbleSort(int a[], int lo, int hi) {
	// Iterate through list
	// Check adjacent elements
		// If a[i] > a[i+1] , SWAP
		// Else continue
	// If no swaps are made, list is sorted

	int i, j;
	int nswaps;
	for (i = lo; i < hi; i++) {					// Iterate through list
		nswaps = 0;
		for (j = hi; j > i; j--) {					// Loop downwards, until reach i
			if (a[j-1] > a[j]) swap(a,j,j-1);		// a[j-1] > a[j] , then swap
			nswaps++;									// increment nswaps
		}
		if (nswaps == 0) break;
	}
}

/* ASCENDING (lo -> hi) */
void ASCbubbleSort(int b[], int lo, int hi) {
	int i, j;
	int nswaps;

	for (i = lo; i < hi; i++) {
		nswaps = 0;
		for (j = lo; j <= hi-1; j++) {
			if (b[j] > b[j+1]) swap(b,j,j+1);
			nswaps++;
		}
		if (nswaps == 0) break;
	}
}

void swap(int a[], int x, int y) {
	int temp = a[x];
	a[x] = a[y];
	a[y] = temp;
}