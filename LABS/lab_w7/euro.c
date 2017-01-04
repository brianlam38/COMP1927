// Build and display Map/Graph of Europe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include "Places.h"

int main(int argc, char **argv)
{
	Map europe;
	europe = newMap();
	printf("Map of Europe\n=============\n");
	showMap(europe);
	return 0;
}
