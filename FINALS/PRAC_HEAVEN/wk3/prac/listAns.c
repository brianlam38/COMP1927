// Prac Week 3 Answers
// By: Tara Tjandra
// Date: 5/8/2016
// Compilation: gcc -Wall -Werror -O -o list testList.c list.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

#define TRUE 1
#define FALSE 0

static void sortList(List l);

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

static void sortList(List l) {
    assert(l != NULL);
    int len = length(l);
    int i, j;
    // Selection sort
    for (i = 0; i < len; i++) {
        link first = l->head;
        int iter = 0;
        while (iter < i) {
            first = first->next;
            iter++;
        }
        link min = first;
        link curr = first;
        for (j = i; j < len; j++) {
            if (curr->value < min->value) {
                min = curr;
            }
            curr = curr->next;
        }
        int temp = first->value;
        first->value = min->value;
        min->value = temp;
    }

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
    if (l2->head == NULL) {
        return;
    }
    if (l1->head == NULL) {
        l1->head = l2->head;
        l1->size = l2->size;
        l2->head = NULL;
        l2->size = 0;
        return;
    }

    link curr1 = l1->head;
    link curr2 = l2->head;
    link prev1 = curr1;
    // Head of l2 is new head of l1
    if (curr2->value < curr1->value) {
        // Make node new head for l1
        l1->head = curr2;
        // Keep track of next node in l2
        link temp = curr2->next;
        // Make new head point to old head
        curr2->next = curr1;
        // Make prev reference for l1 point to new head
        prev1 = curr2;
        // Increment l2 to point to next node in list
        curr2 = temp;
    }
    while (curr1 != NULL && curr2 != NULL) {
        if (curr2->value < curr1->value) {
            // Save reference to next in l2
            link temp = curr2->next;
            // Make previous node from l1 connect to new node
            prev1->next = curr2;
            // Connect l2 node to l1
            curr2->next = curr1;
            // Advance prev and l2 pointer
            prev1 = curr2;
            curr2 = temp;
        } else {
            // Advance prev placeholder
            prev1 = curr1;
            // Advance l1 pointer
            curr1 = curr1->next;
        }
    }
    // No more nodes from l1
    // Append remaining l2 nodes to l1
    // Nothing to do for else
    if (curr1 == NULL) {
        prev1->next = curr2;
    }

    // Assign new values
    l1->size += l2->size;
    l2->head = NULL;
    l2->size = 0;

    return;
}

// BONUS QUESTION
// Similar behaviour to the previous function
// Except the precondition no longer holds
// l1 should still contain a list in non-descending order
// and l2 should be empty upon return
void mergeUnsorted (List l1, List l2) {
    sortList(l1);
    sortList(l2);
    mergeSorted(l1, l2);
}