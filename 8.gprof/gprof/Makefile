CC = gcc
CFLAGS = -pg

xwords: xwords.c
	$(CC) $(CFLAGS) -o xwords xwords.c

funcs: funcs.c
	$(CC) $(CFLAGS) -o funcs funcs.c

clean:
	/bin/rm -f xwords funcs *.o gmon.out core a.out
