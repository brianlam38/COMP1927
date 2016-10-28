// Set.c ... Set ADT implementation
// Written by John Shepherd, August 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Bool.h"
#include "Set.h"

#define MAXHASH 1009

// concrete data structure
typedef struct Node *Link;
typedef struct Node {
	int value;
	Link next;
};
struct SetRep {
	int nelems;
	Link htab[MAXHASH];
};

// Local functions

static int hash(int value)
{
	return value%MAXHASH;
}

// check whether Set looks plausible
static int isValid(Set s)
{
	if (s == NULL) return 0;
	if (s->nelems < 0) return 0;
	if (s->nelems > 0) {
		int i; int nlists = 0;
		for (i = 0; i < MAXHASH; i++)
			if (s->htab[i] != NULL) nlists++;
		if (nlists == 0) return 0;
	}
	return 1;
}

// Interface functions

// create new empty set
Set newSet()
{
	Set new = malloc(sizeof(struct SetRep));
	assert(new != NULL);
	int i;
	for (i = 0; i < MAXHASH; i++)
		new->htab[i] = NULL;
	new->nelems = 0;
	return new;
}

// free memory used by set
void dropSet(Set s)
{
	// assert(isValid(s));
}

// make a copy of a set
Set SetCopy(Set s)
{
	// assert(isValid(s));
	return NULL;
}

// add value into set
void SetInsert(Set s, int n)
{
	assert(isValid(s));
	int h = hash(n);
	Link cur = s->htab[h];
	while (cur != NULL) {
		if (cur->value == n)
			return;
		cur = cur->next;
	}
	Link new = malloc(sizeof(Node));
	assert(new != NULL);
	new->value = n;
	new->next = s->htab[h];
	s->htab[h] = new;
	s->nelems++;
}

// remove value from set
void SetDelete(Set s, int n)
{
	// assert(isValid(s));
}

// set membership test
int SetMember(Set s, int n)
{
	// assert(isValid(s));
	return 0;
}

// union
Set SetUnion(Set s, Set t)
{
	// assert(isValid(s) && isValid(t));
	return NULL;
}

// intersection
Set SetIntersect(Set s, Set t)
{
	// assert(isValid(s) && isValid(t));
	return NULL;
}

// cardinality (#elements)
int SetCard(Set s)
{
	// assert(isValid(s));
	return 0;
}

// display set as {i1,i2,i3,...iN}
void showSet(Set s)
{
	// assert(isValid(s));
	printf("{");
	// iterate through set and display elements
	printf("}");
}

// read+insert set values
void readSet(FILE *in, Set s)
{
	// assert(isReadable(in) && isValid(s));
}