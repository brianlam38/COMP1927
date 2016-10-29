#include <stdio.h>
#include "Queue.h"

// LEVEL-ORDER TRAVERSAL (Breadth-First) and show values
void showBSTreeBF(BSTree t)
{
	if (t == NULL) return;	// #1: Empty tree

	Queue q = newQueue();	// #2: Create queue
	Link curr;
	enQueue(q, t);
									// #3 Queue operations
	while (curr = deQueue(q)) { 	//    deQueue curr node
		printf("%d ", curr->value); //	  print node
		if (curr->left != NULL)
			enQueue(q, curr->left); //    Go to LHS -> Add LHS to queue
		if (curr->right != NULL)
			enQueue(q, curr->right); //   Go to RHS -> Add RHS to queue
	}
	printf("\n"); // Queue empty
}