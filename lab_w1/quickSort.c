#include <stdio.h>
#include <stdlib.h>

void randomSort(int a[], int n);

int main(int argc, char *argv[]){
	randomSort();

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