// Trie.h - interface to Trie ADT

#ifndef TRIE_H
#define TRIE_H

typedef struct TrieRep *Trie;

Trie newTrie();
void insert(Trie, Item);
void delete(Trie, Key_t);
Item *search(Trie, Key_t);
void showKeys(Trie);

#endif
