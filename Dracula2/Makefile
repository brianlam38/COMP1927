CC = gcc
CFLAGS = -Wall -Werror -g
BINS = dracula hunter
OBJS = GameView.o Map.o Places.o commonFunctions.o
LIBS =

all : $(BINS)


dracula : dracPlayer.o dracula.o DracView.o $(OBJS) $(LIBS)
hunter : hunterPlayer.o hunter.o HunterView.o $(OBJS) $(LIBS)

dracPlayer.o : player.c Game.h DracView.h dracula.h
	$(CC) $(CFLAGS) -DI_AM_DRACULA -c player.c -o dracPlayer.o

hunterPlayer.o : player.c Game.h HunterView.h hunter.h
	$(CC) $(CFLAGS) -c player.c -o hunterPlayer.o


dracula.o : dracula.c Game.h DracView.h commonFunctions.c
hunter.o : hunter.c Game.h HunterView.h commonFunctions.c
Places.o : Places.c Places.h
Map.o : Map.c Map.h Places.h
GameView.o : GameView.c GameView.h Globals.h commonFunctions.c
HunterView.o : HunterView.c HunterView.h Globals.h commonFunctions.c
DracView.o : DracView.c DracView.h Globals.h commonFunctions.c
commonFunctions.o : commonFunctions.c commonFunctions.h

clean :
	rm -f $(BINS) *.o core
