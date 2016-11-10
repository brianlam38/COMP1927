// Compilation:  gcc -Wall -Werror -O -o list List.c testList.c 
// List.h
// an ADT to represent a list.
// By: Karl Krauth
// Date: 26 may 2013

#ifndef ListKK_List_h
#define ListKK_List_h

typedef struct _list *List;
typedef int item;

// Create a new list
List newList (void);

// Deletes a list and releases all memory used by it.
// The list should not be access again after this
// function has been called on it.
void disposeList (List l);

// Returns the length of a list
int length (List l);

// Adds an item to the beginning of the list.
void addFront (List l, item data);

// Add an item to the end of the list
void addRear (List l, item data);

// Inserts the item at the given position
// the first item has position 0
// If position is negative, do nothing
// If position is greater than length of list, append to end
void insertItem(List l, item data, int position);

// Gets the item at the given position
// the first item has position 0
// assume position cannot be out of bounds
item getItem (List l, int position);

// Removes the item at the given position
// the first item has position 0
// if position out of bounds, do nothing
void deleteItem (List l, int position);

// Prints the contents of the list in order
void printList (List l);

// Delete all even nodes
// If the list only has one/zero nodes, do nothing
// 1 -> 2 -> 3 -> 4 -> X
// 1  -> 3 -> X
void deleteEven (List l);

// Append the nodes of l2 in to l1
// After terminating l1 should refer to the combined list
// And l2 should be NULL
// l1: 1 -> 2 -> X
// l2: 3 -> 4 -> X
// merge(l1, l2);
// l1: 1 -> 2 -> 3 -> 4 -> X
// l2: X
void merge (List l1, List l2);

#endif

