// Item.c ... implementation of Items
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <string.h>
#include "Item.h"

// These would normally be done as macros
// We do them as functions so that
// they appear in the profile output

// compare two Items
int cmp(Key k1, Key k2)
{
	return strcmp(k1,k2);
}
// make a copy of an Item
Item newItem(char *s)
{
	return strdup(s);
}
// free memory for an Item
void dropItem(Item it)
{
	free(it);
}
// read an Item from a file
Item ItemGet(FILE *f)
{
	char line[1000];
	if (fgets(line,1000,f) == NULL)
		return NULL;
	else {
		// strip '\n'
		line[strlen(line)-1] = '\0';
		return newItem(line);
	}
}
// display an Item
void ItemShow(Item it)
{
	printf("%s",it);
}
