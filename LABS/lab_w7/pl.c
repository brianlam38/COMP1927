#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Places.h"

int main(int argc, char **argv)
{
	int id;

	if (argc < 2) {
		fprintf(stderr,"Usage %s Place (name or abbrev)\n", argv[0]);
		exit(1);
	}
	if (strlen(argv[1]) > 2) {
		id = nameToID(argv[1]);
		if (id != NOWHERE)
			printf("%s has ID %d\n", argv[1], id);
	}
	else {
		id = abbrevToID(argv[1]);
		if (id != NOWHERE)
			printf("%s is %s (%d)\n", argv[1], idToName(id), id);
	}
	if (id == NOWHERE)
		printf("Invalid place '%s'\n", argv[1]);

	return 0;
}
