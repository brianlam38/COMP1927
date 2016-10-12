CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=tlab.o Tree.o

tlab : $(OBJS)
	$(CC) -o tlab $(OBJS)

tlab.o : tlab.c Tree.h

Tree.o : Tree.c Tree.h

clean :
	rm -f tlab $(OBJS) core
