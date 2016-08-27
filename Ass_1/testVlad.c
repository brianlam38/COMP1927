// gcc -Wall -Werror -O -o vladTester vladTest.c
// By: Jonathon Green
// Desc: Some unit tests for allocator.c
// Let me know if you find any issues! Cheers :)
// Remember that vlad.c is not used here, and we're also being
// dodgy by including allocator.c . Please compile with the above
// commands! (Change names appropriately)


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "allocator.h"
#include "allocator.c"


void unitTests();


int main (int argc, char * argv[]) {
   printf("-- Attempting to test vlad! -- \n\n");
   unitTests();
   printf("All tests successful! Passed! \n\n");
   return EXIT_SUCCESS;
}


void vladInitTests(int inSize) {
   //Some tests that never change for each init
   vlad_init(inSize);


   assert(free_list_ptr == 0);
   assert(memory != NULL); //Unless there was some other system error


   free_header_t *first_head = (free_header_t *)&memory[0];
   assert(first_head->magic == MAGIC_FREE);
   assert(first_head->next == 0);
   assert(first_head->prev == 0);
}


void unitTests() {
   //Vlad init and end functions
   printf("-- Testing vlad init and end -- \n\n");


   printf("-- Allocating 0 < x < 1024 bytes \n");
   vladInitTests(10);
   assert(memory_size == 1024); //Tests some other things :)
   free_header_t *init1_head = (free_header_t *)&memory[0];
   assert(init1_head->size == 1024);
   printf("Deallocating 1024 bytes \n\n");
   vlad_end();


   printf("-- Allocating exactly 1024 bytes \n");
   vladInitTests(1024);
   assert(memory_size == 1024); //Tests some other things :)
   free_header_t *init2_head = (free_header_t *)&memory[0];
   assert(init2_head->size == 1024);
   printf("Deallocating 1024 bytes\n\n");
   vlad_end();


   printf("-- Allocating 0 (should be 1024) bytes \n");
   vladInitTests(0);
   assert(memory_size == 1024); //Tests some other things :)
   free_header_t *init3_head = (free_header_t *)&memory[0];
   assert(init3_head->size == 1024);
   printf("Deallocating 1024 bytes\n\n");
   vlad_end();


   printf("-- Allocating (a power of 2)-1 (65535) (should be 65536) bytes \n");
   vladInitTests(65535);
   assert(memory_size == 65536); //Tests some other things :)
   free_header_t *init4_head = (free_header_t *)&memory[0];
   assert(init4_head->size == 65536);
   printf("Deallocating 65536 bytes\n\n");
   vlad_end();


   printf("-- Allocating (a power of 2)+1 (4097) (should be 8192) bytes \n");
   vladInitTests(4097);
   assert(memory_size == 8192); //Tests some other things :)
   free_header_t *init5_head = (free_header_t *)&memory[0];
   assert(init5_head->size == 8192);
   printf("Deallocating 8192 bytes\n\n");
   vlad_end();


   printf("-- Testing reinitialisation does nothing by allocating twice\n");
   vladInitTests(2000);
   assert(memory_size == 2048); //Tests some other things :)
   free_header_t *init6_head = (free_header_t *)&memory[0];
   assert(init6_head->size == 2048);
   vlad_init(4000); //Should do nothing
   assert(memory_size == 2048);
   free_header_t *init7_head = (free_header_t *)&memory[0];
   assert(init6_head == init7_head);
   printf("Deallocating 2048 bytes\n\n");
   vlad_end();


   printf("-- All vlad init and end tests passed!.. But there's more to come! -- \n\n");


   //Vlad alloc, free and merge functions
   printf("-- Testing vlad alloc, free and merge -- \n\n");
   vlad_init(2048);


   printf("-- Vlad allocate 0 bytes to block 'a' \n"
          "should allocate 8 usable bytes (+ 8 bytes header = 16 total) \n\n");
   byte *vlad1 = vlad_malloc(0);
   alloc_header_t *vlad1_alloc = (alloc_header_t *)&memory[0];
   //Check allocated block
   assert(vlad1 == ((byte*)vlad1_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad1_alloc->magic == MAGIC_ALLOC);
   assert(vlad1_alloc->size == 16);
   //Check adjacent free block
   free_header_t *vlad1_adj_free = (free_header_t *)&memory[16];
   assert(vlad1_adj_free->magic == MAGIC_FREE);
   assert(vlad1_adj_free->size == 2032);
   assert(vlad1_adj_free->next == 16);
   assert(vlad1_adj_free->prev == 16);
   assert(free_list_ptr == 16);


   printf("Vlad deallocate block 'a' and check merge \n\n");
   vlad_free(vlad1);
   //Check only 1 free block
   free_header_t *vlad1_free = (free_header_t *)&memory[0];
   assert(vlad1_free->magic == MAGIC_FREE);
   assert(vlad1_free->size == 2048);
   assert(vlad1_free->next == 0);
   assert(vlad1_free->prev == 0);
   assert(free_list_ptr == 0);


   printf("-- Vlad allocate 33 bytes to block 'a' \n"
          "should allocate 36 usable bytes (+ 8 bytes header = 42 total) \n\n");
   byte *vlad2 = vlad_malloc(33);
   //Check allocated block
   alloc_header_t *vlad2_alloc = (alloc_header_t *)&memory[0];
   assert(vlad2 == ((byte*)vlad2_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad2_alloc->magic == MAGIC_ALLOC);
   assert(vlad2_alloc->size == 44);
   //Check adjacent free block
   free_header_t *vlad2_adj_free = (free_header_t *)&memory[44];
   assert(vlad2_adj_free->magic == MAGIC_FREE);
   assert(vlad2_adj_free->size == 2004);
   assert(vlad2_adj_free->next == 44);
   assert(vlad2_adj_free->prev == 44);
   assert(free_list_ptr == 44);


   printf("-- Attempt to vlad allocate 1965 bytes to block 'b' \n"
          "should return NULL \n\n");
   //Threshold will be 8+1968+32 = 2008 which is smaller than the remaining free size
   //This means it should try to allocate the whole region, however
   //since this leaves no free regions it will fail
   byte *vlad3 = vlad_malloc(1965);
   //Check null return
   assert(vlad3 == NULL);
   //Check that the free block has remained the same
   assert(vlad2_adj_free->magic == MAGIC_FREE);
   assert(vlad2_adj_free->size == 2004);
   assert(vlad2_adj_free->next == 44);
   assert(vlad2_adj_free->prev == 44);
   assert(free_list_ptr == 44);


   printf("-- Vlad allocate 1964 bytes to block 'b' \n"
          "should split the free region into an allocated 1972 and a free 32 blocks \n\n");
   //Threshold will be 8+1964+32 = 2004 equal to remaining free size
   //Should attempt to split
   byte *vlad4 = vlad_malloc(1964);
   //Check allocated block
   alloc_header_t *vlad4_alloc = (alloc_header_t *)&memory[44];
   assert(vlad4 == ((byte*)vlad4_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad4_alloc->magic == MAGIC_ALLOC);
   assert(vlad4_alloc->size == 1972);
   //Check adjacent free block
   free_header_t *vlad4_adj_free = (free_header_t *)&memory[2016];
   assert(vlad4_adj_free->magic == MAGIC_FREE);
   assert(vlad4_adj_free->size == 32);
   assert(vlad4_adj_free->next == 2016);
   assert(vlad4_adj_free->prev == 2016);
   assert(free_list_ptr == 2016);


   printf("-- Vlad deallocate block 'b' \n"
          "should leave just one free region \n\n");
   vlad_free(vlad4);
   //Check free block
   free_header_t *vlad4_free = (free_header_t *)&memory[44];
   assert(vlad4_free->magic == MAGIC_FREE);
   assert(vlad4_free->size == 2004);
   assert(vlad4_free->next == 44);
   assert(vlad4_free->prev == 44);
   assert(free_list_ptr == 44);


   //Time to do some serious allocating!!
   printf("-- Vlad allocate 50 bytes to block 'b' \n"
          "should allocate 52 usable bytes (+ 8 bytes header = 60 total) \n\n");
   byte *vlad5 = vlad_malloc(50);
   //Check allocated block
   alloc_header_t *vlad5_alloc = (alloc_header_t *)&memory[44];
   assert(vlad5 == ((byte*)vlad5_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad5_alloc->magic == MAGIC_ALLOC);
   assert(vlad5_alloc->size == 60);
   //Check adjacent free block
   free_header_t *vlad5_adj_free = (free_header_t *)&memory[104];
   assert(vlad5_adj_free->magic == MAGIC_FREE);
   assert(vlad5_adj_free->size == 1944);
   assert(vlad5_adj_free->next == 104);
   assert(vlad5_adj_free->prev == 104);
   assert(free_list_ptr == 104);


   printf("-- Vlad allocate 100 bytes to block 'c' \n"
          "should allocate 100 usable bytes (+ 8 bytes header = 108 total) \n\n");
   byte *vlad6 = vlad_malloc(100);
   //Check allocated block
   alloc_header_t *vlad6_alloc = (alloc_header_t *)&memory[104];
   assert(vlad6 == ((byte*)vlad6_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad6_alloc->magic == MAGIC_ALLOC);
   assert(vlad6_alloc->size == 108);
   //Check adjacent free block
   free_header_t *vlad6_adj_free = (free_header_t *)&memory[212];
   assert(vlad6_adj_free->magic == MAGIC_FREE);
   assert(vlad6_adj_free->size == 1836);
   assert(vlad6_adj_free->next == 212);
   assert(vlad6_adj_free->prev == 212);
   assert(free_list_ptr == 212);


   printf("-- Vlad allocate 200 bytes to block 'd' \n"
          "should allocate 200 usable bytes (+ 8 bytes header = 208 total) \n\n");
   byte *vlad7 = vlad_malloc(200);
   //Check allocated block
   alloc_header_t *vlad7_alloc = (alloc_header_t *)&memory[212];
   assert(vlad7 == ((byte*)vlad7_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad7_alloc->magic == MAGIC_ALLOC);
   assert(vlad7_alloc->size == 208);
   //Check adjacent free block
   free_header_t *vlad7_adj_free = (free_header_t *)&memory[420]; //Ye dats right 420 :))))
   assert(vlad7_adj_free->magic == MAGIC_FREE);
   assert(vlad7_adj_free->size == 1628);
   assert(vlad7_adj_free->next == 420);
   assert(vlad7_adj_free->prev == 420);
   assert(free_list_ptr == 420);


   //Now comes the deallocation and real test
   printf("-- Vlad deallocate block 'b' \n"
          "should mean there are two free blocks which dont merge \n\n");
   vlad_free(vlad5);
   //Check newly created free block
   free_header_t *vlad5_b_free = (free_header_t *)&memory[44];
   assert(vlad5_b_free->magic == MAGIC_FREE);
   assert(vlad5_b_free->size == 60);
   assert(vlad5_b_free->next == 420);
   assert(vlad5_b_free->prev == 420);
   //Check original other block
   free_header_t *vlad5_b_free2 = (free_header_t *)&memory[420];
   assert(vlad5_b_free2->magic == MAGIC_FREE);
   assert(vlad5_b_free2->size == 1628);
   assert(vlad5_b_free2->next == 44);
   assert(vlad5_b_free2->prev == 44);
   assert(free_list_ptr == 44);


   printf("-- Vlad deallocate block 'a' \n"
          "should mean there are two free blocks, a merge also happens \n\n");
   vlad_free(vlad2);
   //Check newly created free block
   free_header_t *vlad2_a_free = (free_header_t *)&memory[0];
   assert(vlad2_a_free->magic == MAGIC_FREE);
   assert(vlad2_a_free->size == 104);
   assert(vlad2_a_free->next == 420);
   assert(vlad2_a_free->prev == 420);
   //Check original other block
   free_header_t *vlad2_a_free2 = (free_header_t *)&memory[420];
   assert(vlad2_a_free2->magic == MAGIC_FREE);
   assert(vlad2_a_free2->size == 1628);
   assert(vlad2_a_free2->next == 0);
   assert(vlad2_a_free2->prev == 0);
   assert(free_list_ptr == 0);


   printf("-- Vlad allocate two 24 size blocks 'a' and 'b' \n"
          "should allocate two (8 + 24 = ) 32 size blocks next to each other \n\n");
   byte *vlad8 = vlad_malloc(24);
   byte *vlad9 = vlad_malloc(24);
   //Check allocs
   alloc_header_t *vlad8_alloc = (alloc_header_t *)&memory[0];
   assert(vlad8 == ((byte*)vlad8_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad8_alloc->magic == MAGIC_ALLOC);
   assert(vlad8_alloc->size == 32);
   alloc_header_t *vlad9_alloc = (alloc_header_t *)&memory[32];
   assert(vlad9 == ((byte*)vlad9_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad9_alloc->magic == MAGIC_ALLOC);
   assert(vlad9_alloc->size == 32);
   //Check the two frees are correct
   free_header_t *vlad9_a_free = (free_header_t *)&memory[64];
   assert(vlad9_a_free->magic == MAGIC_FREE);
   assert(vlad9_a_free->size == 40);
   assert(vlad9_a_free->next == 420);
   assert(vlad9_a_free->prev == 420);
   //This size shouldnt have changed, only addresses
   free_header_t *vlad9_a_free2 = (free_header_t *)&memory[420];
   assert(vlad9_a_free2->magic == MAGIC_FREE);
   assert(vlad9_a_free2->size == 1628);
   assert(vlad9_a_free2->next == 64);
   assert(vlad9_a_free2->prev == 64);
   assert(free_list_ptr == 64);


   //Now deallocate 'a' to reveal 3 free blocks :O
   printf("-- Vlad deallocate block 'a' \n"
          "should expose 3 free regions at front, back and middle\n\n");
   vlad_free(vlad8);
   //Front which has just been deallocated
   free_header_t *vlad8_a_free = (free_header_t *)&memory[0];
   assert(vlad8_a_free->magic == MAGIC_FREE);
   assert(vlad8_a_free->size == 32);
   assert(vlad8_a_free->next == 64);
   assert(vlad8_a_free->prev == 420);
   //Middle free region, only addresses should change
   free_header_t *vlad8_a_free2 = (free_header_t *)&memory[64];
   assert(vlad8_a_free2->magic == MAGIC_FREE);
   assert(vlad8_a_free2->size == 40);
   assert(vlad8_a_free2->next == 420);
   assert(vlad8_a_free2->prev == 0);
   //Back free region, only addresses should change
   free_header_t *vlad8_a_free3 = (free_header_t *)&memory[420];
   assert(vlad8_a_free3->magic == MAGIC_FREE);
   assert(vlad8_a_free3->size == 1628);
   assert(vlad8_a_free3->next == 0);
   assert(vlad8_a_free3->prev == 64);
   assert(free_list_ptr == 0);


   //Now allocate another block on top to check addresses do change well
   printf("-- Vlad allocate 1300 bytes to block 'e' \n"
             "should be of size 1308 on the last free region \n\n");
   byte *vlad10 = vlad_malloc(1300);
   //Check alloc
   alloc_header_t *vlad10_alloc = (alloc_header_t *)&memory[420];
   assert(vlad10 == ((byte*)vlad10_alloc)+ALLOC_HEADER_SIZE);
   assert(vlad10_alloc->magic == MAGIC_ALLOC);
   assert(vlad10_alloc->size == 1308);
   //Check all the frees addresses have fixed nicely
   //Front region
   free_header_t *vlad10_a_free = (free_header_t *)&memory[0];
   assert(vlad10_a_free->magic == MAGIC_FREE);
   assert(vlad10_a_free->size == 32);
   assert(vlad10_a_free->next == 64);
   assert(vlad10_a_free->prev == 1728);
   //Middle free region, only addresses should change
   free_header_t *vlad10_a_free2 = (free_header_t *)&memory[64];
   assert(vlad10_a_free2->magic == MAGIC_FREE);
   assert(vlad10_a_free2->size == 40);
   assert(vlad10_a_free2->next == 1728);
   assert(vlad10_a_free2->prev == 0);
   //Back free region, only addresses should change
   free_header_t *vlad10_a_free3 = (free_header_t *)&memory[1728];
   assert(vlad10_a_free3->magic == MAGIC_FREE);
   assert(vlad10_a_free3->size == 320); //Damn, we lost 420 to go to 320, rip :(
   assert(vlad10_a_free3->next == 0);
   assert(vlad10_a_free3->prev == 64);
   assert(free_list_ptr == 0);


   //Time to dealloc again and reveal a forth free region!
   printf("-- Vlad deallocate block 'd' \n"
          "should have four free regions (how cool!..)\n\n");
   vlad_free(vlad7);
   //Check all the frees regions...
   //Front region - this shouldnt change at all
   free_header_t *vlad7_a_free = (free_header_t *)&memory[0];
   assert(vlad7_a_free->magic == MAGIC_FREE);
   assert(vlad7_a_free->size == 32);
   assert(vlad7_a_free->next == 64);
   assert(vlad7_a_free->prev == 1728);
   //Middle 1
   free_header_t *vlad7_a_free1 = (free_header_t *)&memory[64];
   assert(vlad7_a_free1->magic == MAGIC_FREE);
   assert(vlad7_a_free1->size == 40);
   assert(vlad7_a_free1->next == 212);
   assert(vlad7_a_free1->prev == 0);
   //Middle 2
   free_header_t *vlad7_a_free2 = (free_header_t *)&memory[212];
   assert(vlad7_a_free2->magic == MAGIC_FREE);
   assert(vlad7_a_free2->size == 208);
   assert(vlad7_a_free2->next == 1728);
   assert(vlad7_a_free2->prev == 64);
   //Back region
   free_header_t *vlad7_a_free3 = (free_header_t *)&memory[1728];
   assert(vlad7_a_free3->magic == MAGIC_FREE);
   assert(vlad7_a_free3->size == 320);
   assert(vlad7_a_free3->next == 0);
   assert(vlad7_a_free3->prev == 212);
   assert(free_list_ptr == 0);


   //Deallocate the middle allocated to check merge works for multiple
   printf("-- Vlad deallocate block 'c' \n"
          "should be three free regions \n\n");
   vlad_free(vlad6);
   //Check all the frees regions...
   //Front region - again this shouldnt change at all
   free_header_t *vlad6_a_free = (free_header_t *)&memory[0];
   assert(vlad6_a_free->magic == MAGIC_FREE);
   assert(vlad6_a_free->size == 32);
   assert(vlad6_a_free->next == 64);
   assert(vlad6_a_free->prev == 1728);
   //Middle - should be a merge of 3 free regions, yay
   free_header_t *vlad6_a_free1 = (free_header_t *)&memory[64];
   assert(vlad6_a_free1->magic == MAGIC_FREE);
   assert(vlad6_a_free1->size == 356);
   assert(vlad6_a_free1->next == 1728);
   assert(vlad6_a_free1->prev == 0);
   //Back region
   free_header_t *vlad6_a_free3 = (free_header_t *)&memory[1728];
   assert(vlad6_a_free3->magic == MAGIC_FREE);
   assert(vlad6_a_free3->size == 320);
   assert(vlad6_a_free3->next == 0);
   assert(vlad6_a_free3->prev == 64);
   assert(free_list_ptr == 0);


   //Lastly lets deallocate everything and check all has merged back well
   printf("-- Vlad deallocate blocks 'e' & 'b' \n"
          "should be left with what we started with! Almost there :) \n\n");
   vlad_free(vlad10);
   vlad_free(vlad9);
   //The final test
   free_header_t *doYouWin = (free_header_t *)&memory[0];
   assert(doYouWin->magic == MAGIC_FREE);
   assert(doYouWin->size == 2048);
   assert(doYouWin->next == 0);
   assert(doYouWin->prev == 0);
   assert(free_list_ptr == 0);


   printf("-- All vlad alloc, free and merge tests passed!..\n"
          "Remember to test with your own tests! -- \n\n");
}