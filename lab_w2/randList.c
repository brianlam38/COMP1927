// randList.c - generate a list of random integers
// Written by John Shepherd, July 2008
//

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
	int max, i;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s #values [seed]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	max = atoi(argv[1]);
	if (max < 1) {
		fprintf(stderr, "%s: too few values\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (max > 1000000) {
		fprintf(stderr, "%s: too many values\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 3) 
		srand(atoi(argv[2]));
	else
		srand(time(NULL)); // really random

	for (i = 0; i < max; i++)
		printf("%d\n",1+rand()%(max*10));

	return 0;
}
