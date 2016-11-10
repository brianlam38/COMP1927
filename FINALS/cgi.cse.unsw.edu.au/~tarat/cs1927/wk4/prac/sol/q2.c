/*
	Solution by Riyasat Saber
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct _link {
	char ch;
	struct _link * next;
}*link;

// write your function here
// write a function that finds the first occurance of str2 in str1
// if it does not find it, return NULL

int getLength(link a){
	if(a==NULL) return 0;
	return 1+getLength(a->next);
}

link sublink(link str1, link str2){
	if(str1 == NULL) return NULL;
	if(str2 == NULL) return NULL;

	if(getLength(str2) > getLength(str1)) return NULL;

	link curr1 = str1;
	link curr2 = str2;

	link temp = NULL;

	while(curr1 != NULL){
		if(curr1->ch == curr2->ch){
			// we found the first letter,
			// now check the rest
			temp = curr1;

			while(curr2!=NULL){
				if(curr1->ch != curr2->ch)
					break;
				curr2 = curr2->next;
				curr1 = curr1->next;
			}
			if(curr2==NULL){
				// we found a match!
				return temp;
			}
			curr2 = str2;
			curr1 = temp;
		}
		curr1 = curr1->next;
	}
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