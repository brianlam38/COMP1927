// HashTable.h ... interface of HashTable library

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

typedef struct _hash_table_rep *HashTable;

// create an empty hash table with N slots
HashTable newHashTable(int);

// free memory for a hash table
void dropHashTable(HashTable);

// display a hash table on stdout
void showHashTable(HashTable);

// read values row-by-row from file into a HashTable
void insertHashTable(HashTable, int);

#endif
