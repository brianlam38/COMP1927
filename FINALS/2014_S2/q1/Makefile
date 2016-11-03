# COMP1927 14s2 Exam ... Q1

CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2
PROG=q1
OBJS=main.o Matrix.o

$(PROG) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

main.o : main.c Matrix.h
Matrix.o : Matrix.c Matrix.h

clean :
	rm -fr $(PROG) $(OBJS) tests/*.out
