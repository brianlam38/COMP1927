#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int Item;

typedef struct _node *link;

typedef struct _node {
  Item item;
  link next;
} node;

// pretty prints a list
void printList (link list);

// return the sum of all items in list
int sumListItems (link ls);

// return a list with items start (start+1) ... end
// empty list if start > end
link fromTo (int start, int end);

//frees all memory used in the list
void freeList(link list);

// zip two lists together
link zip(link a, link b);

// print a single node
void printLink(link l);

// create a new node
link newNode(Item it);




// THE FUNCTIONS YOU NEED TO IMPLEMENT
// q1
link nodeFromEnd(link l, int k);

// q2
link reverse(link l);
