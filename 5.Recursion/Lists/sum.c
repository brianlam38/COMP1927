// ######################
// EXERCISE: SUM OF ITEMS
// ######################

INSTRUCTIONS
Write functions to sum the values in a list
- Write an iterative version
- Write a recursive version
Each function has the same prototype: int sum(List);


// RECURSIVE VERSION
int sum(List L)
{
	if (L->head == NULL) {
		return 0;
	}
	int sum;
	sum += sum(L->next);
	return sum;
}

// ITERATIVE VERSION
int sum(List L)
{
	Link curr;
	int sum = 0;
	for (curr = L->head; curr != NULL; curr = curr->next) {
		 sum += curr->value;
	}
	return sum;
}
