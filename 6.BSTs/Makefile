CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=useT.o BSTree.o

useT : $(OBJS)
	$(CC) -o useT $(OBJS)

useT.o : useT.c BSTree.h

BSTree.o : BSTree.c BSTree.h

clean :
	rm -f useT $(OBJS) core
