/* Graph implementation for the Euler tour existence problem for connected graphs.
   An Euler tour exists if and only if the degree of every node is even.
   Care taken to calculate the degree of a node as the number of neighbours, not
   number of incident edges (the graph can have self edges).
*/
#include <stdio.h>

#define MAXSIZE 100

int read_graph(int connected[MAXSIZE][MAXSIZE]);
void print_graph(int size, int connected[MAXSIZE][MAXSIZE]);
int eulerexists(int size, int connected[MAXSIZE][MAXSIZE]);

int main(int argc, char **argv) {
    int exists, size, connected[MAXSIZE][MAXSIZE];

    size = read_graph(connected);
    // print_graph(size, connected); // used for debugging

    exists = eulerexists(size, connected);

    if (exists > 0) {
        printf("an euler tour exists\n");
    } else {
        printf("there is no euler tour\n");
    }
    return 0;
}

// read size of graph then rows and columns of adjacency matrix
int read_graph(int connected[MAXSIZE][MAXSIZE]) {
    int i, j, size;

    scanf("%d", &size);
    for (i = 0; i < size; i++) {
	for (j = 0; j < size; j++) {
   	    scanf("%d", &connected[i][j]);
	}
    }
    return size;
}

// print graph of connectivity
void print_graph(int size, int connected[MAXSIZE][MAXSIZE]) {
    int i, j;

    for (i = 0; i < size; i++) {
	for (j = 0; j < size; j++) {
	    if (connected[i][j] == 1) {
   	        printf("%d is connected to %d\n", i, j);
	    }
	    else printf("%d is not connected to %d\n", i, j);
	}
    }
}

// return 1 if an Euler tour exists, 0 if none exists
int eulerexists(int size, int connected[MAXSIZE][MAXSIZE]) {
}
