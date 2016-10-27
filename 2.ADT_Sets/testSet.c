// testSet.c ... client to test Set ADT

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Set.h"

int main(int argc, char *argv[])
{
	Set s1, s2, s3;
	int i;
    
	s1 = newSet();
	for (i = 1; i < 6; i++) SetInsert(s1,i);

	s2 = newSet();
	for (i = 4; i < 10; i++) SetInsert(s2,i);

	printf("s1:"); showSet(s1); printf("\n");
	printf("s2:"); showSet(s2); printf("\n");
	printf("Cardinality(s1): %d\n", SetCard(s1));
	assert(SetCard(s1) == 5);
	printf("Cardinality(s2): %d\n", SetCard(s2));
	assert(SetCard(s2) == 6);
	s3 = SetUnion(s1,s2);
	printf("Union:"); showSet(s3); printf("\n");
	assert(SetCard(s3) == 9);
	dropSet(s3); // avoid memory leak
	s3 = SetIntersect(s2,s1);
	printf("Intersect:"); showSet(s3); printf("\n");
	assert(SetCard(s3) == 2);
	// more tests needed ...
	return 0;
}
