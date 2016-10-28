// ######################
// EXERCISE: SUM OF ITEMS
// ######################

INSTRUCTIONS
Write functions to sum the values in a list
- Write an iterative version
- Write a recursive version
Each function has the same prototype: int sum(List);


// ################
// BRIAN'S VERSIONS
// ################

// RECURSIVE VERSION (Brian's ver)
int sum(List L)
{
	if (L->head == NULL) {
		return 0;
	}
	int sum;
	if (L->next != NULL) {
		sum += sum(L->next);		
	}
	return sum;
}

// ITERATIVE VERSION (correct same as Jas' ver)
int sum(List L)
{
	Link curr;
	int sum = 0;
	for (curr = L->head; curr != NULL; curr = curr->next) {
		 sum += curr->value;
	}
	return sum;
}

// #############
// JAS' VERSIONS
// #############

// RECURSIVE VERSION
// Remember:
// head(L) = gives value in first node of L
// tail(L) = gives "rest of" list L after first node
// empty(L) = list is empty (base case)
int sum(List L)
{
	if (empty(L))
		return 0;
	else
		return head(L) + sum(tail(L));
}




