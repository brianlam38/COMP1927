// Test file for list.c implementation
// You should write your own tests as I am only human
// And therefore flawed
// Plus it is about 2am and I have a cold
// By: Tara Tjandra
// Date: 5/8/2016

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"

void testMergeSorted(void);
void testMergeUnsorted(void);

int main(int argc, char *argv[]) {
    testMergeSorted();
    // Uncomment to try the bonus question
    //testMergeUnsorted();

    printf("All tests passed!\n");
    return EXIT_SUCCESS;
}

void testMergeSorted(void) {
    printf("********** Testing mergeSorted() **********\n");
    List l1 = newList();
    List l2 = newList();

    printf("Test 1: Testing two empty lists...\n");
    mergeSorted(l1, l2);
    assert(length(l1) == 0);
    assert(length(l2) == 0);
    printf("Test 1 passed!\n");

    printf("Test 2: Testing single empty lists...\n");
    prepend(l1, 0);
    mergeSorted(l1, l2);
    assert(length(l1) == 1);
    assert(length(l2) == 0);
    freeList(l1);
    prepend(l2, 0);
    mergeSorted(l1, l2);
    assert(length(l1) == 1);
    assert(length(l2) == 0);
    freeList(l1);
    printf("Test 2 passed!\n");

    printf("Test 3: Testing lists of equal length...\n");
    prepend(l1, 0);
    prepend(l2, 1);
    mergeSorted(l1, l2);
    assert(length(l1) == 2);
    assert(length(l2) == 0);
    assert(isSorted(l1));
    freeList(l1);
    prepend(l1, 1);
    prepend(l1, 1);
    prepend(l2, 2);
    prepend(l2, 2);
    mergeSorted(l1, l2);
    assert(length(l1) == 4);
    assert(length(l2) == 0);
    assert(isSorted(l1));
    freeList(l1);
    prepend(l1, 5);
    prepend(l1, 3);
    prepend(l1, 1);
    prepend(l2, 6);
    prepend(l2, 4);
    prepend(l2, 2);
    mergeSorted(l1, l2);
    assert(length(l1) == 6);
    assert(length(l2) == 0);
    assert(isSorted(l1));
    freeList(l1);
    printf("Test 3 passed!\n");

    printf("Test 4: Testing l2 nodes as new head...\n");
    prepend(l2, 0);
    prepend(l1, 1);
    mergeSorted(l1, l2);
    assert(length(l1) == 2);
    assert(length(l2) == 0);
    assert(isSorted(l1));
    freeList(l1);
    prepend(l2, 3);
    prepend(l2, 2);
    prepend(l2, 1);
    prepend(l1, 6);
    prepend(l1, 5);
    prepend(l1, 4);
    mergeSorted(l1, l2);
    assert(length(l1) == 6);
    assert(length(l2) == 0);
    assert(isSorted(l1));
    freeList(l1);
    printf("Test 4 passed!\n");

    printf("Test 5: Testing lists of uneven size...\n");
    prepend(l2, 2);
    prepend(l1, 1);
    prepend(l2, 0);
    mergeSorted(l1, l2);
    assert(length(l1) == 3);
    assert(length(l2) == 0);
    assert(isSorted(l1));
    freeList(l1);
    prepend(l2, 1);
    prepend(l1, 6);
    prepend(l1, 5);
    prepend(l1, 4);
    mergeSorted(l1, l2);
    assert(length(l1) == 4);
    assert(length(l2) == 0);
    assert(isSorted(l1));
    freeList(l1);
    prepend(l2, 1);
    prepend(l2, 0);
    prepend(l1, 6);
    prepend(l1, 5);
    prepend(l1, 4);
    prepend(l1, 3);
    prepend(l1, 2);
    mergeSorted(l1, l2);
    assert(length(l1) == 7);
    assert(length(l2) == 0);
    assert(isSorted(l1));
    freeList(l1);
    printf("Test 5 passed!\n");

    destroyList(l1);
    destroyList(l2);
    printf("All mergeSorted() tests passed!\n");
}


void testMergeUnsorted(void) {
    printf("********** Testing mergeUnsorted() **********\n");
    // Testing a generic list
    List l1 = newList();
    List l2 = newList();

    mergeUnsorted(l1, l2);
    assert(length(l1) == 0);
    assert(length(l2) == 0);
    assert(isSorted(l1));

    prepend(l1, 1);
    prepend(l1, 8);
    prepend(l1, 2);
    prepend(l1, 5);
    prepend(l1, 6);
    prepend(l1, 3);
    prepend(l1, 2);
    prepend(l2, 0);
    prepend(l1, 1);
    prepend(l1, 2);
    mergeUnsorted(l1, l2);
    assert(length(l1) == 10);
    assert(length(l2) == 0);
    assert(isSorted(l1));

    // Assuming you can get this far, you're probably ok
    destroyList(l1);
    destroyList(l2);
    printf("All mergeUnsorted() tests passed!\n");
}


