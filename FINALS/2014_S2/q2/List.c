// List.c ... implementation of List library

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

typedef struct _node {
   int value;
   struct _node *next;
} Node;

typedef struct _list_rep {
   Node *first;
   Node *last;
} ListRep;

// create a new List node
static Node *newNode(int val)
{
   Node *new;
   new = malloc(sizeof(Node));
   assert(new != NULL);
   new->value = val;
   new->next = NULL;
   return new;
}

// create an empty list
List newList()
{
   ListRep *new;
   new = malloc(sizeof(ListRep));
   assert(new != NULL);
   new->first = NULL;
   new->last = NULL;
   return new;
}

// free memory for a list
void dropList(List L)
{
   assert(L != NULL);
   Node *cur;
   cur = L->first;
   while (cur != NULL) {
      Node *next;
      next = cur->next;
      free(cur);
      cur = next;
   }
   free(L);
}

// display a list on stdout
void showList(List L)
{
   assert(L != NULL);
   Node *cur;
   cur = L->first;
   while (cur != NULL) {
      printf("%d",cur->value);
      if (cur->next != NULL) printf("->");
      cur = cur->next;
   }
   printf("\n");
}

// add a value at the end of a list
void appendList(List L, int val)
{
   assert(L != NULL);
   Node *new = newNode(val);
   if (L->first == NULL)
      L->first = L->last = new;
   else {
      L->last->next = new;
      L->last = new;
   }
}

// get values from list into array
// - array is dynamically allocated to be large enough
// - returns pointer to array as result of function
// - sets var referenced by nvals to hold size of array
int *valuesFromList(List L, int *nvals)
{
   Node *cur;
   int n = 0;
   for (cur = L->first; cur != NULL; cur = cur->next)
      n++;
   int *array = malloc(n*sizeof(int));
   int i = 0;
   for (cur = L->first; cur != NULL; cur = cur->next)
      array[i++] = cur->value;
   *nvals = n;
   return array;
}

