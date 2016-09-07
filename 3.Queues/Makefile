CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2

tq : test.o Queue.o
	$(CC) -o tq test.o Queue.o

test.o : test.c Queue.h

Queue.o : Queue.c Queue.h

clean :
	rm -f tq *.o
