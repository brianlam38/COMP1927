// hlab.c ... client for HashTable ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include "HashTable.h"

void usage();

int main(int argc, char *argv[])
{
	HashTable mytab;
	int  N=11;
	char collType = 'L';
	char word[50];
	FILE *in;
	int  nwords, nmatches;

	// collect command-line params
	switch (argc) {
	case 1: N = 11; collType = 'L'; break;
	case 2: N = atoi(argv[1]); collType = 'L'; break;
	case 3: N = atoi(argv[1]); collType = argv[2][0]; break;
	default: usage(); break;
	}

	mytab = newHashTable(N,collType);
	showHashTable(mytab);

	nwords = 0;
	in = fopen("words","r");
	while (fgets(word,50,in) != NULL) {
		word[strlen(word)-1] = '\0';
		HashTableInsert(mytab,word);
		showHashTable(mytab);
		nwords++;
	}
	fclose(in);

	showHashTable(mytab);

	nmatches = 0;
	in = fopen("words","r");
	while (fgets(word,50,in) != NULL) {
		word[strlen(word)-1] = '\0';
		if (HashTableSearch(mytab,word))
			nmatches++;
	}
	fclose(in);

	if (nmatches != nwords)
		printf("We have a problem\n");
	else
		printf("Tests Passed\n");

	return 0;
}

void usage()
{
	fprintf(stderr, "Usage: hlab N CollType ");
	fprintf(stderr, "(where N>2, CollType = C|D|L)\n");
	exit(1);
}
