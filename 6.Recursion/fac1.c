#include <stdlib.h>
#include <stdio.h>

int fac(int);
void indent();

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr,"Usage: %s N\n",argv[0]);
		exit(1);
	}

	int i = atoi(argv[1]);

	printf("%d! = %d\n", i, fac(i));

	return 0;
}

int level = -1; // hideous global variable!

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
