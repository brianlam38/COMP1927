//Sorting Algorithms

//Bubble Sort
void bubbleSort(int items[], int n) {
	int i, j;
	for (i = n - 1; i > 0 ; i--){
			for (j = 1; j <= i; j++) {
			
			//comparison
			if (items[j - 1] > items[j]){
				swap(j, j - 1,items);
			}
		}
	}
}

//Bubble Sort w/ Early Exit
void bubbleSortEE(int items[], int n) {
	int i, j;
	int done = 0;
	
	for (i = n - 1; i > 0 && !done; i--){
		done = 1; // Assume sorted
		for (j = 1; j <= i; j++) {
			if (items[j - 1] > items[j]){
				swap(j, j - 1,items);
				done = 0;
			}
		}
	}
}

//Helper Swap
void swap (int i, int j, int items[]) {
	int store = i;
	items[i] = items[j];
	items[j] = store;
}

//Selection Sort (ARRAY)
void selectionSort(int items[], int n) {
	int i, j, min;
	for (i = 0; i < n - 1; i++) {
		min = i; // current minimum is first unsorted element
		
		// find index of minimum element
		for (j = i + 1; j < n; j++) {
			if (items[j] < items[min]) {
				min = j;
			}
		}
		// swap minimum element into place
		swap(i, min, items[]);
	}
}

//Insertion Sort (ARRAY) with shift
//There is an error on this slide!
void insertionSortSimple(int items[], int n) {
	int i, j;
	for (i = 1; i < n; i++) {
		for (j = i; j > 0 && items[j] < items[j-1]; j--){
			swap(j, j-1, items);
		}
	}
}

//Insertion Sory (ARRAY) with Sentinel and shift
void insertionSort(int items[], int n) {
	int i, j;
	
	// Move smallest element to the left
	for (i = n - 1; i > 0; i--) {
		if (items[i - 1] > items[i])
			swap(i, i - 1, items);
	}

	//Insertion sort
	for (i = 2; i < n; i++) { //i starts at 2 as i=0 sorted
		for (j = i; j > 0 && items[j] < items[j-1]; j--){
			swap(j, j-1, items);
		}
	}
	
}

//Shell Sort (3x+1)
void shellSort(int items[], int n) {
	int i, j, h;
	
	//the starting size of h is found.
	for (h = 1; h <= (n - 1)/9; h = (3 * h) + 1); //bound h so appropriate h is used
	
		for (; h > 0; h /= 3) {
		
		//Insertion sort when h = 1
		for (i = h; i < n; i++) {
			for (j = i; j > 0 && items[j] < items[j-h]; j-=h){
				swap(j, j-h, items);
			}
		}
}

//Merge sort
void mergesort (Item a[], int l, int r){
	int m = (l+r)/2;
	if (r <= l) {
		return;
	}
	
	mergesort (a, l, m);
	mergesort (a, m+1, r);
	merge (a, l, m, r);
}

//Merge Function
void merge(int a[], int l, int mid, int r) {
	int i, j, k, nitems = r-l+1;
	int *tmp = malloc(nitems*sizeof(int));
	i = l; j = mid+1; k = 0;
	while (i <= mid && j <= r) {
		if ( a[i] < a[j] ) {
			tmp[k++] = a[i++];
		} else{
			tmp[k++] = a[j++];
		}
	}
	
	while (i <= mid) tmp[k++] = a[i++];
	while (j <= r) tmp[k++] = a[j++];
	
	//copy back
	for (i = l, k = 0; i <= r; i++, k++)
		a[i] = tmp[k];
		
	free(tmp);
}

//Bottom Up Merge Sort
#define min(A,B) (A<B ? A : B)
int merge (int a[], int l, int m, int r);
void mergesortBU (int a[], int l, int r){
	int i, m, end;
	for (m = 1; m <= r-l; m = 2*m) {
		for (i = l; i <= r-m; i += 2*m) {
			end = min(i + 2*m – 1, r);
			merge (a, i, i+m-1, end);
		}
	}
}


//QuickSort
void quicksort (int a[], int l, int r){ int i;
	if (r <= l) {
		return;
	}
	i = partition (a, l, r);
	quicksort (a, l, i-1);
	quicksort (a, i+1, r);
}

//QuickSort partition
int partition (int a[], int l, int r) {
	int i = l-1;
	int j = r;
	int pivot = a[r]; //rightmost is pivot
	for (;;) {
		while ( a[++i] < pivot);
		while ( pivot < a[--j] && j != l);
		if (i >= j) {
			break;
		}
		swap(i,j,a);
	}
	//put pivot into place
	swap(i,r a);
	return i; //Index of the pivot
}