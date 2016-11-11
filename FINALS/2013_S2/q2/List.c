// List.c ... implementation of simple List ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

typedef struct Node *Link;

typedef struct Node {
	int  value;
	Link next;
} Node;

typedef struct ListRep {
	int  nnodes;
	Link first;
	Link last;
} ListRep;

// newList ... make new empty List
List newList()
{
	List new = malloc(sizeof(ListRep));
	assert(new != NULL);
	new->nnodes = 0;
	new->first = NULL;
	new->last = NULL;
	return new;
}

// ListShow ... display List as <a, b, c, d, ...z>
void ListShow(List L)
{
	assert(L != NULL);
	printf("[");
	Link curr = L->first;
	while (curr != NULL) {
		printf("%d",curr->value);
		if (curr->next != NULL) printf(", ");
		curr = curr->next;
	}
	printf("]\n");
}

// ListLength ... number of nodes in List
int ListLength(List L)
{
	assert(L != NULL);
	return (L->nnodes);
}

// ListAppend ... append a new value to List
void ListAppend(List L, int value)
{
	assert(L != NULL);
	Link new = malloc(sizeof(Node));
	assert(new != NULL);

	new->value = value;
	new->next = NULL;
	L->nnodes++;
	if (L->first == NULL)
		L->first = L->last = new;
	else {
		L->last->next = new;
		L->last = new;
	}
}

// ListReverse ... reverse a List
void ListReverse(List L)
{
	// Create temp list
	// Copy entire list over to temp list
	// Go through temp list and insert values into original list

	List temp = newList();
	Link curr = L->first;

	if (curr != NULL) {
		curr = curr->next;
		ListReverse(L);
	}
	ListAppend(temp,curr->value);

}

/*
Link newNode()
{
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->value = 0;
	new->next = NULL;
	return new;
}
*/

/*
// ListReverse ... reverse a List
void ListReverse(List L)
{

	// While curr != NULL
	// Start from node #2, insert at head.
	// For each node, insert at the head then continue
	// from where we left off.

	ListShow(L);

	Link curr = L->first;	// grab 1st node
	Link orig = curr;		// store original position
	Link old = NULL;		// store position of old head

	printf("Starting cases\n\n");
	if (curr == NULL) return;		// EMPTY LIST
	if (curr->next == NULL) return; // ONE NODE LIST

	curr = curr->next;		// start from node #2

	printf("Main cases\n\n");
	while (orig != NULL) {
		ListShow(L);
		orig = curr->next;		// grab curr node
		old = L->first;
		L->first->next = old->next;
		L->first = curr;	// insert curr at head
		curr->next = old;	// link new head to old head
		curr = orig;
	}
}
*/






