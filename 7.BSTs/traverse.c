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
