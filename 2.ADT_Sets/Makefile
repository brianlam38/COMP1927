# Makefile for Set ADT

CC = gcc
CFLAGS = -Wall -Werror -g

all : ts slab uniq

ts : testSet.o Set.o
	$(CC) -o ts testSet.o Set.o

slab : SetLab.o Set.o
	$(CC) -o slab SetLab.o Set.o

uniq : uniq.o Set.o
	$(CC) -o uniq uniq.o Set.o

testSet.o : testSet.c Set.h
	$(CC) $(CFLAGS) -c testSet.c

SetLab.o : SetLab.c Set.h
	$(CC) $(CFLAGS) -c SetLab.c

uniq.o : uniq.c Set.h
	$(CC) $(CFLAGS) -c uniq.c

Set.o : Set.c Set.h Bool.h

clean :
	rm -f *.o slab ts uniq core

