#include <stdio.h>
#include <stdlib.h>

typedef struct _link {
	char ch;
	struct _link * next;
}*link;

// write your function here
link sublink(link str1, link str2){
	// TODO your implementation here
	return NULL;
}

// DONT TOUCH THIS FUNCTION
link append(link ls, link n){
	if(ls == NULL) return n;

	link c = ls;
	while(c->next!=NULL){
		c = c->next;
	}
	c->next = n;
	n->next = NULL;
	return ls;
}

// OR THIS ONE
link newNode(char c){
	link n = malloc(sizeof(struct _link));
	n->ch = c;
	n->next = NULL;
	return n;
}

// OR THIS ONE EITHER
link newWord(char * str){
	int i = 0;
	link word = NULL;
	while(str[i]!='\0'){
		word = append(word, newNode(str[i++]));
	}
	return word;
}

void printWord(link w){
	if(w==NULL){
		printf("[(null)]\n");
		return;
	}
	link c = w;
	while(c!=NULL){
		printf("%c", c->ch);
		c = c->next;
	}
	printf("\n");
}

// but you should write some more tests in this function right here
int main(){
	// write test cases here
	link s1 = newWord("Hello World!");
	link s2 = newWord("lo W");

	link s3 = sublink(s1,s2);

	printf("Searching for: ");
	printWord(s2);
	printf("Inside of: ");
	printWord(s1);
	printWord(s3);
	printf("found\n");

	return 0;
}