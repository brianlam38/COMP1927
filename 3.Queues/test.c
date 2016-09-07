// Test Queue ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Queue.h"

int main(int argc, char **argv)
{
	Queue q;

	q = newQueue(10);
	show(q);

	int i;
	for (i = 0; i < 20; i++) {
		enqueue(q,i);
		show(q);
		if (i%3 == 1) dequeue(q);
	}
	
	int n = dequeue(q);
	printf("n=%d\n",n);
	show(q);
}
