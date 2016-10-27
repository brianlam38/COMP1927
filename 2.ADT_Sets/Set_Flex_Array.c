// Set.c ... Set ADT implementation
// Written by John Shepherd, August 2015

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Bool.h"
#include "Set.h"

#define INIT_ELEMS 100

// concrete data structure
struct SetRep {
	int *elems;
	int nelems;
	int maxelems;
};

int isValid(Set s)
{
	if (s == NULL) return 0;
	if (s->nelems < 0) return 0;
	return 1;
}

// create new empty set
Set newSet()
{
	Set s;
	if ((s = malloc(sizeof(struct SetRep))) == NULL) {
		fprintf(stderr, "Insufficient memory for Set\n");
		exit(EXIT_FAILURE);
	}
	if ((s->elems = malloc(INIT_ELEMS*sizeof(int))) == NULL) {
		fprintf(stderr, "Insufficient memory for Set\n");
		exit(EXIT_FAILURE);
	}
	s->nelems = 0;
	s->maxelems = INIT_ELEMS;
	// assert(isValid(s));
	return s;
}

// free memory used by set
void dropSet(Set s)
{
	// assert(isValid(s));
	if (s == NULL) return;
	free(s->elems);
	free(s);
}

// make a copy of a set
Set SetCopy(Set s)
{
	// assert(isValid(s));
	Set new;
	new = newSet();
#if 1 
	int i;
	for (i = 0; i < s->nelems; i++)
		SetInsert(new, s->elems[i]);
#endif
#if 0
	new->nelems = s->nelems;
	int i;
	for (i = 0; i < s->nelems; i++)
		new->elems[i] = s->elems[i];
#endif
#if 0
	new->nelems = s->nelems;
	memcpy(new->elems, s->elems, s->nelems*sizeof(int));
#endif
	// assert(isValid(new));
	return new;
}

// add value into set
void SetInsert(Set s, int n)
{
	// assert(isValid(s));
	if (s->nelems == s->maxelems) {
		// set array full, make a bigger one
		s->maxelems = 133*s->maxelems/100;
		s->elems = realloc(s->elems,s->maxelems*sizeof(int));
		assert(s->elems != NULL);
	}
	int i;
	for (i = 0; i < s->nelems; i++)
		if (s->elems[i] == n) return;
	s->elems[s->nelems] = n;
	s->nelems++;
}

// remove value from set
void SetDelete(Set s, int n)
{
	// assert(isValid(s));
	int i;
	for (i = 0; i < s->nelems; i++)
		if (s->elems[i] == n) break;
	if (i < s->nelems) {
		// overwrite i'th element with last
		s->elems[i] = s->elems[s->nelems-1];
		s->nelems--;
	}
}

// set membership test
int SetMember(Set s, int n)
{
	// assert(isValid(s));
	int i;
	for (i = 0; i < s->nelems; i++)
		if (s->elems[i] == n) return TRUE;
	return FALSE;
}

// union
Set SetUnion(Set s, Set t)
{
	// assert(isValid(s) && isValid(t));
	int i;  Set new = newSet();
	for (i = 0; i < s->nelems; i++)
		new->elems[i] = s->elems[i];
	new->nelems = s->nelems;
	for (i = 0; i < t->nelems; i++)
		SetInsert(new,t->elems[i]);
	return new;
}

// intersection
Set SetIntersect(Set s, Set t)
{
	// assert(isValid(s) && isValid(t));
	int i;  Set new = newSet();
	for (i = 0; i < s->nelems; i++) {
		if (SetMember(t, s->elems[i]))
			SetInsert(new, s->elems[i]);
	}
	return new;
}

// cardinality (#elements)
int SetCard(Set s)
{
	// assert(isValid(s));
	return s->nelems;
}

// display set as {i1,i2,i3,...iN}
void showSet(Set s)
{
	// assert(isValid(s));
	printf("{");
	int i;
	for (i = 0; i < s->nelems; i++) {
		printf("%d", s->elems[i]);
		if (i < s->nelems-1) printf(",");
	}
	printf("}");
}

// read+insert set values
void readSet(FILE *in, Set s)
{
	// assert(isReadable(in) && isValid(s));
	int val;
	while (fscanf(in, "%d", &val) == 1)
		SetInsert(s, val);
}
