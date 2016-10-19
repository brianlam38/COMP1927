// Item.h ... interface to Items
// Written by John Shepherd, May 2013

#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>
#include <string.h>

typedef char *Key;
typedef Key Item; // item is just a key

#define key(it) (it)

int cmp(Key, Key);
#define less(k1,k2) (cmp(k1,k2) < 0)
#define eq(k1,k2) (cmp(k1,k2) == 0)
Item newItem(char *);
void dropItem(Item);
Item ItemGet(FILE *);
void ItemShow(Item);

#endif
