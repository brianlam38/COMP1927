// Reminder ...

typedef struct HashTabRep {
   Item *items; // array of Items
   int  nslots; // # elements in array  (was called N)
   int  nitems; // # items stored in array
} HashTabRep;
typedef HashTabRep *HashTable;


HashTable expand(HashTable ht)
{
}
