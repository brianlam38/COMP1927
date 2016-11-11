// main.c ... program to read keys and insert into a Trie
// The data associated with each key is not interesting
//   just a string containing "This is key #"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "Item.h"
#include "Trie.h"

//void dumpTrie(Trie); // TODO

int main(int argc, char **argv)
{
   Trie t;         // Trie to hold keys
   char k[MAXKEY]; // next key value from stdin
   int  nk = 0;    // counter for # keys
   void normalise(char *);

   t = newTrie();
   while (fgets(k, MAXKEY-1, stdin) != NULL) {
      Item i;
      normalise(k);
      strcpy(i.key,k);
      sprintf(i.data, "This is key %d", nk++);
      insert(t, i);
      //printf("After inserting %s\n",k); // TODO
      //dumpTrie(t); // TODO
   }
   showKeys(t);
   return 0;
}

void normalise(char *s)
{
   while (*s != '\0' && *s != '\n') {
      *s = tolower(*s);
      s++;
   }
   *s = '\0';
}
