// List manipulation ... not using ADT 
// Primarily to illustrate recursion

#include <stdlib.h>
#include <stdio.h>
#include "List.h"

// make a single node
static Link makeNode(int v)
{
	Link new = malloc(sizeof(Node));
	if (new == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	new->value = v;
	new->next = NULL;
	return new;
}

// append a node to a List
List append(List L, int v)
{
	if (L == NULL) {
		return (List)makeNode(v);
	}
	else {
		// tail(L) = append(tail(L), v);
		L->next = append(L->next, v);
		return L;
	}
}

// "map" a function across a list
void applyToList(List L, int (*f)(int))
{
	if (!empty(L)) {
		// head(L) = F(head(L))
		L->value = (*f)(L->value);
		applyToList(tail(L), f);
	}
}
