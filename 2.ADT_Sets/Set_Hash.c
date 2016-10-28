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
	int htab[MAXHASH];
};

// ###############
// LOCAL FUNCTIONS
// ###############

// example hash function
static int hash(int value) {
	return value%MAXHASH;
}

// check whether Set looks plausible
static int isValid(Set s) {
	if (s == NULL) return 0;
	if (s->nelems < 0) return 0;
	if (s->nelems > 0) {
		int i;
		int nlists = 0;
		for (i = 0; i < MAXHASH; i++) {	// Scan through H.T looking
			if (s->htab[i] != NULL) {	// for a non empty list.
				nlists++;				// Makes sure things are being
			}							// stored correctly in the H.T
		}
		if (nlists == 0) {
			return 0;
		}
	}								
}

// ###################
// INTERFACE FUNCTIONS
// ###################

// create new empty set
Set newSet()
{
	Set new = malloc(sizeof(struct SetRep));
	assert (new != NULL);

	int i;
	for (i = 0; i < MAXHASH; i++) {	// Initialise table = NULL
		new->htab[i] = NULL;
	}
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

// add value into hash table (append to end of list)
void SetInsert(Set s, int n)
{
	// assert(isValid(s));
	int val = hash(n);			// get elt
	Link curr = s->htab[val];	// set curr = start of index
	Link new = malloc(sizeof(struct Node));
	assert(new != NULL);

	while (curr != NULL) {		// loop until end
		if (curr->value == n) {	// check for duplicate
			return;
		}
		curr = curr->next;
	}
	curr->next = new;			// add node to end
	new->value = n;				// add value to node
	new->next = NULL;

	s->nelems++;				// increment nelems
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
