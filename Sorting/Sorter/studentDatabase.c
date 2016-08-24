#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
}

typedef struct _Student {
	int id;			// unique number (key)
	char name[20];  // student name
	int program;    // enrolled program
} Student

//Defining our generic sort functions

//Sort by IDs
sort(a,0,N-1,compareIDs);
//Sort by name
sort(a,0,N-1,compareNames);
//Sort by program
sort(a,0,N-1,compareProgramThenName);