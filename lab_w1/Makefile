# COMP1927 13s1 Week 02 Lab

CC=gcc
CFLAGS=-Wall -Werror

all: usel randl

usel: useIntList.o IntList.o
	$(CC) -o usel IntList.o useIntList.o

useIntList.o: useIntList.c IntList.h
	$(CC) -c $(CFLAGS) useIntList.c

IntList.o: IntList.c IntList.h
	$(CC) -c $(CFLAGS) IntList.c

randl: randList.c
	$(CC) $(CFLAGS) -o randl randList.c

clean:
	rm -f usel IntList.o useIntList.o randl
