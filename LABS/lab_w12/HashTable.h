// HashTable.h ... interface to HashTable ADT
// Written by John Shepherd, March 2013

#ifndef HASHTAB_H
#define HASHTAB_H

#include "Item.h"

typedef struct HashTabRep *HashTable;

// create an empty HashTable
HashTable newHashTable(int);
// free memory associated with HashTable
void dropHashTable(HashTable);
// display HashTable stats
void HashTableStats(HashTable);

// insert a new value into a HashTable
void HashTableInsert(HashTable, Item);
// delete a value from a HashTable
void HashTableDelete(HashTable, Key);
// get Item from HashTable using Key
Item *HashTableSearch(HashTable, Key);

#endif
