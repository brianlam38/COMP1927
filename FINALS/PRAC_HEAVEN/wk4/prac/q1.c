#include <stdio.h>
#include <stdlib.h>

// Question similiar to 16x1 final
// 16x1 final had the same function but on a single linked list
// Credit to Riyasat Saber

typedef struct _dlink {
	int data;
	struct _dlink * next;
	struct _dlink * prev;
}*dlink;

// Your function here
void removeAtIndex(dlink list, int index){
	// TODO your implementation here
}

dlink append(dlink list, dlink node){
	if(list == NULL) return node;
	dlink curr = list;

	while(curr->next!=NULL){
		curr = curr->next;
	}
	curr->next = node;
	node->prev = curr;

	return list;
}

dlink newDNode(int a){
	dlink d = malloc(sizeof(struct _dlink));
	d->data = a;
	d->next = NULL;
	d->prev = NULL;
	return d;
}

void printDList(dlink ls){
	printf("\nPrinting list:\n");
	dlink curr = ls;
	printf("[X]");
	while(curr!=NULL){
		printf("<-[%d]->", curr->data);
		curr = curr->next;
	}
	printf("[X]\n");
}

void printDListReverse(dlink ls){
	printf("\nPrinting list in reverse:\n");

	dlink curr = ls;
	while(curr->next!=NULL) curr = curr->next;
	printf("[X]");
	while(curr!=NULL){
		printf("<-[%d]->", curr->data);
		curr = curr->prev;
	}
	printf("[X]\n");
}

dlink newDListWithString(char * str){
	int i = 0;
	dlink ls = NULL;
	while(str[i]!='\0'){
		ls = append(ls, newDNode(str[i++] - '0'));
	}
	return ls;
}

int main(){
	// write your test cases here

	dlink l1 = newDListWithString("123456789");
	printf("List l1:\n");
	printDList(l1);
	printf("Deleting node with index %d.\n", 5);
	removeAtIndex(l1, 5);
	printf("List l1:\n");
	printDList(l1);
	printDListReverse(l1);

	return 0;
}
