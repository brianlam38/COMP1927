#include <stdlib.h>
#include <stdio.h>
#include "set.h"

void randomString(char *);

int main(int argc, char **argv)
{
	int N = (argc < 2) ? 20 : atoi(argv[1]);
	if (N < 20) N = 20;
	Set s;
	s = newSet();
	int i;
	char x[50];
	for (i = 0; i < N; i++) {
		randomString(x);
		insertInto(s,x);
		printf("Insert %s\n",x);
		showSet(s);
	}
	return 0;
}

void randomString(char *buf)
{
	char *choices = "abcd efgh ijkl mnop qrst uvw xyz";
	int i, n;
	n = random()%49;
	for (i = 0; i < n; i++)
		*buf++ = choices[random()%32];
	*buf = '\0';
}
