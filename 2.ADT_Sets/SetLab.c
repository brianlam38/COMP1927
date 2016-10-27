// SetLab.c ... client for Set ADT
// Written by John Shepherd, August 2015

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "Set.h"

void help();
int  toIndex(char);
int  toValue(char *);

int main(int argc, char *argv[])
{
	Set  sets[26];  // Sets, referenced by 'a'..'z'
	char line[50];  // command line buffer
	char a[4][10];  // command line args
	int  nargs;     // how many cmd line args
	int  val;       // input val
	int  s, t, u;   // set indexes

	// to keep compiler happy
	s = (t = (u = 0));

	// create a bunch of empty sets
	char c;
	for (c = 'a'; c <= 'z'; c++) {
		s = toIndex(c);
		sets[s] = newSet();
	}

	// read and execute commands
	printf("\n> ");
	while (fgets(line,50,stdin) != NULL) {
		nargs = sscanf(line, "%s %s %s %s",
		               a[0], a[1], a[2], a[3]);
		               // note: a[0] == &a[0][0], etc.
		
		/**
		int i;
		for (i = 0; i < nargs; i++)
			printf("a[%d] = %s\n", i, a[i]);
		**/

		switch (tolower(a[0][0])) {
		case 's': // s S = show
			if (nargs < 2)
				printf("Usage: d Set\n");
			else {
				s = toIndex(a[1][0]);
				printf("Set %c: ",a[1][0]);
				showSet(sets[s]);
			}
			break;
		case 'i': // i V S = insert
			if (nargs < 3)
				printf("Usage: i Val Set\n");
			else {
				val = toValue(a[1]);
				s = toIndex(a[2][0]);
				SetInsert(sets[s],val);
				printf("Set %c: ",a[2][0]);
				showSet(sets[s]);
			}
			break;
		case 'd': // d V S = remove
			if (nargs < 3)
				printf("Usage: d Val Set\n");
			else {
				val = toValue(a[1]);
				s = toIndex(a[2][0]);
				SetDelete(sets[s],val);
				printf("Set %c: ",a[2][0]);
				showSet(sets[s]);
			}
			break;
		case 'm': // m V S = member
			if (nargs < 3)
				printf("Usage: m Val Set\n");
			else {
				val = toValue(a[1]);
				s = toIndex(a[2][0]);
				printf(SetMember(sets[s],val)? "yes" : "no");
			}
			break;
		case 'c': // c S = cardinality
			if (nargs < 2)
				printf("Usage: c Set\n");
			else {
				s = toIndex(a[1][0]);
				val = SetCard(sets[s]);
				printf("Set %c has %d members\n",a[1][0],val);
			}
			break;
		case '=': // = S T copy
			if (nargs < 2)
				printf("Usage: = Set1 Set2\n");
			else {
				s = toIndex(a[1][0]);
				t = toIndex(a[2][0]);
				if (s != t) {
					dropSet(sets[t]);
					sets[t] = SetCopy(sets[s]);
				}
				printf("Set %c: ", a[2][0]);
				showSet(sets[u]);
			}
			break;
		case '+': // + S T R = union
			if (nargs < 4)
				printf("Usage: + Set1 Set2 Set3\n");
			else {
				s = toIndex(a[1][0]);
				t = toIndex(a[2][0]);
				u = toIndex(a[3][0]);
				dropSet(sets[u]);
				sets[u] = SetUnion(sets[s],sets[t]);
				printf("Set %c: ", a[3][0]);
				showSet(sets[u]);
			}
			break;
		case '*': // * S T R = intersection
			if (nargs < 4)
				printf("Usage: * Set1 Set2 Set3\n");
			else {
				s = toIndex(a[1][0]);
				t = toIndex(a[2][0]);
				u = toIndex(a[3][0]);
				dropSet(sets[u]);
				sets[u] = SetIntersect(sets[s],sets[t]);
				printf("Set %c: ", a[3][0]);
				showSet(sets[u]);
			}
			break;
		case 'r': // r F S = read values
			if (nargs < 3)
				printf("Usage: r File Set\n");
			else {
				FILE *f;
				if ((f = fopen(a[1],"r")) == NULL)
					printf("Invalid file: %s\n",a[1]);
				else {
					s = toIndex(a[2][0]);
					readSet(f,sets[s]);
					printf("Set %c: ", a[2][0]);
					showSet(sets[s]);
				}
			}
			break;
		case 'q':
			return EXIT_SUCCESS;
			break;
		default:
			help();
			break;
		}
		printf("\n> ");
	}
	return EXIT_SUCCESS;
}

// Convert a set name (e.g. 'a')
// into an index in the sets[] array
int  toIndex(char ch)
{
	if (islower(ch))
		return ch-'a';
	else if (isupper(ch))
		return ch-'A';
	else
		return 0; // lazy!
}

// Convert a string of digits
// into an integer value (using sscanf)
int  toValue(char *str)
{
	int n;
	if (sscanf(str, "%d", &n) == 1)
		return n;
	else
		return 0; // lazy!
}

// Display a help message
void help()
{
	printf("Commands:\n");
	printf("s S = show Set S\n");
	printf("i V S = insert value V in Set S\n");
	printf("d V S = remove value V from Set S\n");
	printf("m V S = check if value V is in Set S\n");
	printf("c S = cardinality of Set S (#elems)\n");
	printf("= S T = copy Set S to set T\n");
	printf("+ S T R = put (S Union T) in Set R\n");
	printf("* S T R = put (S Intersect T) in Set R\n");
	printf("r F S = read values from file F into Set S\n");
	printf("q = quit\n");
}
