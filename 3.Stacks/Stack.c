// Stack ADT interface

// ##########################
// STACK REP AS A LINKED LIST
// ##########################

// Items = char

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Stack.h"

typedef char Item;
typedef struct Node *Link;
typedef struct Node { Item value; Link next; } Node;
typedef struct StackRep { int nitems; Link items; } StackRep;

// Stack validity check
static int isValidStack(Stack s)
{
	if (s == NULL) return 0;		// empty stack, not valid
	if (s->nitems < 0) return 0;	// nitems < 0, not valid
	if (s->nitems > 0 && s->items == NULL) return 0;	// nitems = null, not valid
	Link cur; int nitems = 0;
	for (cur = s->items; cur != NULL; cur = cur->next)	// loop through stack
		nitems++;											// count items in stack
	if (nitems != s->nitems) return 0;					// if nitems doesn't match count, not valid

	return 1;	// passed call conditions, valid stack.
}

// add item onto stack
// Pre-condition: valid(Stack,s) // && !isFull(s)
// Post-condition: s' == it:s 	(s' = new value of stack at end of the FN)
void push(Stack s, Item it)
{
	assert(isValidStack(s));
	
	Link new = malloc(sizeof(Node));	// create new node
	new->value = it;					// set val of node
	new->next = s->items;				// add to front of list
	s->items = new;						// re-arrange ptr
	s->nitems++;						// increment nitems
}
 
// remove item from stack
// Pre: valid(Stack,s) && nItems(s) > 0 && s == x:ss
// Post: ret == x && s' == ss
Item pop(Stack s)
{
	assert(isValidStack(s));

	Link old = s->items;	// grab item using ptr (stack -> always remove front of list)
	s->items = old->next;	// skip over old item
	Item it = old->value;	// grab old item value
	s->nitems--;			// decrement nitems
	free(old);				// FREE OLD ITEM
	return (it);			// RETURN VAL OF OLD ITEM
}
 
// stack contains no items?
// Pre: valid(Stack,s)
// Post: ret == (nItems(s) == 0)
int isEmpty(Stack s)
{
	return (nItems(s) == 0);	// TRUE if nItems = 0;
}
 
// how many items in stack
int nItems(Stack s)
{
	return s->nitems;
}
 
// display stack on stdout
void show(Stack s)
{
	printf("top: ");
	Link cur;
	for (cur = s->items; cur != NULL; cur = cur->next)
		printf("%c ",cur->value);
	printf("\n");
}
 
// create new empty stack
// Pre: nothing
// Post valid(Stack,ret)
Stack newStack()
{
	Stack new = malloc(sizeof(struct StackRep)); // allocate mem for new stack
	assert(new != NULL);	// NULL check
	new->nitems = 0;		// set nitems = 0
	new->items = NULL;		// set head of stack = NULL
	return new;				// return ptr to new stack
}
 
// release stack data
void dropStack(Stack s)
{
	Link cur, next;	// next ptr needed as curr must be free'd before going curr->next
	for (cur = s->items; cur != NULL; cur = next) {	// remove all existing
		next = cur->next;							// nodes in stack
		free(cur);
	}
	free(s); // free stack
}
 
