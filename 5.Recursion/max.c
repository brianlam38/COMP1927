#include <stdlib.h>
#include <stdio.h>

int max(int *, int);
int maxi(int *, int, int);
void showVec(int *, int, int);
void indent();

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr,"Usage: %s a b c d ...\n",argv[0]);
		exit(1);
	}

	int n, vec[100];

	for (n = 1; n < argc; n++)
		vec[n-1] = atoi(argv[n]);
	n--;

	printf("Find max of: ");
	showVec(vec,0,n-1);
	printf("\n");
	printf("Max = %d\n", max(vec,n));

	return 0;
}

int level = -1; // hideous global variable!

int max(int a[], int n)
{
	return maxi(a, 0, n-1);
}

int maxi(int a[], int lo, int hi)
{
	int max1, max2, mid, res;

	level++;
	indent(); printf("max of ");
	showVec(a,lo,hi); printf("\n");

	if (lo == hi)
		res = a[lo];
	else {
		mid = (lo+hi)/2;
		max1 = maxi(a, lo, mid);
		max2 = maxi(a, mid+1, hi);
		indent(); printf("compare %d > %d\n",max1,max2);
		res = (max1 > max2) ? max1 : max2;
	}

	indent(); printf("returns %d\n",res);
	level --;
	return res;
}

int fac(int n)
{
	int res;
	level++;
	indent(); printf("fac(%d)\n",n);
	if (n < 2) {
		res = 1;
	}
	else {
		res = n * fac(n-1);
	}
	indent(); printf("returns %d\n",res);
	level--;
	return res;
}

void indent()
{
	int i;
	for (i = 0; i < level; i++) printf("  ");
}

void showVec(int a[], int lo, int hi)
{
	int i;
	printf("[");
	for (i = lo; i <= hi; i++) {
		printf("%d", a[i]);
		if (i < hi) printf(",");
	}
	printf("]");
}
