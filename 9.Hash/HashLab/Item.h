// Item.h ... interface to Items
// Written by John Shepherd, May 2013

#ifndef ITEM_H
#define ITEM_H

#include <string.h>

typedef char *Key;
typedef Key Item; // item is just a key
#define key(it) (it)
#define cmp(k1,k2) (strcmp((k1),(k2)))
#define less(k1,k2) (cmp(k1,k2) < 0)
#define eq(k1,k2) (cmp(k1,k2) == 0)
#define copyItem(it) strdup((it))
#define dropItem(it) free((it))
#define showItem(it) printf("%5.4s",(it))

#endif
