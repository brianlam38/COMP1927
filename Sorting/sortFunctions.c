#include <stdio.h>
#include <stdlib.h>

void randomSort(int a[], int n);

int main(int argc, char *argv[]){
	randomSort();
	quickSort();

	return EXIT_SUCCESS;	
}

void randomSort(int a[], int n) {
	// choose two random elements
	// if out of order, swap
	// repeat until sorted

	int ntimes = 0;	// check how many iterations
	
	while (!isSorted(a,n)) {
		int i, j;
		i = rand()%n;
		j = rand()%n;
		if (i < j && a[i] > a[j]) {
			swap(a,i,j);
		} else if (i > j && a[j] > a[i]) {
			swap(a,i,j);
		}
		ntimes++;
	}
	fprintf(stderr,"#iterations:%d\n", ntimes);
}

// comparison function that takes two pointers
int cmp(void *a, void *b) {
	int x = *((int*)*a);
	int y = *((int*)*b);

	if (x > y) return 1;       // returns +ve
	else if (x < y) return -1; // returns -ve
	else return 0;			   // returns 0
}

void quickSort(int a[], int n) {
	qsort(a, b, sizeof(int), cmp);
}





