// main.c ... driver program for HashTable type

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "HashTable.h"

int main(int argc, char **argv)
{
   HashTable tab; int n,v;

   tab = newHashTable(3);
   while ((n = scanf("%d",&v)) == 1) {
      insertHashTable(tab,v);
      //potential debugging statements
      //printf("After %d\n",v);
      //showHashTable(tab);
   }
   showHashTable(tab);
   return 0;
}
