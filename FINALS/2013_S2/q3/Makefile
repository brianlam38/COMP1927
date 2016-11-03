CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2

q3 : q3.o Graph.o
	$(CC) $(CFLAGS) -o q3 q3.o Graph.o

q3.o : q3.c Graph.h
Graph.o : Graph.c Graph.h

test : q3
	check q3

clean :
	rm -fr q3 *.o q3.dSYM tests/*.out
