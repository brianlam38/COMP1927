CC=gcc
CFLAGS=-Wall -Werror -g
BINS=lst

lists : lists.o List.o
	$(CC) -o lst lists.o List.o

lists.o : lists.c List.h
List.o : List.c List.h

clean :
	rm -fr $(BINS) *.o core *.dSYM
