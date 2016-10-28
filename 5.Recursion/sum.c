// iterative

int sum(List L)
{
	int sum = 0;
	Link cur = L;
	while (cur != NULL) {
		sum += cur->value;
		cur = cur->next;
	}
	return sum;
}

// recursive

int sum(List L)
{
	if (empty(L))
		return 0;
	else
		return head(L) + sum(tail(L));
}
