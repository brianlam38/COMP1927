# Makefile for COMP1927 15s2 Exam Question Q3

CC=gcc
CFLAGS=-std=c99 -Wall -Werror

q2 : main.o Trie.o
	$(CC) -o q2 main.o Trie.o

main.o : main.c Item.h Trie.h

Trie.o : Trie.c Item.h

clean :
	rm -f q2 *.o core
