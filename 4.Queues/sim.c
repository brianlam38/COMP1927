// Bank Queue Simulation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Queue.h"

#define NQUEUES 3
#define MAX_Q_LEN 10

int main(int argc, char **argv)
{
	Queue q[NQUEUES];
	int i, n;

	// initialise Queues to empty
	for (i = 0; i < NQUEUES; i++) {
		q[i] = newQueue(MAX_Q_LEN);
		assert(q[i] != NULL);
	}

	// add your own tests here
}
