# COMP1927 14s2 Exam ... Q2

CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2
PROG=q2
OBJS=main.o HashTable.o List.o

$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

main.o : main.c HashTable.h
HashTable.o : HashTable.c HashTable.h List.h
List.o : List.c List.h

clean :
	rm -fr $(PROG) $(OBJS) tests/*.out
