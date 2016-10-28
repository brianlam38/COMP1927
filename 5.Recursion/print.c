void putListRev(List L)
{
	if (empty(L))
		/*nothing to do*/;
	else {
		putListRev(tail(L));
		printf("%d\n",head(L));
	}
}
void putList(List L)
{
   if (empty(L))
     printf("\n");
   else if (empty(tail(L))) {
     show(head(L));
     printf("\n");
   }
   else {
     show(head(L));
     printf(",");
     putList(tail(L));
   }
}
void putList(List L)
{
	Link cur = L;
	while (cur != NULL) {
		show(cur->value);
		if (cur->next != NULL) printf(",");
		cur = cur->next;
	}
	printf("\n");
}
