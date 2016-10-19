# COMP1927 15s2 Week 12 Lab

CC=gcc
CFLAGS=-Wall -Werror -pg

all: words mkwords

words: words.o HashTable.o List.o Item.o
	$(CC) $(CFLAGS) -o words words.o HashTable.o List.o Item.o

mkwords: mkwords.c
	$(CC) -Wall -Werror -o mkwords mkwords.c

HashTable.o: HashTable.c HashTable.h
List.o: List.c List.h
Item.o: Item.c Item.h

clean:
	rm -fr words mkwords *.o core *.dSYM gmon.out
