// Trie.c - implementation of Trie ADT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Item.h"

///// Internal data representation

typedef struct TrieNode *Link;

typedef struct TrieNode {
   char keybit; // one char from key
   int  finish; // last char in key?
   Item data;   // no Item if !finish
   Link child;
   Link sibling;
} TrieNode;

typedef struct { Link root; } TrieRep;

typedef TrieRep *Trie;

///// Internal functions

// newTrieNode: create a new new node in a Trie (internal)
static TrieNode *newTrieNode(Key_t k, int i, Item it)
{
   TrieNode *new = malloc(sizeof(TrieNode));
   assert(new != NULL);

   new->keybit = k[i];
   if (k[i+1] != '\0') {
      new->finish = 0;
      strcpy(new->data.key,"");
      strcpy(new->data.data,"");
   }
   else {
      new->finish = 1;
      new->data = it;
   }
   new->child = NULL;
   new->sibling = NULL;
   return new;
}

// find: search for a key value in a Trie (used internally)
static TrieNode *find(Trie t, Key_t k)
{
   char *c = k;
   TrieNode *curr = t->root;
   while (*c != '\0' && curr != NULL) {
      // scan siblings
      while (curr != NULL && curr->keybit != *c)
         curr = curr->sibling;
      if (curr == NULL) return NULL;
      if (*(c+1) == '\0') return curr;
      curr = curr->child; // move down one level
      c++;                // get next character
   }
   return NULL;
}

///// Interface functions

// newTrie: create a new empty Trie
Trie newTrie()
{
   TrieRep *new = malloc(sizeof(TrieRep));
   assert(new != NULL);
   new->root = NULL;
   return new;
}

// insert: insert (or update) an Item in a Trie
void insert(Trie t, Item it)
{
   Key_t k = keyOf(it);
   TrieNode *n = find(t,k);
   if (n != NULL) {
      n->finish = 1;
      n->data = it; // replaces any existing Item
      return;
   }
   if (t->root == NULL) {
      t->root = newTrieNode(k,0,it);
   }
   int i;
   TrieNode *curr = t->root, *prev;
   for (i = 0; k[i] != '\0'; i++) {
      // scan siblings
      prev = NULL;
      while (curr != NULL && curr->keybit != k[i]) {
         prev = curr;
         curr = curr->sibling;
      }
      if (curr == NULL) // add new sibling
         curr = prev->sibling = newTrieNode(k,i,it);
      if (k[i+1] == '\0') break;
      if (curr->child == NULL)
         curr->child = newTrieNode(k,i+1,it);
      curr = curr->child; // move down one level
   }
}

// delete: remove Item associated with Key
void delete(Trie t, Key_t k)
{
   TrieNode *n = find(t,k);
   if (n == NULL) return;
   n->finish = 0;
}

// search: return pointer to Item associated with Key
Item *search(Trie t, Key_t k)
{
   TrieNode *n = find(t,k);
   if (n == NULL) return NULL;
   return (n->finish) ? &(n->data) : NULL;
}

// showKeys: display all Keys in Trie, one per line
void showKeys(Trie t)
{
	// TODO
}
