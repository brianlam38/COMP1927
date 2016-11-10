#include <stdio.h>
#include <string.h>

#define MAXLEN 100

//-------------------------------------------------------------------
// FUNCTION YOU NEED TO ADD
//
char *substring(char [], char []);
//-------------------------------------------------------------------

int main(void) {

	char s1[MAXLEN];
	char s2[MAXLEN];
	char *found;

	printf("Enter the string you wish to search: ");
	fgets(s1, MAXLEN, stdin);
	s1[strlen(s1)-1] = '\0';

	printf("Enter the substring you wish to search for: ");
	fgets(s2, MAXLEN, stdin);
	s2[strlen(s2)-1] = '\0';

	printf("Searching: [%s] for [%s]\n", s1, s2);
	
	// YOUR FUNCTION CALLED HERE ------------------------------------
	found = substring(s1, s2);

	printf("Substring [%s] found\n", found);

	return 0;
}

char *substring(char s1[], char s2[]) {
/*
 * Finds the first occurrence of the substring s2 in the string s1.  The terminat-
 * ing '\0' characters are not compared.
 *
 * The function returns a pointer to the beginning of the substring, or NULL if 
 * the substring is not found.
 */

}
