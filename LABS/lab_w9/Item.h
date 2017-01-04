// Item.h  ... definition for items in Queues
// Written by John Shepherd, March 2013

#ifndef ITEM_H
#define ITEM_H

#include "Graph.h"

typedef Vertex Item;

#define ItemCopy(i)     (i)
#define ItemEQ(i1,i2)   ((i1) == (i2))
#define ItemShow(i)     printf("%d",(i))

#endif

