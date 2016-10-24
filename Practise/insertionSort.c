#include <stdio.h>
#include <stdlib.h>

void insertionSort(int a[], int lo, int hi);
void swap(int a[], int x, int y);
void move(int a[], int i, int j);

int main(int argc, char *argv[]) {

	int a[5] = {5,4,3,2,1};

	printf("List before sort: %d %d %d %d %d\n\n", a[0], a[1], a[2], a[3], a[4]);

	insertionSort(a, 0, 4);

	printf("List after sort: %d %d %d %d %d", a[0], a[1], a[2], a[3], a[4]);



	return EXIT_SUCCESS;
}

void insertionSort(int a[], int lo, int hi) {

   	int i, j, min, val;
   	min = lo;

   	for (i = lo+1; i <= hi; i++)		   
      	if (a[i] < a[min]) min = i;    // Grab index with lowest val in array
    printf("PRE-SWAP: %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
    printf("a[lo] = %d\n", a[lo]);
    printf("a[min] = %d\n", a[min]);
   	swap(a, lo, min);					   // Set up 1st sorted elt = min val
    printf("a[lo] = %d\n", a[lo]);
    printf("a[min] = %d\n", a[min]);
    printf("PRE-SORT: %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
   	for (i = lo+2; i <= hi; i++) {
      	val = a[i];						   // Take next elt in UNSORTED list
      	for (j = i; val < a[j-1]; j--) // Loop from end of sorted --> start of sorted
         	move(a, j, j-1);			   // Shift values across per iteration
      	a[j] = val;						   // Insert into sorted part, preserving order
		printf("List after sort: %d %d %d %d %d\n", a[0], a[1], a[2], a[3], a[4]);
   	}
}

void swap(int a[], int x, int y) {
	int temp = a[x];
	a[x] = a[y];
	a[y] = temp;
}

void move(int a[], int i, int j) {
	a[i] = a[j];
}
