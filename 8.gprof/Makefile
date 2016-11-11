CC=gcc
CFLAGS=-Wall -Werror -pg
OBJS=tlab.o Tree.o

tlab : $(OBJS)
	$(CC) -o tlab $(OBJS) -pg

tlab.o : tlab.c Tree.h

Tree.o : Tree.c Tree.h

clean :
	rm -f tlab $(OBJS) core
