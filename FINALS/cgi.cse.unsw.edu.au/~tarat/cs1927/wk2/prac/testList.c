#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

// compile with gcc -Wall -Werror -o question testList.c list.c

// insert proper tests here
int main (int argc, const char * argv[]) {
  printf("Write your tests here!\n");
  printf("Remember to consider boundary/edge cases as well as typical use cases!\n");
  printf("And dont forget to free!\n");

  link i = fromTo(1,10);
  // do some tests
  printf("List i: ");
  printList(i);
  printf("%dth node from the back is ", 2);
  printLink(nodeFromEnd(i,2));

  link j = reverse(i);
  printf("Reversed:\nList j: ");
  printList(j);
  
  freeList(i);
  freeList(j);
  return EXIT_SUCCESS;;
}
