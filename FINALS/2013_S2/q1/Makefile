CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2

q1 : q1.c
	$(CC) $(CFLAGS) -o q1 q1.c

test : q1
	check q1

clean :
	rm -fr q1 q1.dSYM tests/*.out
