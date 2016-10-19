// List.c ... implementation of List ADT as linked-list
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

typedef struct ListNode {
	Item value;
	struct ListNode *next;
} ListNode;

typedef struct ListRep {
	ListNode *first;  // ptr to first node
	ListNode *last;   // ptr to last node
} ListRep;

// create new empty list
List newList()
{
	List L;
	L = malloc(sizeof(ListRep));
	assert(L != NULL);
	L->first = NULL;
	L->last = NULL;
	return L;
}

// free memory used by list
void dropList(List L)
{
	assert(L != NULL);
	ListNode *curr, *next;
	curr = L->first;
	while (curr != NULL) {
		next = curr->next;
		dropItem(curr->value);
		free(curr);
		curr = next;
	}
	free(L);
}

// display as [1,2,3,4...]
void showList(List L)
{
	assert(L != NULL);
	ListNode *curr = L->first;
	printf("[");
	while (curr != NULL) {
		ItemShow(curr->value);
		if (curr->next != NULL)
			printf(",");
	}
	printf("]");
}

// add item into list
// no check for duplicates
void ListInsert(List L, Item it)
{
	assert(L != NULL);
	ListNode *new = malloc(sizeof(ListNode));
	assert(new != NULL);
	new->value = it;
	new->next = NULL;
	if (L->last == NULL)
		L->first = L->last = new;
	else {
		L->last->next = new;
		L->last = new;
	}
}

// remove item(s)
// assumes no duplicates
void ListDelete(List L, Key k)
{
	assert(L != NULL);
	ListNode *prev, *curr;
	prev = NULL; curr = L->first;
	while (curr != NULL) {
		if (eq(k,key(curr->value)))
			break;
		prev = curr;
		curr = curr->next;
	}
	if (curr != NULL) {
		if (prev == NULL)
			L->first = curr->next;
		else
			prev->next = curr->next;
		free(curr);
		if (L->first == NULL)
			L->last = NULL;
	}
}

// return item with key
Item *ListSearch(List L, Key k)
{
	assert(L != NULL);
	ListNode *curr = L->first;
	while (curr != NULL) {
		if (eq(k,key(curr->value)))
			return &(curr->value);
		else
			curr = curr->next;
	}
	return NULL; // key not found
}

// # items in list
int ListLength(List L)
{
	int n = 0;
	ListNode *curr = L->first; 
	while (curr != NULL) {
		n++;
		curr = curr->next;
	}
	return n;
}
