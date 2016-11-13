// HashTable.c ... implementation of HashTable library

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"
#include "HashTable.h"

typedef struct _hash_table_rep {
   int nslots;    // size of chains[] array
   int nvals;     // # keys stored in table
   List *chains;  // array of hash chains
} HashTableRep;

// hash function (int -> [0..nslots-1])
static int hash(int val, int nslots)
{
   return (val%nslots);
}

// create an empty hash table
HashTable newHashTable(int N)
{
   HashTable new;
   int i;
   new = malloc(sizeof(HashTableRep));
   assert(new != NULL);
   new->nslots = N;
   new->nvals = 0;
   new->chains = malloc(N*sizeof(List));
   assert(new->chains != NULL);
   for (i = 0; i < N; i++)
      new->chains[i] = newList();
   return new;
}

// free memory for a hash table
void dropHashTable(HashTable ht)
{
   assert(ht != NULL);
   int i;
   for (i = 0; i < ht->nslots; i++)
      dropList(ht->chains[i]);
   free(ht->chains);
   free(ht);
}

// display a hash table on stdout
void showHashTable(HashTable ht)
{
   assert(ht != NULL);
   int i;
   for (i = 0; i < ht->nslots; i++) {
      printf("[%2d] ",i);
      showList(ht->chains[i]);
   }
}

// add a new value into a HashTable
void insertHashTable(HashTable ht, int val)
{
   void expand(HashTable);
   assert(ht != NULL);
   if (ht->nvals > 2*ht->nslots) expand(ht);
   int h = hash(val,ht->nslots);
   appendList(ht->chains[h],val);
   ht->nvals++;
}

// ##################
// SUGGESTED SOLUTION
// ##################

// double the number of slots/chains in a hash table
void expand(HashTable ht)
{
   assert(ht != NULL);
   int i, j;

   int newN = 2*ht->nslots;                        // #1 make new chains[] array, x2 size
   List *newChains = malloc(newN*sizeof(List));
   assert(newChains != NULL);

   for (i = 0; i < newN; i++)                      // #2 allocate + init new chains
      newChains[i] = newList();

   for (i = 0; i < ht->nslots; i++) {              // #3 re-hash values into new chains[]
      int n;
      int *values = valuesFromList(ht->chains[i], &n);
      for (j = 0; j < n; j++) {
         int h = hash(values[j],newN);
         appendList(newChains[h],values[j]);
      }
      free(values);
   }

   for (i = 0; i < ht->nslots; i++)                // #4 clean up old chains[]
      dropList(ht->chains[i]);
   free(ht->chains);

   ht->nslots = newN;                              // #5 update HashTable data
   ht->chains = newChains;
}

// ###############
// BRIANS SOLUTION
// ###############
/*
// double the number of slots/chains in a hash table
void expand(HashTable ht)
{
   // #1 Takes in HashTable ht
   // #2 Doubles number of slots
   // #3 Re-hashes existing keys to new slots (chains)

   int expand = 2 * ht->nslots;
   HashTable new = newHashTable(expand);
   assert(new != NULL);

   for (int i = 0; i < ht->nslots; i++) {
      int nvals = 0;
      int *values = valuesFromList(ht->chains[i], &nvals);
      for (int j = 0; j < nvals; j++) {
         insertHashTable(new, values[j]);
      }
   }

   ht = new;
   printf("UPDATED HASH TABLE\n");
   showHashTable(ht);
}
*/
