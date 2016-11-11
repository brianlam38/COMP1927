HashTable expand(HashTable ht)
{
	int newnslots = ht->nslots*2;
	HashTable new = newHashTable(newnslots);
	
	for (int i = 0; i < ht->nslots; i++) {
		if (ht->items[i] != NoItem)
			insert(new, ht->items[i]);
	}

	disposeHashTable(ht);
	return new;
}
