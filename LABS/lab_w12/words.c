// words.c ... Build a table of words from a dictionary file
// Written by John Shepherd, August 2008
// Modified by John Shepherd, May 2013

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "Item.h"
#include "HashTable.h"

void usage(char *prog)
{
	fprintf(stderr, "Usage: %s FileName HashTabSize\n", prog);
	exit(1);
}

int main(int argc, char *argv[])
{
    char *fname;     // name of dictionary file
    FILE *wordf;     // handle for dictionary file
	Item word;       // current word from file
    int size = 7919; // default size of hash table
    HashTable htab;  // the hash table
	int nwords;      // # words read and stored
    int nfound;      // # words found during search tests

	// set up parameters
	switch (argc) {
	case 2: fname = argv[1]; break;
	case 3: fname = argv[1]; size = atoi(argv[2]); break;
    default: fname = NULL; usage(argv[0]); break;
	}
	
    // access the word file
	if (eq(fname,"-")) {
		wordf = stdin;
		printf("Reading words from stdin\n");
	}
	else {
    	wordf = fopen(fname,"r");
    	if (wordf == NULL) {
        	printf("Can't open %s\n",fname);
        	exit(1);
    	}
    	printf("Reading words from %s\n",fname);    
	}

	// build hash table, containing all words from file
	nwords = 0; nfound = 0;
    htab = newHashTable(size);
	while ((word = ItemGet(wordf)) != NULL) {
		if (eq(word,"")) { dropItem(word); continue; }
		HashTableInsert(htab,word);
		nwords++;
		if (HashTableSearch(htab,word) != NULL)
			nfound++;
	}

	// examine hash table
	HashTableStats(htab);

	// tests
	// warning: we are assuming that "!aaaaaa!" etc.
	// do not occur in the input; this is not guaranteed
	assert(nfound == nwords);
	assert(HashTableSearch(htab,"!aaaaaa!") == NULL) ;
	assert(HashTableSearch(htab,"!xxxxxx!") == NULL) ;
	assert(HashTableSearch(htab,"!yyyyyy!") == NULL) ;
	assert(HashTableSearch(htab,"!zzzzzz!") == NULL) ;
	printf("Testing completed OK\n");

	// clean up
	fclose(wordf);
    dropHashTable(htab);
    return 0;
}
