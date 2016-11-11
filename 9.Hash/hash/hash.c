#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char **argv)
{
	int hash(char *, int);
	assert(argc > 1);
	printf("hash(%s) = %d\n", argv[1], hash(argv[1],10000000));
	return 0;
}

#if 0
int hash(char *key, int N)
{
	int h = 0; char *c;
	for (c = key; *c != '\0'; c++) {
		h = h + *c;
	}
	return h%N;
}
#endif
#if 1
int hash(char *key, int N)
{
	int i = 0, h = 0; char *c;
	for (c = key; *c != '\0'; c++) {
		h = h + (++i * *c);
	}
	return h%N;
}
#endif
#if 0
int hash(char *key, int N)
{
	int i = 0, h = 0; char *c;
	for (c = key; *c != '\0'; c++) {
		i++;
		h = h + (*c * i);
	}
	return h;
}
#endif
#if 0
int hash(char *key, int N)
{
   int h = 0, a = 31415, b = 21783;
   char *c;
   for (c = key; *c != '\0'; c++) {
      a = a*b % (N-1);
      h = (a * h + *c) % N;
   }
   return h&0x7fffffff;
}
#endif
