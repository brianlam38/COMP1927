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