# Makefile for COMP1917 15s2 Week 08 Lab

CC=gcc
CFLAGS=-Wall -Werror -g
LIBS=url_file.o html.o set.o queue.o graph.o stack.o
TESTS=ts tq tk tg

crawl : crawl.o $(LIBS)
	gcc -g -o crawl crawl.o $(LIBS) -lcurl

tests : $(TESTS)

url_file.o : url_file.c url_file.h
html.o : html.c html.h
set.o : set.c set.h
queue.o : queue.c queue.h
stack.o : stack.c stack.h
graph.o : graph.c graph.h

ts : ts.o set.o
	gcc -o ts ts.o set.o
ts.o : ts.c set.h

tq : tq.o queue.o
	gcc -o tq tq.o queue.o
tq.o : tq.c queue.h

tk : tk.o stack.o
	gcc -o tk tk.o stack.o
tk.o : tk.c stack.h

tg : tg.o graph.o
	gcc -o tg tg.o graph.o
tg.o : tg.c graph.h

clean :
	rm -f crawl $(TESTS) *.o core
