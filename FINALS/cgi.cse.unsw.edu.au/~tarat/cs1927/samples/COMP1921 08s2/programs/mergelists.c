#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef struct lnode Lnode;

struct lnode {
	int data;
	Lnode *next;
};

Lnode * makeNode( int );
Lnode * findNode( int, Lnode* );
void printList( Lnode * );
void freeList( Lnode * );
Lnode * insert( Lnode *, Lnode * );
Lnode * excise ( Lnode *, Lnode * );

//-------------------------------------------------------------------
// FUNCTION YOU NEED TO WRITE
Lnode * mergeLists( Lnode *, Lnode * );
//-------------------------------------------------------------------

int main ( void ) {

	Lnode *list1 = NULL;
	Lnode *list2 = NULL;
	Lnode *list3 = NULL;

	Lnode *node;
	int ch;

    // get first list
	printf("Input List 1:\n"); 

	while ((ch = getchar()) != EOF) { // get character
		if ( ch == '-' ) { // remove item from list
			ch = getchar();
			node = findNode( ch, list1 );
			if (node != NULL) {
				list1 = excise( node, list1 );
				free( node );
			}
		}
		else if ( ch == '\n') {
			printf("List 1: ");
			printList(list1);
		} else {
			list1 = insert( makeNode(ch), list1 ); // insert in order
		}
	}

    // get second list
	printf("\nInput List 2:\n");

	while ((ch = getchar()) != EOF) { // loop to enter list 2
		if ( ch == '-' ) { // remove item from list
			ch = getchar();
			node = findNode( ch, list2 );
			if (node != NULL) {
				list2 = excise( node, list2 );
				free( node );
			}
		}
		else if ( ch == '\n') {
			printf("List 2: ");
			printList(list2);
		} else {
			list2 = insert( makeNode(ch), list2 ); // insert in order
		}
	}

	// print both lists out
	printf("\nList 1: ");
	printList(list1);
	printf("List 2: ");
	printList(list2);

    // YOUR FUNCTION CALLED HERE ------------------------------------
	// derive merged and sorted list
	list3 = mergeLists(list1, list2);

	// print merged list
	printf("List 1+2: ");
	printList(list3);

	// free all lists
	freeList( list1 );
	freeList( list2 );
	freeList( list3 );

	return 0;
}

Lnode * makeNode( int data ) {
/*
 * Create a new node with data
 *
 * Return pointer to the new node
 */
	Lnode *new_node = (Lnode*)malloc(sizeof(Lnode));

	assert(new_node != NULL);

	new_node->data = data;
	new_node->next = NULL; // ensure next->NULL !

	return (new_node);
}

Lnode * findNode(int data, Lnode *head ) {
/*
 * Return pointer to node containing data within list pointed to by head
 */
    Lnode * node = head; // start at first node in list

	// keep searching until data found, or end of list
	while ((node != NULL) && (node->data != data)) {
		node  = node->next;
	}

	return (node);
}

void printList( Lnode *head ) {
/*
 * Print list pointed to by head in order
 */
	Lnode *node = head;
	 
	// traverse the list printing each node in turn
	while (node != NULL) {
		printf("->%c", node->data );
		node = node->next;
	}
	printf("\n");
}

void freeList( Lnode *head ) {
/*
 * Free all nodes of list pointed to by head
 */
	Lnode *node = head;
	Lnode *tmp;
		  
	while ( node != NULL ) {
		tmp = node;
		node = node->next;
		free(tmp);
	}
}

Lnode * insert( Lnode *new_node, Lnode *head ) { 
/*
 * Insert new node in increasing ascii code order into list pointed to by head
 *
 * Return new head
 */
	Lnode *next_node = head, *prev_node = NULL;
	while (next_node && new_node->data > next_node->data) {
		prev_node = next_node;
		next_node = next_node->next;
	}
	if (prev_node == NULL)
		head = new_node;
	else
		prev_node->next = new_node;
	new_node->next = next_node;
	return (head);
}

Lnode * excise ( Lnode *node, Lnode *head ) { 
/*
 * Remove node from list pointed to by head and return new head
 */
	if (node != NULL) {
		if (node == head)
			head = head->next;    // remove first item
		else {
			Lnode *prev_node = head;
			while (prev_node && prev_node->next != node) {
				prev_node = prev_node->next;
			}
			if ( prev_node != NULL ) { // node found in list
				prev_node->next = node->next;
			}
		}
	}
	return (head);
}

Lnode * mergeLists ( Lnode *head1, Lnode *head2 ) {
/*
 * Create a new list containing all items of lists pointed to by head1 and head2
 * in increasing ascii code order 
 *
 * Return a pointer to the head of the newly created list
 */

}
