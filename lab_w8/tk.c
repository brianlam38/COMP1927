// tk.c ... test Stack of strings ADT
// Written by John Shepherd, September 2015

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

void randomString(char *);

int main(int argc, char **argv)
{
	int N = (argc < 2) ? 20 : atoi(argv[1]);
	if (N < 20) N = 20;
	Stack s = newStack();
	int i;
	char x[50];
	for (i = 0; i < N; i++) {
		if (random()%10 > 5) {
			if (!emptyStack(s)) {
				char *str = popFrom(s);
				printf("Remove %s\n",str);
				free(str);
			}
		}
		else {
			randomString(x);
			pushOnto(s,x);
			printf("Insert %s\n",x);
		}
		showStack(s);
	}
	return 0;
}

void randomString(char *buf)
{
	char *choices = "abcd efgh ijkl mnop qrst uvw xyz";
	int i, n;
	n = random()%48;
	*buf++ = 'A'+random()%26;
	for (i = 0; i < n; i++)
		*buf++ = choices[random()%32];
	*buf = '\0';
}
