// Test Queue ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Queue.h"

int main(int argc, char **argv)
{
	Queue q;

	q = newQueue(10);	// Creates queue of len = 10
	show(q);			// Show queue values

	int i;
	for (i = 0; i < 40; i++) {	// Loop 20 iterations
		enqueue(q,i);			// Add to queue
		show(q);				// Show queue
		if (i%3 == 1) dequeue(q); // Dequeue every i%3=1 items
	}
	
	int n = dequeue(q);
	printf("n=%d\n",n);
	show(q);
}
