/*
 * Sample Prac Question
 * q1.c
 * Read the question sheet first
 *
 * Any questions email riyasat.saber@student.unsw.edu.au
 * but please ask your tutor first
 */

#include <stdio.h>

void printTriangle(int r){
	int i,j;
	for(i=0; i<r; i++){
		for(j=0; j<r; j++){
			if(i <= j) printf("*");
			else printf(" ");
		}
		printf("\n");
	}
}

int main(){
	int r;
	scanf("%d", &r);
	printTriangle(r);
	return 0;
}