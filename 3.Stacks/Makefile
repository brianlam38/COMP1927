# Make Stack ADT

CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2

test : test.o Stack.o

test.o : test.c

test.c : Stack.h

Stack.o : Stack.c

Stack.c : Stack.h

clean :
	rm -f test stack.o test.o
