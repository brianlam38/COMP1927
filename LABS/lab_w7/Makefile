CC=gcc
CFLAGS=-Wall -Werror -g
BINS=pl euro conn

all : $(BINS)

pl : pl.o Places.o
pl.o : pl.c Places.h

euro : euro.o Map.o Places.o
euro.o : euro.c Map.c Places.h

conn : conn.o Map.o Places.o
conn.o : conn.c Map.c Places.h

Places.o : Places.c Places.h
Map.o : Map.c Map.h Places.h
Queue.o : Queue.c Queue.h Item.h

clean :
	rm -f $(BINS) *.o core *.dSYM
