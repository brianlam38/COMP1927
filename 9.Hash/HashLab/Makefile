CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=hlab.o HashTable.o List.o

hlab : $(OBJS)
	$(CC) -o hlab $(OBJS)

hlab.o : hlab.c HashTable.h List.h

HashTable.o : HashTable.c HashTable.h Item.h

List.o : List.c List.h Item.h

clean :
	rm -f hlab $(OBJS) core *.dSYM
