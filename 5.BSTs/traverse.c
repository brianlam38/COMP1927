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
