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

char * substring(char * s1, char * s2){
    int s1len = lenstr(s1);
    int s2len = lenstr(s2);
    int i,j, flag = 1;
    
    for(i=0; i<s1len; i++){
        if(s1[i] == s2[0]){
        	if(i+s2len > s1len) return NULL;
            // we found the first letter, now to look for the others
            for(j=0; j<s2len && flag; j++){
                if(s1[i+j] != s2[j]) flag = 0;
            }
            if(flag) return s1+i;
        }
        flag = 1;
    }
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