// stack.h ... interface to simple Stack of Strings
// Written by John Shepherd, September 2015

#ifndef STACK_H
#define STACK_H

typedef struct StackRep *Stack;

// Function signatures

Stack newStack();
void  disposeStack(Stack);
void  pushOnto(Stack,char *);
char  *popFrom(Stack);
int   emptyStack(Stack);
void  showStack(Stack s);

#endif
