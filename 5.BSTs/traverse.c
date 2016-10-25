#include <stdio.h>
#include "Queue.h"

// LEVEL-ORDER TRAVERSAL (Breadth-First) and show values
void showBSTreeBF(BSTree t)
{
	if (t == NULL) return;	// empty tree
	Queue q = newQueue();	// create queue
	Link curr;
	enQueue(q, t);					// add root node to queue
	while (curr = deQueue(q)) { 	// take front elt off queue
		printf("%d ", curr->value); // print deQueued nodes
		if (curr->left != NULL)
			enQueue(q, curr->left); // add LHS child to queue
		if (curr->right != NULL)
			enQueue(q, curr->right); // add RHS child to queue
	}
	// Loop ends queue is empty now
	printf("\n");
}

/* 	Traversal with visit operation
	void(*visit)(item) = Do something when we visit an item
	Example: Print something out, compute something etc.
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

/* 	Passing ptr into function
   	E.g. void Traverse(Tree t, void (*visit)(item));

	Visit is a FN that takes in an Item and doesn't return
	anything. We're giving the BSTreeTraverse function a
	ptr to the visit function.
*/




