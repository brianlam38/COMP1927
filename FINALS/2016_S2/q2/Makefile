# Makefile for COMP1927 15s2 Exam Question Q3

CC=gcc
CFLAGS=-std=c99 -Wall -Werror
OBJS=main.o Graph.o Queue.o Stack.o PQueue.o

q2 : $(OBJS)
	$(CC) -o q2 $(OBJS)

main.o : main.c Graph.h

Graph.o : Graph.c Graph.h Queue.h
Queue.o : Queue.c Queue.h Item.h
Stack.o : Stack.c Stack.h Item.h
PQueue.o : PQueue.c PQueue.h Item.h

clean :
	rm -f q2 $(OBJS) core
