CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=travel.o Graph.o Queue.o

travel : $(OBJS)
	$(CC) -o travel $(OBJS)

travel.o : travel.c Graph.h
Graph.o : Graph.c Graph.h
Queue.o : Queue.c Queue.h Item.h

clean :
	rm -f travel $(OBJS) core *.dSYM
