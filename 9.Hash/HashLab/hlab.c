// hlab.c ... client for HashTable ADT
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include "HashTable.h"

void usage();

int main(int argc, char *argv[])
{
	HashTable mytab;
	int  N = 11;
	int  N2 = 3;
	char t = 'L';

	// collect command-line params
	switch (argc) {
	case 1: N = 11; t = 'L'; break;
	case 2: N = atoi(argv[1]); t = 'L'; break;
	case 3: N = atoi(argv[1]); t = argv[2][0]; break;
	case 4: N = atoi(argv[1]); t = 'D'; N2 = atoi(argv[3]); break;
	default: usage(); break;
	}

	mytab = newHashTable(N,t,N2);
	showHashTable(mytab);

	char line[20];  int noShow = 0;
	printf("\n> ");
	while (fgets(line,20,stdin) != NULL) {
		Item value = &line[2];
		char *c;
		for (c = value; *c != '\0' & *c != '\n'; c++) ;
		*c = '\0';
		switch (line[0]) {
		case 'n':
			dropHashTable(mytab);
			sscanf(&line[1],"%d %c %d",&N,&t,&N2);
			mytab = newHashTable(N,t,N2);
			break;
		case 'i':
			HashTableInsert(mytab,value);
			break;
		case 'd':
			HashTableDelete(mytab,key(value));
			break;
		case 'f':
			if (HashTableSearch(mytab,key(value)))
				printf("Found!\n");
			else
				printf("Not found\n");
			noShow = 1;
			break;
		case 'q':
			return 0;
			break;
		default:
			printf("i=insert, d=delete, f=find, q=quit\n");
			noShow = 1;
			break;
		}
		if (noShow)
			noShow = 0;
		else
			showHashTable(mytab);
		printf("\n> ");
	}

	return 0;
}

void usage()
{
	fprintf(stderr, "Usage: hlab N CollType ");
	fprintf(stderr, "(where N>2, CollType = C|D|L)\n");
	exit(1);
}
