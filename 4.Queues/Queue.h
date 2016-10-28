// Queue ADT interface

typedef int Item;
typedef struct QueueRep *Queue;

// add item onto queue
void enqueue(Queue q, Item it); 
// remove item from queue
Item dequeue(Queue q); 
// queue contains no items?
int isEmpty(Queue q); 
// how many items in queue
int nItems(Queue q); 
// display queue on stdout
void show(Queue q); 
// create new empty queue
Queue newQueue(int length); 
// release queue data
void dropQueue(Queue q); 
