// Set.c ... Set ADT implementation
// Written by John Shepherd, August 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Bool.h"
#include "Set.h"

// concrete data structure
struct SetRep {
	...
};

// Local functions

// check whether Set looks plausible
int isValid(Set s)
{
}

// Interface functions

// create new empty set
Set newSet()
{
	return NULL;
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
	// assert(isValid(s));
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
