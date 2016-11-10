 /*
 * Sample Prac Question
 * q5.c
 * Read the question sheet first
 *
 * Any questions email riyasat.saber@student.unsw.edu.au
 * but please ask your tutor first
 */

#include <stdio.h>

#define MAXLEN 100

int lenstr(char * a){
	return a[0] == '\0' ? 0 : 1 + lenstr(&a[1]);
}

char *substring(char s1[], char s2[]) {
 	// write your function here
 	return NULL;
}

int main(void) {

	char s1[MAXLEN];
	char s2[MAXLEN];
	char *found;

	printf("Enter the string you wish to search: ");
	fgets(s1, MAXLEN, stdin);
	s1[lenstr(s1)-1] = '\0';

	printf("Enter the substring you wish to search for: ");
	fgets(s2, MAXLEN, stdin);
	s2[lenstr(s2)-1] = '\0';

	printf("Searching: [%s] for [%s]\n", s1, s2);
	
	// YOUR FUNCTION CALLED HERE ------------------------------------
	found = substring(s1, s2);

	printf("Substring [%s] found\n", found);

	return 0;
}