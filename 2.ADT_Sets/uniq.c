// uniq.c ... eliminate duplicates from stdin

#include <stdlib.h>
#include <stdio.h>
#include "Set.h"

int main(int argc, char **argv)
{
	// foreach number in stdin
	//    if seen already, ignore
	//    otherwise, write to stdout
	
	Set alreadySeen = newSet();
	int n;
	char line[BUFSIZ];
	while (fgets(line,BUFSIZ,stdin) != NULL) {
		if (sscanf(line,"%d", &n) != 1) continue;
		// process current number
		if (SetMember(alreadySeen,n)) continue;
		printf("%d\n",n);
		SetInsert(alreadySeen,n);
		showSet(alreadySeen); putchar('\n');
	}

	return EXIT_SUCCESS;
}
