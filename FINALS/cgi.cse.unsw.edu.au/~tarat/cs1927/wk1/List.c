// List.c
// the implementation the List.h ADT
// By: Karl Krauth, modified by Callum Howard
// Date: 26 may 2013

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "List.h"

typedef struct _node *nodePointer;

typedef struct _node {
    item value;
    nodePointer next;
} node;

typedef struct _list {
    int size;
    nodePointer first;
} list;


// static function definitions
static nodePointer newNode (item data);

List newList (void) {
    
    // allocate memory for the new list
    List new = malloc( sizeof(struct _list) );
    assert(new != NULL);
    
    // initialise the values inside the struct
    new->first = NULL;
    new->size = 0;
    
    return new;
}

void disposeList (List l) {
    // obtain pointer to first node in the list
    nodePointer current = l->first;
    // free nodes in the list if any exist
    while (current != NULL) {
        nodePointer temp = current;
        current = current->next;
        free(temp);
    }
    
    // free the list
    free(l);
}

// 
int length (List l) {
    int length = 0;
    nodePointer current = l->first;
    
    while (current != NULL) {
        current = current->next;
        length++;
    }
    
    return length;
}


// Adds an item to the beginning of the list.
void addFront (List l, item data) {
    // assert a valid list is given
    assert(l != NULL);
    
    // allocate memory for a new node
    nodePointer newNode = malloc( sizeof(struct _node) );
    assert(newNode != NULL);
    
    // fill the node with the given data
    newNode->value = data;
    
    // the new node points to the first item in the list
    newNode->next = l->first;
    
    // list points to the new node, which becomes the (new) first node
    l->first = newNode;
    
}

// Add an item to the end of the list
void addRear (List l, item data) {
    // STEP 00: CHECK your list
    assert(l != NULL);
    
    // create a new node and fill the node with the given data
    nodePointer new = newNode(data);
    
    // STEP 01: handle empty list case
    if (l->first == NULL) {
        l->first = new;
        
    } else {
        // STEP 02: create CURRENT pointer
        nodePointer current = l->first;
    
        // STEP 03: TRAVERSE to the end of the list
        while (current->next != NULL) {
            current = current->next;
        }
        
        // add node to the end of the list
        current->next = new;
    }
    
}

// Prints the contents of the list in order
void printList (List l) {
    
    // STEP 00
    assert(l != NULL);
    
    // STEP 01 handle empty list case
    if (l->first == NULL) {
        printf("[LIST_FIRST]->");
        
    } else {
        
        // STEP 02
        nodePointer current;
        current = l->first;
        
        printf("[LIST_FIRST]->");
        
        // STEP 03: traversal
        while (current != NULL) {
            printf("[%d]->", current->value);
            current = current->next;
        }
        
    }
    
    printf("[X]\n");
}

// creates new node, initialises value of node and points node to NULL
static nodePointer newNode (item data) {
    
    // allocate memory for new node
    nodePointer newNode = malloc( sizeof(struct _node) );
    assert(newNode != NULL);
    
    newNode->next = NULL;
    newNode->value = data;
    
    return newNode;
}


// Inserts the item at the given position
// the first item has position 0
// If position is negative, do nothing
// If position is greater than length of list, append to end
void insertItem(List l, item data, int position) {
    // TO COMPLETE
    return;

}

// Gets the item at the given position
// the first item has position 0
// assume position cannot be out of bounds
item getItem (List l, int position) {
    // TO COMPLETE

    return 0;
}

// Removes the item at the given position
// the first item has position 0
// if position out of bounds, do nothing
void deleteItem (List l, int position) {
    // TO COMPLETE

    return;
}

// Delete all even nodes
// If the list only has one/zero nodes, do nothing
// 1 -> 2 -> 3 -> 4 -> X
// 1  -> 3 -> X
void deleteEven (List l) {
    // TO COMPLETE

    return;
}

// Append the nodes of l2 in to l1
// After terminating l1 should refer to the combined list
// And l2 should be NULL
// l1: 1 -> 2 -> X
// l2: 3 -> 4 -> X
// merge(l1, l2);
// l1: 1 -> 2 -> 3 -> 4 -> X
// l2: X
void merge (List l1, List l2) {
    // TO COMPLETE

    return;
}