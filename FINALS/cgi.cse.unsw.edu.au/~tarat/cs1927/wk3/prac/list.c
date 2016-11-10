// Implementation the List ADT
// By: Tara Tjandra
// Date: 5/8/2016
// Compilation: gcc -Wall -Werror -O -o list testList.c list.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

#define TRUE 1
#define FALSE 0

typedef struct node *link;

struct node {
    item value;
    link next;
};

struct list {
    int size;
    link head;
};

List newList (void) {
    
    // allocate memory for the new list
    List new = malloc(sizeof(struct list));
    assert(new != NULL);
    
    // initialise the values inside the struct
    new->head = NULL;
    new->size = 0;
    
    return new;
}

void freeList(List l) {
   // Obtain pointer to first node in the list
    link current = l->head;
    // Free nodes in the list if any exist
    while (current != NULL) {
        link temp = current;
        current = current->next;
        free(temp);
    }
    l->size = 0;
    l->head = NULL; 
}

void destroyList (List l) {
    // Obtain pointer to first node in the list
    link current = l->head;
    // Free nodes in the list if any exist
    while (current != NULL) {
        link temp = current;
        current = current->next;
        free(temp);
    }
    
    // Free the list
    free(l);
}

int length(List l) {
    int length = 0;
    link curr = l->head;
    
    while (curr != NULL) {
        curr = curr->next;
        length++;
    }
    
    return length;
}


// Inserts an item at the beginning of the list
void prepend(List l, item data) {
    // Assert a valid list is given
    assert(l != NULL);
    
    // Allocate memory for a new node
    link newNode = malloc(sizeof(struct node));
    assert(newNode != NULL);
    // Fill the node with the given data
    newNode->value = data;
    // New node points to the first item in the list
    newNode->next = l->head;
    // List points to the new node, which becomes the (new) first node
    l->head = newNode;
    l->size++;
}

// Prints the contents of the list in order
void printList (List l) {
    assert(l != NULL);
    // Traverse nodes if any
    for (link curr = l->head; curr != NULL; curr = curr->next) {
        printf("%d -> ", curr->value);
    }
    // Prints NULL
    printf("X\n");
}

// Returns TRUE if the list is in non-descending order
int isSorted(List l) {
    assert(l != NULL);
    if (l->head == NULL) {
        return TRUE;
    }
    int prev = l->head->value;
    for (link curr = l->head; curr != NULL; curr = curr->next) {
        if (curr->value < prev) {
            return FALSE;
        }
        prev = curr->value;
    }

    return TRUE;
}

// Precondition: l1 and l2 are in non-descending order
// Given two lists in non-descending order
// Merge the two so that l1 contains the nodes of
// both original lists in non-descending order
// And l2 holds a reference to the empty list
// No new nodes should be created
// The function should handle empty lists
// l1: 1 -> 3 -> 5 -> X
// l2: 2 -> 4 -> X
// mergeSorted(l1, l2)
// l1: 1 -> 2-> 3 -> 4 -> 5 -> X
// l2: X
void mergeSorted (List l1, List l2) {
    // TO DO
}

// BONUS QUESTION
// Similar behaviour to the previous function
// Except the precondition no longer holds
// l1 should still contain a list in non-descending order
// and l2 should be empty upon return
void mergeUnsorted (List l1, List l2) {
    // TO DO
}