// List.h ... interface to simple List ADT

#ifndef LIST_H
#define LIST_H

typedef struct ListRep *List;

// newList ... make new empty List
List newList();

// ListShow ... display List as <a, b, c, d, ...z>
void ListShow(List);

// ListLength ... number of nodes in List
int ListLength(List);

// ListAppend ... append a new value to List
void ListAppend(List, int);

// ListReverse ... reverse a List
void ListReverse(List);

#endif
