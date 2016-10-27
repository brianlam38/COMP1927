// Set.h ... interface to Set ADT
// Written by John Shepherd, March 2013
// Modified by John Shepherd, August 2015

#ifndef SET_H
#define SET_H

#include <stdio.h>
#include "Bool.h"

typedef struct SetRep *Set;

// create new empty set
Set newSet();

// free memory used by set
void dropSet(Set);

// make a copy of a set
Set SetCopy(Set);

// add value into set
void SetInsert(Set,int);

// remove value from set
void SetDelete(Set,int);

// set membership test
int SetMember(Set,int);

// union
Set SetUnion(Set,Set);

// intersection
Set SetIntersect(Set,Set);

// cardinality (#elements)
int SetCard(Set);

// display set as {i1,i2,i3,...iN}
void showSet(Set);

// read+insert set values from File
void readSet(FILE *, Set);

#endif
