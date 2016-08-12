#include <stdio.h>
#include <stdlib.h>

void binarySearch(void) {
	lo = 0; hi = n-1;
	while (lo < hi) {
		int mid = (lo+hi)/2;
		int diff = compare(key,a[mid]);
		if (diff < 0) hi = mid-1;
		if (diff > 0) lo = mid+1;
		if (diff == 0) return 1;
	}
	if (hi < lo)
		return 0;
	else {lo == hi
		if (a[lo] == key)
			return 1;
		else
			return 0;
	}
}
