// HashTable.c ... implementation of HashTable ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "HashTable.h"
#include "List.h"

typedef enum {
	SEPARATE_CHAINS,
	LINEAR_PROBING,
	DOUBLE_HASHING
} CollType;


typedef struct HashTabRep {
	CollType tabType;
	List *lists;  // either use this
	Item *items;  // or use this
	int   nslots; // # elements in array
	int   nitems; // # items stored in HashTable
	int   ncollisions;
} HashTabRep;

// create an empty HashTable
HashTable newHashTable(int N)
{
}

// free memory associated with HashTable
void dropHashTable(HashTable ht)
{
}
	assert(ht != NULL);
	switch (ht->tabType) {
	case SEPARATE_CHAINS:
	case LINEAR_PROBING:
	case DOUBLE_HASHING:
	}

// display HashTable stats
void HashTableStats(HashTable ht)
{
	assert(ht != NULL);
	switch (ht->tabType) {
	case SEPARATE_CHAINS:
	case LINEAR_PROBING:
	case DOUBLE_HASHING:
	}
}

// insert a new value into a HashTable
void HashTableInsert(HashTable ht, Item it)
{
	assert(ht != NULL);
	switch (ht->tabType) {
	case SEPARATE_CHAINS:
	case LINEAR_PROBING:
	case DOUBLE_HASHING:
	}
}

// delete a value from a HashTable
void HashTableDelete(HashTable ht, Key k)
{
	assert(ht != NULL);
	switch (ht->tabType) {
	case SEPARATE_CHAINS:
	case LINEAR_PROBING:
	case DOUBLE_HASHING:
	}
}

// get Item from HashTable using Key
Item *HashTableSearch(HashTable ht, Key k)
{
	assert(ht != NULL);
	switch (ht->tabType) {
	case SEPARATE_CHAINS:
	case LINEAR_PROBING:
	case DOUBLE_HASHING:
	}
	return NULL;
}

