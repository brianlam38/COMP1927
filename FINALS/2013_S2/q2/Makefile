CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2

q2 : q2.o List.o
	$(CC) $(CFLAGS) -o q2 q2.o List.o

q2.o : q2.c List.h
List.o : List.c List.h

test : q2
	check q2

clean :
	rm -fr q2 *.o q2.dSYM tests/*.out
