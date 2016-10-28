// #####################
// FIND LENGTH OF A LIST
// #####################

// RECURSIVE VERSION
int length(List L)
{
	if (empty(L)) 					// base case
		return 0;
	else
		return 1 + length(tail(L));	// iteratively count length
}

// ITERATIVE VERSION
int length(List L)
{
	if (empty(L))
		return 0;
	else {
		Link curr;
		int length;
		for (curr = L->head; curr != NULL; curr = curr->next) {
			length++;
		}
		return length;
	}
}
