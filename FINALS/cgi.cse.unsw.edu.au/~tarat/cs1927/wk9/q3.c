/*
 * Sample Prac Question
 * q3.c
 * Read the question sheet first
 *
 * Any questions email riyasat.saber@student.unsw.edu.au
 * but please ask your tutor first
 */

#include <stdio.h>

#define MAX_STRING_LENGTH 100

int isAnagram(char *s, char *t){
	// write your function here
	return 1;
}

int main(){
	char s1[MAX_STRING_LENGTH], s2[MAX_STRING_LENGTH];
	printf("Enter string 1: ");
	fgets(s1, MAX_STRING_LENGTH, stdin);
	printf("Enter string 2: ");
	fgets(s2, MAX_STRING_LENGTH, stdin);

	if(isAnagram(s1,s2)){
		printf("%scan be rearranged to spell\n%s",s1,s2);
	}else{
		printf("They are not anagrams\n");
	}
	return 0;
}