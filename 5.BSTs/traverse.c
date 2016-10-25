#include <stdio.h>
#include "Queue.h"

void showBSTreeBF(BSTree t)
{
	if (t == NULL) return;
	Queue q = newQueue();
	Link curr;
	enQueue(q, t);
	while (curr = deQueue(q)) {
		printf("%d ", curr->value);
		if (curr->left != NULL)
			enQueue(q, curr->left);
		if (curr->right != NULL)
			enQueue(q, curr->right);
	}
	printf("\n");
}

/* 	Traversal with visit operation
	void(*visit)(item) = Do something when we visit an item
	Example: Print something out, compute something etc.
	Visit function: Takes item, does something to it, no returns.
*/
void Traverse(Tree t, void (*visit)(Item)) {
	if (t != NULL) {
		// visit data in root node FIRST = NLR (prefix)
		Traverse(t->left, visit);
		// visit data in root node MIDDLE = LNR (infix)
		Traverse(t->right,visit);
		// visit data in root node LAST = LRN (postfix)
 	}
 	// Level-order cannot be implemented recursively
}

/* Passing ptr into function

   void(*visit)(item)

   *visit = ptr to a function with parameter of type (Item)
   void(*visit)(item) = the function returns nothing
*/



