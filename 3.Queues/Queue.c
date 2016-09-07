// Queue ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Queue.h"

// data structures

struct QueueRep {
	int size;     // # slots in queue
	int nitems;   // # Items in slots
	int head;     // index of first item
	int tail;     // index of last item
	Item *data;   // array of Items
};
 
// create new empty queue
Queue newQueue(int length)
{
	Queue new = malloc(sizeof(struct QueueRep));
	assert(new != NULL);
	new->size = length;
	new->nitems = 0;
	new->head = 0;
	new->tail = -1;
	new->data = malloc(length*sizeof(Item));
	assert(new->data != NULL);
	return new;
}
 
// release queue data
void dropQueue(Queue q)
{
	assert(q != NULL);
	free(q->data);
	free(q);
}
 
// add item onto queue
void enqueue(Queue q, Item it)
{
	assert(q->nitems < q->size);
	q->tail = (q->tail+1)%q->size;
	q->data[q->tail] = it;
	q->nitems++;
}
 
// remove item from queue
Item dequeue(Queue q)
{
	assert(q->nitems > 0);
	Item it = q->data[q->head];
	q->head = (q->head+1)%q->size;
	q->nitems--;
	return it;
}
 
// queue contains no items?
int isEmpty(Queue q)
{
	return (nItems(q) == 0);
}
 
// how many items in queue
int nItems(Queue q)
{
	assert(q != NULL);
	return q->nitems;
}
 
// display queue on stdout
void show(Queue q)
{
	int i;
	if (q->nitems == 0) {
		printf("<empty>\n");
		return;
	}
	for (i = q->head; i != q->tail; i = (i+1)%q->size)	
		printf("%d->",q->data[i]);
	printf("%d@%d\n",q->data[i],i);
}
