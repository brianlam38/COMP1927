// List manipulation ... not using ADT 
// Primarily to illustrate recursion

#ifndef LIST_H

typedef struct Node *Link;
typedef struct Node {int value; Link next;} Node;
typedef Link List;

#define head(L)  (empty(L) ? -1 : (L)->value)
#define tail(L)  (empty(L) ? NULL : (L)->next)
#define empty(L) ((L) == NULL)

#define show(it) printf("%d",(it))

List append(List, int);

void applyToList(List, int (*)());  // say what!?

#endif
