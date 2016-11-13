// Hash Table implementation from lecture

// Create a HashTableRep and array, fill array with NoItem
HashTable newHashTable(int N)
{
	HashTableRep *new = malloc(sizeof(HashTableRep));	// Alloc HashTable struct
	assert(new != NULL);
	new->items = malloc(N*sizeof(Item));				// Alloc ptr to array of Items
	assert(new->items != NULL);

	for (int i = 0; i < N; i++) {						// Init values = NoItem
		new->items[i] = NoItem;
	}
	new->nitems = 0;									// Init remaining struct data
	new->nslots = N;

	return new;
}

// ##########################################
// IDEALISED versions of HashTable operations
// ##########################################

// Search an item by key
Item *search(Hashtable ht, Key k)
{
	int i = hash(k, ht->nslots);	// Run hash fn, grab index
	return &(ht->items[i])	// Return ptr to item in index
}

Item *insert(Hashtable ht, Item it)
{
	int i = hash(key(it), ht->N);	// Grab item key value, compute index
	ht->items[i] = it;				// Store item in the location
}

void delete(Hashtable ht, Item it)
{
	int i = hash(k, ht->N);			// Run hash fn, grab index
	ht->items[i] = NoItem;			// Go to index and delete item
}

// ########################################
// More REALISTIC versions of HT operations
// ########################################

// Search an item by key
Item *search(HashTable ht, Ket k)
{
	int i = hash(k, ht->nslots);
	if (ht->items[i] == NoItem)		  // #1 Item doesn't exist
		return NULL;
	else if (key(ht->items[i] != k))  // #2 Duplicate key!!! (Key hashes to the same val)
		return NULL;
	else							  // #3 Item exists
		return &(ht->items[i]);

}

// Insert an item into HT
void *insert(Hashtable ht, Item it)
{
	int i = hash(key(it), ht->nslots);
	if (ht->item[i] == NoItem) {				// #1 Item doesn't exist
		ht->items[i] = it;
		ht->nitems++;
	} else if (key(ht->items[i] == key(it))) {	// #2 Item already exists. Replace old->new item
		ht->items[i] = it;
	} else {  // Collision (item w/ different key in location)
		// .. What to do?
	}
}




