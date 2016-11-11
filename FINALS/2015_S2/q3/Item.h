// Item.h  ... definition for items in Queues
// Written by John Shepherd, March 2013

#ifndef ITEM_H
#define ITEM_H

typedef int Item;

#define ItemCopy(i)     (i)
#define ItemEQ(i1,i2)   ((i1) == (i2))
#define ItemLT(i1,i2)   ((i1) < (i2))
#define ItemGT(i1,i2)   ((i1) > (i2))
#define ItemShow(i)     printf("%d",(i))

#endif

