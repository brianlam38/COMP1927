// Test Stack ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Stack.h"

int main(int argc, char **argv)
{
	Stack s;

	s = newStack();
	assert(s != NULL);

}
