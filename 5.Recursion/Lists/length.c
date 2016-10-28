// recursive version

int length(List L)
{
	if (empty(L)) // base case
		return 0;
	else
		return 1 + length(tail(L));
}


// iterative version

int length(List L)
{
	// TODO
}
