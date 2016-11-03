# COMP1927 14s2 Exam ... Q3

CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2
PROG=q3
OBJS=main.o Graph.o

$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)


main.o : main.c Graph.h
Graph.o : Graph.c Graph.h

clean :
	rm -fr $(PROG) *.o tests/*.out
