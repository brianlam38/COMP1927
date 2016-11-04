void insertionSort(int a[], int lo, int hi) {

	// Find lowest value in list. Set as 1st sorted elt.
	// Compare 1st unsorted elt with each elt @ sorted part
		// For each iteration, shift values up
	// If sorted value < unsorted value, insert unsorted into the slot
	// Repeat until last elt in list is compared

	int i, j, val;
	int min = lo;

	for (i = lo+1; i <= hi; i++) {
		if (a[i] < a[min])
			min = i;
	}
	swap(a,lo,min);

	for (i = lo+2; i <= hi; i++) {
		val = a[i];	// 1st elt @unsorted
		for (j = i; val < a[j-1]; j--) {
			move(a,j,j-1);
		}
		a[j] = val;
	}
}

void bubbleSort(int a[], int n) {

	// Loop through array
		// For each iteration:
			// if a[i] > a[i+1]
				// swap values
			// else
				// continue
	// when no swaps occur, sort is done.

	int i, j;
	int nswaps = 0;

	for (i = 0; i < n; i++) {
		nswaps = 0;
		for (j = n; j > 1; j--) {
			if (a[j] < a[j-1]) {
				swap(a,j,j-1);
				nswaps++;
			}
		}
		if (nswaps == 0) {
			break;
		}
	}
}

void swap(int a[], int x, int y) {
	int temp = a[x];
	a[x] = a[y];
	a[y] = temp;
}











