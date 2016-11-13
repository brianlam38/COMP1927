#define NoItem distinguished Item value

typedef enum { NO_ITEM, OCCUPIED, DELETED } State;    // Different states for Item Deletion

typedef struct HashTabRep {
   Item *items; // array of Items
   State *state; // state[i] gives status of items[i]
   int  nslots; // # elements in array
   int  nitems; // # items stored in array
} HashTabRep;

typedef HashTabRep *HashTable;


HashTable newHashTable(int N)
{
   HashTable new = malloc(sizeof(HashTabRep));
   Item *a = malloc(N*sizeof(Item));
   State *s = malloc(N*sizeof(State)); // Allocating an array of STATE VALUES parallel to ITEM VALUES
   assert(new!=NULL && a!=NULL);
   for (int i = 0; i < N; i++)         // No need to worry about No Item anymore, we just 
      s[i] = NO_ITEM;                  // initialise as the state values
   new->items = a; new->state = s;
   new->nitems = 0; new->nslots = N;
   return new;
}


void delete(HashTable ht, Key k)
{
   int N = ht->nslots;
   Item *a = ht->items;
   State *s = ht->state;
   int i, j, h = hash(k,N);
   for (j = 0; j < N; j++) {
      i = (h+j)%N;
      if (s[i] == NO_ITEM) return; // k not in table
      if (eq(k,key(a[i]))) break;
   }
   if (j == N) return;
   if (s[i] != DELETED) ht->nitems--;
   s[i] = DELETED;
#if 0
   // clean up probe path     // NO NEED TO CLEAN IF FLAGGING AS DELETED
   j = i+1;
   while (a[j] != NoItem) {
      Item it = a[j];
      a[j] = NoItem;
      ht->nitems--;
      insert(ht, it);
      j = (j+1)%N;
   }
#endif
}

void insert(HashTable ht, Item it)
{
   assert(ht->nitems < ht->nslots);
   int N = ht->nslots;
   Item *a = ht->items;
   Key k = key(it);
   int i, j, h = hash(k,N);
   for (j = 0; j < N; j++) {        // Scan along probing chain
      i = (h+j)%N;
      if (s[i] == NO_ITEM) break;   // NO_ITEM found, break
      if (eq(k,key(a[i]))) break;   // Item with same key, break
   }
   if (j == N) return; // unnecessary?
   if (s[i] == NO_ITEM) ht->nitems++;      // NO_ITEM -> Increment nitems
   else if (s[i] == DELETED) ht->nitems++; // DELETED -> Increment nitems
   a[i] = it;           // Insert / Replace item in array
   s[i] = OCCUPIED;     // Flag as occupied
}


Item *search(HashTable ht, Key k)
{
   int N = ht->nslots;
   Item *a = ht->items;
   State *s = ht->state;
   int i, j, h = hash(k,N);
   for (j = 0; j < N; j++) {
      i = (h+j)%N;
      if (s[i] == NO_ITEM) return NULL;
      if (eq(k,key(a[i]))) {              // Check if item has been marked as DELETED
         if (s[i] == DELETED)                // If DELETED -> return NULL
            return NULL;                     // Else !DELETED -> return ptr to item
         else
            return &(a[i]);
      }
   }
   return NULL;
}

