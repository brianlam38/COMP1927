mkp : mkprefix.c
	gcc -Wall -Werror -std=c99 -o mkp mkprefix.c

mktree : mktree.o ../TreeLab/Tree.o
	gcc -o mktree mktree.o ../TreeLab/Tree.o

mktree.o : mktree.c ../TreeLab/Tree.h
	gcc -Wall -Werror -c mktree.c
	
