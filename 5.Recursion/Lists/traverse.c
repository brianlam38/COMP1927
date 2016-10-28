void printOneNode(Link node) { printf("%d ",node->value); }

void doubleIt(Link node) { node->value *= 2; }

void showList(List L) { traverse(L, &printOneNode); }

void showListI(List L) { traverseI(L, &printOneNode); }

void doubleAll(List L) { traverse(L, &doubleIt); }
