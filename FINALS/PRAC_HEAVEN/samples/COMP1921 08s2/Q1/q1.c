#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef struct lnode ListNode;

struct lnode {
	int data;
	ListNode *next;
};

// prototypes of given functions
ListNode *makeNode(int);
ListNode *findNode(int, ListNode*);
void printList(ListNode *);
void freeList(ListNode *);
ListNode *insert(ListNode *, ListNode *);
ListNode *excise(ListNode *, ListNode *);

// ------------------------------------------------------------------
// prototype of function you need to modify
// ------------------------------------------------------------------
ListNode * reorder(ListNode *);

int main(void)
{
	ListNode *list = NULL;
	ListNode *node;
	int ch;

	// create unordered list of characters entered by user

	printf("Enter characters from the keyboard; <ENTER>=list; <CTRL-D>=end\n");

	while ((ch = getchar()) != EOF) { 
		if (ch == '-') { // remove item from list
			ch = getchar();
			node = findNode(ch, list);
			if (node != NULL) {
				list = excise(node, list);
				free(node);
			}
		}
		else if (ch == '\n') {
			printList(list);
		} else {
			list = insert(makeNode(ch), list);
		}
	}

	// print out the entered list

	printf("\nList before reorder:\n");
	printList(list);

	// move the largest element of the list to the end of the list
	// --------------------------------------------------------------
	// your function called here
	// --------------------------------------------------------------
	list = reorder(list);
	printf("\nList after reorder:\n");
	printList(list);

	// free the memory allocated to the list

	freeList(list);

	return 0;
}

/*
 * Create a new node with data
 *
 * Return pointer to the new node
 */
ListNode * makeNode(int data)
{
	ListNode *new_node = (ListNode*)malloc(sizeof(ListNode));

	assert(new_node != NULL);

	new_node->data = data;
	new_node->next = NULL; // ensure next->NULL !

	return (new_node);
}

/*
 * Return pointer to node containing data within list pointed to by head
 */
ListNode * findNode(int data, ListNode *head)
{
    ListNode * node = head; // start at first node in list

	// keep searching until data found, or end of list
	while ((node != NULL) && (node->data != data)) {
		node  = node->next;
	}

	return (node);
}

/*
 * Print list pointed to by head in order
 */
void printList(ListNode *head)
{
	ListNode *node = head;
	 
	// traverse the list printing each node in turn
	while (node != NULL) {
		printf("->%c", node->data);
		node = node->next;
	}
	printf("\n");
}

/*
 * Free all nodes of list pointed to by head
 */
void freeList(ListNode *head)
{
	ListNode *node = head;
	ListNode *tmp;
		  
	while (node != NULL) {
		tmp = node;
		node = node->next;
		free(tmp);
	}
}

/*
 * Insert new node at the front of the list pointed to by head
 *
 * Return new head
 */
ListNode *insert(ListNode *new_node, ListNode *head)
{ 
	new_node->next = head;
	return (new_node);
}

/*
 * Remove node from list pointed to by head and return new head
 */
ListNode *excise(ListNode *node, ListNode *head)
{ 
	if (node != NULL) {
		if (node == head)
			head = head->next;    // remove first item
		else {
			ListNode *prev_node = head;
			while (prev_node && prev_node->next != node) {
				prev_node = prev_node->next;
			}
			if (prev_node != NULL) { // node found in list
				prev_node->next = node->next;
			}
		}
	}
	return (head);
}

/*
 * Move the largest element on the list pointed to by head to the end of the list
 *
 * Return new head
 */
ListNode *reorder(ListNode * head)
{
	return (head);
}
