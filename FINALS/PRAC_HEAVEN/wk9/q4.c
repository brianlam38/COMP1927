/*
 * Sample Prac Question
 * q4.c
 * Read the question sheet first
 *
 * Any questions email riyasat.saber@student.unsw.edu.au
 * but please ask your tutor first
 */

#include <stdio.h>
#include <stdlib.h>

#define ASCENDING 1
#define DESCENDING 0
#define UNDETERMINED -1

int findOrder(int * a, int size){
	// write your function here
	return 0;
}

int main(){
	int size;
	printf("Enter the size of your array: ");
	scanf("%d",&size);
	int * a = malloc(sizeof(int) * size);
	printf("Enter the elements of the array, seperated by new lines\n");
	int i;
	for(i=0; i<size; i++){
		scanf("%d", &a[i]);
	}

	int r = findOrder(a,size);

	printf("The array is in ");

	if(r==ASCENDING){
		printf("ascending");
	}else if(r==DESCENDING){
		printf("descending");
	}else{
		printf("an indeterminable");
	}
	printf(" order\n");

	free(a);
	return 0;
}