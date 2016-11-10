// testList.c
// unit tests for the List.h ADT
// By: Karl Krauth
// Date: 26 may 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

void test1 (void);
void testGetItem (void);
void testInsertItem (void);
void testDeleteItem (void);
void testDeleteEven (void);
void testMerge (void);
static List makeTestList(void);

int main(int argc, char *argv[]) {
    printf ("Testing List.h ADT\n");
    test1();
    printf("Provided functions passed!\n");
    testGetItem();
    printf("getItem() passed!\n");
    testInsertItem();
    printf("insertItem() passed!\n");
    testDeleteItem();
    printf("deleteItem() passed!\n");
    testDeleteEven();
    printf("deleteEven() passed!\n");
    testMerge();
    printf ("All tests passed, you are awesome!\n");
    printf("But you might want to write some of your own tests too.\n");
    return EXIT_SUCCESS;
}

static List makeTestList(void) {
    List testList = newList();
    addFront (testList, 6);
    addFront (testList, 5);
    addFront (testList, 4);
    addFront (testList, 3);
    addFront (testList, 2);
    addFront (testList, 1);
    addFront (testList, 0);
    return testList;
}

void test1 (void) {
    List testList = makeTestList();
    assert( length(testList) == 7);
    printList (testList);
    addRear(testList, 7);
    addRear(testList, 8);
    assert( length(testList) == 9);
    disposeList(testList);

}
void testGetItem (void) {
    List testList = makeTestList();
    assert( getItem(testList, 0) == 0);
    assert( getItem(testList, 1) == 1);
    assert( getItem(testList, 2) == 2);
    assert( getItem(testList, 3) == 3);
    assert( getItem(testList, 4) == 4);
    assert( getItem(testList, 5) == 5);
    assert( getItem(testList, 6) == 6);
    disposeList(testList);
}

void testInsertItem (void) {
    List testList = newList();
    insertItem(testList, 0, 0);
    insertItem(testList, 1, 1);
    insertItem(testList, -1, 2);
    insertItem(testList, 2, -1);
    insertItem(testList, 100, 100);
    assert( length(testList) == 4);
    assert( getItem(testList, 0) == 0);
    assert( getItem(testList, 1) == 1);
    assert( getItem(testList, 2) == -1);
    assert( getItem(testList, 3) == 100);
    printList (testList);
    disposeList(testList);
}

void testDeleteItem (void) {
    List testList = makeTestList();
    deleteItem(testList, 0);
    assert( length(testList) == 6);
    deleteItem(testList, 5);
    assert( length(testList) == 5);
    printList (testList);
    deleteItem(testList, -1);
    assert( length(testList) == 5);
    deleteItem(testList, 100);
    assert( length(testList) == 5);
    disposeList(testList);
}

void testDeleteEven (void) {
    // Testing a generic list
    List testList = makeTestList();
    deleteEven(testList);
    assert( length(testList) == 3);
    assert( getItem(testList, 0) == 1);
    assert( getItem(testList, 1) == 3);
    assert( getItem(testList, 2) == 5);
    disposeList(testList);

    // Testing an empty list
    testList = NULL;
    deleteEven(testList);
    assert( length(testList) == 0);

    // Testing an odd list
    testList = newList();
    addFront (testList, 5);
    addFront (testList, 3);
    addFront (testList, 1);
    assert( length(testList) == 3);
    deleteEven(testList);
    assert( length(testList) == 3);
    disposeList(testList);

    // Testing an even list
    testList = newList();
    addFront (testList, 4);
    addFront (testList, 2);
    addFront (testList, 0);
    assert( length(testList) == 3);
    deleteEven(testList);
    assert( length(testList) == 0);
    disposeList(testList);
}


void testMerge (void) {
    // Testing a generic list
    List l1 = makeTestList();
    List l2 = makeTestList();
    merge(l1, l2);
    assert( length(l1) == 14);
    assert( getItem(l1, 6) == 6);
    assert( getItem(l1, 7) == 0);
    assert( getItem(l1, 13) == 6);
    assert( length(l2) == 0);

    // Think of some more test cases that might break merge

    disposeList(l1);
    disposeList(l2);
}

