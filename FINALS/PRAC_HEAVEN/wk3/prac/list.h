// Created for COMP1927 S2 2016 Prac Revision
// By: Tara Tjandra
// Date: 5/8/2016

typedef struct list *List;
typedef int item;

// Create a new list
List newList(void);

// Deletes a list and releases all memory used by it.
// The list should not be access again after this
// function has been called on it.
void destroyList(List l);

// Free the underlying list pointed to by head
// Makes head point to NULL but keeps List ADT intact
void freeList(List l);

// Inserts an item at the beginning of the list
void prepend(List l, item data);

// Returns the number of items in a list
int length(List l);

// Prints the contents of list
void printList (List l);

// Returns TRUE if the list is in non-descending order
int isSorted(List l);

// Given two lists in ascending order
// Merge the two so that l1 contains the nodes of
// both original lists in ascending order
// And l2 holds a reference to the empty list
// No new nodes should be created
// The function should handle empty lists
// l1: 1 -> 3 -> 5 -> X
// l2: 2 -> 4 -> X
// mergeSorted(l1, l2)
// l1: 1 -> 2-> 3 -> 4 -> 5 -> X
// l2: X
void mergeSorted (List l1, List l2);

// BONUS QUESTION
// Similar behaviour to the previous function
// Except the precondition no longer holds
// l1 should still contain a list in non-descending order
// and l2 should be empty upon return
void mergeUnsorted (List l1, List l2);