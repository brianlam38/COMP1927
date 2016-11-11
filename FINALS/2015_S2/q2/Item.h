// Item.h ... definition of Items in Tries

#ifndef ITEM_H
#define ITEM_H

#define MAXKEY 20
#define MAXDATA 100

typedef char *Key_t;
typedef char *Data_t;

typedef struct Item {
   char key[MAXKEY];
   char data[MAXDATA];
} Item;


#define keyOf(it)  ((it).key)
#define dataOf(it) ((it).data)

#endif
