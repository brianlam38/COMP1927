// Set.c ... Set ADT implementation
// Written by John Shepherd, August 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Bool.h"
#include "Bits.h"
#include "Set.h"

// concrete data structure
struct SetRep {
	BitS bits;
};

// Local functions

// check whether Set looks plausible
int isValid(Set s)
{
	assert(s != NULL);
}

// Interface functions

// create new empty set
Set newSet()
{
	Set new = calloc(sizeof(struct SetRep));
	assert(new != NULL);
	return new;
}

// free memory used by set
void dropSet(Set s)
{
	assert(isValid(s));
	free(s);
}

// make a copy of a set
Set SetCopy(Set s)
{
	Set new = newSet();
	for (i = 0; i < NWORDS; i++)
		new->bits[i] = s->bits[i];
}

// add value into set
void SetInsert(Set s, int n)
{
	assert(0 <= n && n <= NBITS-1);
	setBit(s->bits, n);
}

// remove value from set
void SetDelete(Set s, int n)
{
	assert(0 <= n && n <= NBITS-1);
	unsetBit(s->bits, n);
}

// set membership test
int SetMember(Set s, int n)
{
	assert(0 <= n && n <= NBITS-1);
	return getBit(s->bits, n);
}

// union
Set SetUnion(Set s, Set t)
{
	Set new = newSet();
	BitUnion(s->bits, t->bits, new->bits);
	return new;
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
	int i, nbits = 0;
	for (i = 0; i < NBITS; i++) {
		if (getBit(s->bits,i)) nbits++;
	}
	return nbits;
}

// display set as {i1,i2,i3,...iN}
void showSet(Set s)
{
	// assert(isValid(s));
	printf("{");
	int i;
    for (i = 0; i < NBITS; i++) {
		if (i != 0) printf(",");
        if (getBit(s->bits,i)) print("%d");
    }
	printf("}");
}

// read+insert set values
void readSet(FILE *in, Set s)
{
	// assert(isReadable(in) && isValid(s));
}