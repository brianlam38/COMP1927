#include "list.h"

static link newNode(Item it);

// YOU NEED TO IMPLEMENT THE FUNCTIONS BELOW
// ---------------------------------------
// QUESTION 1
// SOLUTION 1: Traversing through the list more than once
/*
link nodeFromEnd(link l, int k){
  // TODO: Your implementation here
  if(l==NULL){
    return NULL;
  }

  int length = 0;
  link curr = l;
  // first traversal
  while(curr!=NULL){
    length++;
    curr = curr->next;
  }

  // the node we have to return is at index length-k
  curr = l;
  int i = 0;
  // second traversal
  while(i<length-k-1){
    curr = curr->next;
    i++;
  }
  return curr;
}
*/
// SOLUTION 2: Traversing through the list only once
// for the bonus imaginary marks
link nodeFromEnd(link l, int k){
  // TODO: Your implementation here
  // Edge case:
  if(l==NULL){
    return NULL;
  }
  link curr = l, reference = l;
  int i = 0;
  while(i<k){
    reference = reference->next;
    i++;
  }
  while(reference->next!=NULL){
    curr = curr->next;
    reference = reference->next;
  }
  return curr;
}

// ---------------------------------------

// ---------------------------------------
// QUESTION 2
// SOLUTION 1
link newLink(Item data){
  link n = malloc(sizeof(struct _node));
  n->item = data;
  n->next = NULL;
  return n;
}

link insertAtFront(link l, link n){
  if(l==NULL) return n;
  if(n==NULL) return l;
  n->next = l;
  return n;
}

link reverse(link l){
  if(l== NULL) return NULL;
  link new = NULL;
  link curr = l;

  while(curr!=NULL){
    new = insertAtFront(new, newLink(curr->item));
    curr = curr->next;
  }
  return new;
}

/*
// SOLUTION 2 
// Not actually a solution but you should know how to do this
// this modifies the original linked list
link reverse(link l){
  link prev   = NULL;
  link current = l;
  link next;
  while (current != NULL){
      next  = current->next;  
      current->next = prev;   
      prev = current;
      current = next;
  }
  l = prev;
  return l;
}
*/


void printList (link list) {
  link curr = list;
  while (curr != NULL) {
    printf ("[%d]->", curr->item);
    curr = curr->next;
  }
  printf ("[X]\n");
}

int sumListItems (link ls) {
    link curr = ls;
    int sum = 0;
    while (curr != NULL) {
        sum += curr->item;
        curr = curr->next;
    }
    return sum;
}

void freeList(link list){
  link curr = list;
  link next;   
  while(curr!= NULL){
        next = curr->next;
        free(curr);
        curr = next;
  }
}

link fromTo (int start, int end) {
  int i;
  link list = NULL;
  link tail = NULL;
  for(i = start; i <= end; i++){
    link n = newNode(i);   
    if(list == NULL){
      list = n;     
    } else {
      tail->next = n;     
    }
    tail = n;
  }
  return list;
}

static link newNode(Item it)
{
  link newNode = malloc(sizeof(node));
  newNode->next = NULL;
  newNode->item = it;
  return newNode;
}

void printLink(link l){
  if(l==NULL) return;
  printf("[%d]\n",l->item);
}




