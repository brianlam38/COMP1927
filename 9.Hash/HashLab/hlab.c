// hlab.c ... client for HashTable ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include "HashTable.h"

void usage();

int main(int argc, char *argv[])
{
	HashTable hashtab;
	int N=10, hash; char coll;
	char word[50];
	FILE *in;
	int nwords, nmatches;

	// collect command-line params
	if (argc < 2) usage();
	switch (argc) {
	case 2: N = atoi(argv[1]); hash = 0;
	        coll = 'L'; break;
	case 3: N = atoi(argv[1]); hash = atoi(argv[2]);
	        coll = 'L'; break;
	case 4: N = atoi(argv[1]); hash = atoi(argv[2]);
	        coll = argv[3][0]; break;
	default: usage(); break;
	}
	if (N < 100) usage();

	hashtab = newHashTable(N);

	nwords = 0;
	in = fopen("words","r");
	while (fgets(word,50,in) != NULL) {
		word[strlen(word)-1] = '\0';
		hashTableInsert(hashtab,word);
		nwords++;
	}
	fclose(in);

	nmatches = 0;
	in = fopen("words","r");
	while (fgets(word,50,in) != NULL) {
		word[strlen(word)-1] = '\0';
		if (hashTableSearch(hashtab,word))
			nmatches++;
	}
	fclose(in);

	if (nmatches != nwords)
		printf("We have a problem\n");
	else
		printf("OK\n");

	return 0;
}

void usage()
{
	fprintf(stderr, "Usage: hlab N HashFunc CollType\n");
	fprintf(stderr, "N>100, HashFunc = 0..3, CollType = C|D|L\n");
	exit(1);
}
