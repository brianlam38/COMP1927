/*
 * Collect all words in a text containing a given substring
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*
 * Constants
 */
#define MAXWORD	40

/*
 * Data types
 */
typedef enum {FALSE, TRUE} Bool;

typedef struct w {
	char     *word;
	struct w *next;
} Node;

typedef Node *Words;


/*
 * Main program
 */
int main(int argc, char*argv[])
{
	char  word[MAXWORD];  /* current word */
	Words matches;        /* list of matching words */
	char  *substring;     /* string to look for */
	FILE  *input;         /* the input file */

	char  *getWord();
	Bool  member(), contains();
	void  insert(), printWords();

	/*
	 * Check command-line arguments
	 */
	if (argc != 3) {
		fprintf(stderr, "Usage: %s substring file\n", argv[0]);
		exit(1);
	}
	substring = argv[1];

	/*
	 * Open the file - check that it's ok
	 */
	input = fopen(argv[2], "r");
	if (input == NULL) {
		fprintf(stderr, "%s: can't open %s\n", argv[0], argv[2]);
		exit(2);
	}

	/*
	 * Process the file - find the matching words
	 */
	while (getWord(input, word) != NULL) {
		if (contains(word,substring) && !member(word, matches))
			insert(word, &matches);
	}
	printWords(matches);
	return 0;
}

/*
 * Read next word from input (& convert all chars to lower case)
 *
 * Pre:  outside any word
 * Post: result == NULL || (result == buffer && next input word is in buffer)
 */
char *getWord(FILE *f, char *buffer)
/*!*/             /*#*/    /*!*/
{
	int  ch = ' ';        /* current input character */
	int  nc = 0;          /* count of chars stored in buffer */

	/*
	 * Scan to start of next word
	 */
	while ((ch = getc(f)) != EOF && !isalnum(ch))
		/* skip over uninteresting stuff */;

	/*
	 * Is there really a word?
	 */
	if (ch == EOF) /* bump into EOF without finding any word */
		return NULL;
	else {         /* start the word off */
		buffer[0] = tolower(ch);
		nc = 1;
	}

	/*
	 * Scan the word and put it into the buffer
	 * If we meet EOF halfway through word, take what
	 *  we've got and use it as a word. Leave EOF to
	 *  be handled by next call to getWord.
	 */
	while ((ch = getc(f)) != EOF && isalnum(ch))
		if (nc < MAXWORD-1)
			buffer[nc++] = tolower(ch);
	buffer[nc] = '\0';
	return buffer;
}


/*
 * Check whether substring occurs in a string
 *
 * Pre:  sub:String && str:String
 * Post: result == (sub occurs in string)
 */
Bool contains(char *str, char *sub)
/*!*/             /*?*/      /*?*/
{
	char *c1, *c2, *c3;

	for (c1 = str; *c1 != '\0'; ++c1) {
		if (*c1 == *sub) { /* matched first letter of sub */
			c2 = sub+1; /* start at 2nd char in sub */
			c3 = c1+1; /* start at next pos in str */
			for (; *c2 != '\0' && *c3 != '\0'; ++c2, ++c3)
				if (*c2 != *c3)
					break;
			if (*c2 == '\0')
				return TRUE;
		}
	}
	return FALSE;
}

/*
 * Search for a word in a sorted list of words
 *
 * Pre:  word:String && list:Words
 * Post: result == (word occurs in list)
 */
Bool member(char *word, Words list)
/*!*/           /*?*/       /*?*/
{
	Node *curr; /* pointer to next Node */

	for (curr = list; curr != NULL; curr = curr->next) {
		if (strcmp(word, curr->word) == 0)
			return TRUE;
	}
	return FALSE;
}


/*
 * Create new word Node and insert into sorted word list
 *
 * Pre:  w:string && *list:Words
 * Post: *list' has w inserted into it && ordered(*list')
 */
void insert(char *w, Words *list)
                /*?*/     /*#*/
{
	char *newWord; /* copy of word */
	Node *newNode; /* new list node */
	Node *curr;    /* pointer to next Node */
	Node *prev;    /* pointer to previous Node */

	/*
	 * Create and fill buffer for the new word
	 */
	if ((newWord = malloc(strlen(w)+1)) == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	strcpy(newWord, w);

	/*
	 * Create a list Node for it
	 */
	if ((newNode = malloc(sizeof(Node))) == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	newNode->word = newWord;
	newNode->next = NULL;

	/*
	 * Find correct location for new word
	 */
	prev = NULL;
	curr = *list;
	while (curr != NULL && strcmp(curr->word, w) < 0) {
		prev = curr;
		curr = curr->next;
	}

	/*
	 * Insert new Node into list
	 */
	if (prev == NULL) {
		*list = newNode;
		newNode->next = curr;
	}
	else {
		newNode->next = curr;
		prev->next = newNode;
	}
}


/*
 * Display a list of Words, one per line
 */
void printWords(Words list)
		    /*?*/
{
	Node *w;

	/* For each word in the list ...  */
	for (w = list; w != NULL; w = w->next)
		printf("%s\n", w->word); /* ... print it */
}


/*
 * freeWords:
 *	Clean up memory allocated to a list of words
 */
void freeWords(Words *list)
		    /*!*/
{
	Node *w, *next;

	/* release first node */
	next = *list;
	*list = NULL;

	/* for each node in list */
	for (w = next; w != NULL; w = next) {
		next = w->next;
		/* release its data */
		free(w->word);
		/* then release it */
		free(w);
	}
}

