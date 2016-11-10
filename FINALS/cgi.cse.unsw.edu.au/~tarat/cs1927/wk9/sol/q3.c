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

char upperify(char ch){
	if('a' <= ch && ch <= 'z'){
		ch = 'A' + ch - 'a';
	}
	return ch;
}

int isLetter(char c){
	return ('A' <= c && c <= 'Z') ? 1:0;
}

// EDIT THIS FUNCTION ONLY
int isAnagram(char *s, char *t){
	int alphaS[25] = {0}, alphaT[25] = {0}, i=0;
	char ch;
	while(s[i]!='\0'){
		ch = upperify(s[i]);
		if(isLetter(ch)){
			alphaS[ch-'A']++;
		}
		i++;
	}
	i=0;
	while(t[i]!='\0'){
		ch = upperify(s[i]);
		if(isLetter(ch)){
			alphaT[ch-'A']++;
		}
		i++;
	}
	i=0;
	while(i<26){
		if(alphaS[i] != alphaT[i]){
			return 0;
		}
		i++;
	}
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