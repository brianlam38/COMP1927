/* Graph implementation for the celebrity problem.
*/
#include <stdio.h>

#define MAXSIZE 100
#define TRUE 1

int celebrity(int size, int knows[MAXSIZE][MAXSIZE]);

int main(int argc, char **argv)
{
	int i, j, c, size, knows[MAXSIZE][MAXSIZE];

	// read graph
	scanf("%d", &size);
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
   			scanf("%d", &knows[i][j]);
		}
	}

	// print graph for testing
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			if (knows[i][j] == 1)
   				printf("%d knows %d\n", i, j);
			else
				printf("%d does not know %d\n", i, j);
		}
	}

	// --------------------------------------------------------------
	// your function called here
	// --------------------------------------------------------------
	c = celebrity(size, knows);

	if (c >= 0)
		printf("%d is a celebrity\n", c);
	else
		printf("there is no celebrity\n");
	return 0;
}

/*
 * Return index of celebrity in adjacency matrix if one exists, else return -1
 */
int celebrity(int size, int knows[MAXSIZE][MAXSIZE])
{
	return -1; // implement your solution and modify this line
}
