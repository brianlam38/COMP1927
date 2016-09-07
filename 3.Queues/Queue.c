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
	Queue new = malloc(sizeof(struct QueueRep)); // malloc space for queue representation
	assert(new != NULL);						 // check that new ptr is pointing to nothing
	new->size = length;						     // initialise size of queue
	new->nitems = 0;							 // initially empty
	new->head = 0;								 // location starting at 0
	new->tail = -1;							     // tail needs to start at -1 as we move tail +1 position in enqueue()
	new->data = malloc(length*sizeof(Item));	 // malloc space for array (storing queue data)
	assert(new->data != NULL);					 // check that data is pointing to nothing		
	return new;
}
 
// release queue data
void dropQueue(Queue q)
{
	assert(q != NULL);	// check that queue exists
	free(q->data);		// free up data
	free(q);			// free up rest of the structure
}
 
// add item onto queue
void enqueue(Queue q, Item it)
{
	assert(q->nitems < q->size);	// check queue isn't already full (otherwise can't enqueue)
	q->tail = (q->tail+1)%q->size;  // move tail onto next location
	q->data[q->tail] = it;			// copy new item into that particular location
	q->nitems++;					// increment num of items
}
 
// remove item from queue
Item dequeue(Queue q)
{
	assert(q->nitems > 0);			// check num of items in queue > 0
	Item it = q->data[q->head];		// grab head item (FIFO, so remove 1st item)
	q->head = (q->head+1)%q->size;	// advance head ptr (Advance head ptr by 1 position)
	q->nitems--;					// decrement item count
	return it;						// return dequeued item
}
 
// queue contains no items?
int isEmpty(Queue q)
{
	return (nItems(q) == 0);	// check there are no items in queue
}
 
// how many items in queue
int nItems(Queue q)
{
	assert(q != NULL);			// check queue is not empty
	return q->nitems;			// return number of items
}
 
// display queue on stdout
void show(Queue q)
{
	int i;
	if (q->nitems == 0) {		// queue is empty
		printf("<empty>\n");
		return;
	}
	for (i = q->head; i != q->tail; i = (i+1)%q->size)
	// increment: mod size of queue, so it will go back to index 0
		printf("%d->",q->data[i]);		// print out data
	// print out final data
	printf("%d@%d\n",q->data[i],i); 
}
