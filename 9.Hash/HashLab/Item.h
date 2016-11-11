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
#define itemCopy(it) strdup((it))
#define itemDrop(it) free((it))
#define itemShow(it) printf("%s",(it))

#endif
