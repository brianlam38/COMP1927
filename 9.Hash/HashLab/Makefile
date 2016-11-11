CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=hlab.o HashTable.o

hlab : $(OBJS)
	$(CC) -o hlab $(OBJS)

hlab.o : hlab.c HashTable.h

HashTable.o : HashTable.c HashTable.h

clean :
	rm -f hlab $(OBJS) core *.dSYM
