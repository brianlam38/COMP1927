// mkwords.c ... Generate random "words"
// Written by John Shepherd, October 2015

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void usage(char *prog)
{
	fprintf(stderr, "Usage: %s #words Seed\n", prog);
	exit(1);
}

int main(int argc, char *argv[])
{
	int nwords;    // # words to generate
    int seed;      // seed for rand # generator

	// set up parameters
	switch (argc) {
	case 2: nwords = atoi(argv[1]); seed = 42; break;
	case 3: nwords = atoi(argv[1]); seed = atoi(argv[2]); break;
    default: fprintf(stderr, "Usage: %s #words Seed\n", argv[0]); return 1;
	}
	srand(seed);

	// array of letters, to simulate distribution of letters in English
	char *letters = "aaaabccdeeeeeefghhiiijkllmmnnoooopqrrssttuuvwxyz";
	int nletters = strlen(letters);

	// generate some words
	// each "word" consists of 2..15 random lower-case letters
	int i, j, len; char word[17];
	for (i = 0; i < nwords; i++) {
		len = rand() % 14 + 2;
		for (j = 0; j < len; j++)
			word[j] = letters[rand()%nletters];
		word[j] = '\n'; word[j+1] = '\0';
		fputs(word,stdout);
	}
    return 0;
}
