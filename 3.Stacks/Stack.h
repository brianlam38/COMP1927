// Stack ADT interface

typedef char Item;
typedef struct StackRep *Stack;

// add item onto stack
void push(Stack s, Item it); 
// remove item from stack
Item pop(Stack s); 
// stack contains no items?
int isEmpty(Stack s); 
// how many items in stack
int nItems(Stack s); 
// display stack on stdout
void show(Stack s); 
// create new empty stack
Stack newStack(); 
// release stack data
void dropStack(Stack s); 
