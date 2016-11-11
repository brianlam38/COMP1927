# Makefile for COMP1927 15s2 Exam Question QX

CC=gcc
CFLAGS=-std=c99 -Wall -Werror

q1 : main.o BSTree.o
	$(CC) -o q1 main.o BSTree.o

main.o : main.c BSTree.h

BSTree.o : BSTree.c BSTree.h

clean :
	rm -f q1 *.o core
