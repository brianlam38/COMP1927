#define NDIGITS 26

typedef struct TrieNode *Link;

typedef struct TrieNode {
   char keybit; // one char from key
   int  finish; // last char in key?
   Item data;   // no Item if !finish
   Link child[NDIGITS];
} TrieNode;

typedef struct { Link root; } TrieRep;

typedef TrieRep *Trie;
typedef char *Key;

TrieNode *newTrieNode(Key k, int i, Item it)
{
   TrieNode *new = malloc(sizeof(TrieNode));
   new->keybit = k[i];
   if (k[i+1] != '\0') {
      new->finish = 0;
      new->data = NoItem;
   }
   else {
      new->finish = 1;
      new->data = it;
   }
   int i;
   for (i = 0; i < NDIGITS; i++) new->child[i] = NULL;
}

TrieNode *find(Trie t, Key k)
{
   char *c = k;
   TrieNode *curr = t->root;
   while (*c != '\0' && curr != NULL) {
      if (*(c+1) == '\0') return curr;
      curr = curr->child[*c-'a']; // move down one level
      c++;                // get next character
   }
   return NULL;
}

Item *search(Trie t, Key k)
{
   TrieNode *n = find(t,k);
   if (n == NULL) return NULL;
   return (n->finish) ? &(n->data) : NULL;
}

void delete(Trie t, Key k)
{
   TrieNode *n = find(t,k);
   if (n == NULL) return;
   n->finish = 0;
}

void insert(Trie t, Item it)
{
   Key k = key(it);
   TrieNode *n = find(t,k);
   if (n != NULL) {
      n->finish = 1;
      n->data = it; // replaces any existing Item
      return;
   }
   // didn't find a place in trie for this key
   if (t->root == NULL) {
      t->root = newTriNode(k,0,it);
   }
   TrieNode *curr = t->root;
   for (i = 0; k[i] != '\0'; i++) {
      // scan siblings
      if (k[i+1] == '\0') {
         curr->finish = 1;
         curr->data = it;
	  }
      else {
         int j = k[i] - 'a';
         if (curr->child[j] == NULL)
            curr->child[j] = newTrieNode(k,i+1,it);
      curr = curr->child[j]; // move down one level
   }
}
