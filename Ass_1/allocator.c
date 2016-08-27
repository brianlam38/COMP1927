//
//  COMP1927 Assignment 1 - Vlad: the memory allocator
//  allocator.c ... implementation
//
//  Created by Liam O'Connor on 18/07/12.
//  Modified by John Shepherd in August 2014, August 2015
//  Copyright (c) 2012-2015 UNSW. All rights reserved.
//

#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FREE_HEADER_SIZE  sizeof(struct free_list_header)  
#define ALLOC_HEADER_SIZE sizeof(struct alloc_block_header)  
#define MAGIC_FREE     0xDEADBEEF
#define MAGIC_ALLOC    0xBEEFDEAD

// Allocation Strategy
#define BEST_FIT       1
#define WORST_FIT      2
#define RANDOM_FIT     3

// Extra Macros
#define MIN_SIZE       1024   // Minimum malloc size
#define MIN_ALLOC      8      // Minimum allocation size
#define TRUE           0
#define FALSE          1
#define POWER          2      // Malloc size must be power of 2
#define MULTIPLE       4      // Alloc size must be multiple of 4


// #################
// Data Types
// #################

typedef unsigned char byte;
typedef u_int32_t vsize_t;
typedef u_int32_t vlink_t;
typedef u_int32_t vaddr_t;

// Header struct: Free blocks
typedef struct free_list_header {
   u_int32_t magic;  // ought to contain MAGIC_FREE
   vsize_t size;     // # bytes in this block (including header)
   vlink_t next;     // memory[] index of next free block
   vlink_t prev;     // memory[] index of previous free block
} free_header_t;

// Header struct: Allocated blocks
typedef struct alloc_block_header {
   u_int32_t magic;  // ought to contain MAGIC_ALLOC
   vsize_t size;     // # bytes in this block (including header)
} alloc_header_t;

// Global Data: To reference block of memory you want to use for the allocator
static byte *memory = NULL;   // pointer to start of allocator memory
static vaddr_t free_list_ptr; // index in memory[] of first block in free list
static vsize_t memory_size;   // number of bytes malloc'd in memory[]
static u_int32_t strategy;    // allocation strategy (by default BEST_FIT)

// #################
// Private Functions
// #################

// Determine if size is power 2
static int is_power_2(u_int32_t size) {
   assert(size >= MIN_SIZE);                
   int val = size; 
   while ((val != POWER) && (val%POWER == 0)) {
      val = (val/POWER);
   }
   if (val == POWER) {
      return TRUE;
   } else {
      return FALSE;
   }
}

// Convert size to next smallest power of 2
static u_int32_t conv_power(u_int32_t size) {
   assert(size >= MIN_SIZE);
   //printf("Before conversion, size is: %d bytes\n", size);
   while (is_power_2(size) == FALSE) {
      size = size + POWER;
   }
   //printf("After conversion, size is: %d bytes\n", size);
   return size;
}

// Convert allocation bytes to multiple of 4
static u_int32_t conv_mult_4(u_int32_t n) {
   while (n%MULTIPLE != 0) {
      n++;
   }
   printf("n bytes is now size: %d\n", n);
   assert(n%MULTIPLE == 0);
   return n;
}

static void vlad_merge();

// ###################
// Interface Functions
// ###################

// Input: size - number of bytes to make available to the allocator
// Output: none              
// Precondition: Size >= 1024
// Postcondition: `size` bytes are now available to the allocator
// 
// (If the allocator is already initialised, this function does nothing,
//  even if it was initialised with different size)

void vlad_init(u_int32_t size)
{
   //printf("Begin mem alloc\n");
   if (memory == NULL) {            // test for < MIN and if power of 2
      if (size < MIN_SIZE) {     
         //printf("Conv to MIN\n");         
         size = MIN_SIZE;           // convert size to MIN
         memory = malloc(size);
      } else {
         //printf("Conv to POW\n");
         size = conv_power(size);   // convert size to next power of 2
         assert(is_power_2(size) == TRUE);
         memory = malloc(size);
      }
   }
   //printf("Fin mem alloc\n");
   if (memory == NULL){             // if malloc fails
      fprintf(stderr, "vlad_init: insufficient memory\n");
      exit(EXIT_FAILURE);
   }
   memory_size = size;
   strategy = BEST_FIT;
   free_list_ptr = (vaddr_t) 0;                           // typecasting memory add
   free_header_t *init_header = (free_header_t *) memory; // typecasting ptr --> memory
   init_header->magic = MAGIC_FREE;
   init_header->size = size;
   init_header->next = free_list_ptr;
   init_header->prev = free_list_ptr;
   //printf("Initialisation successful\n");
}


// Input: n - number of bytes requested
// Output: p - a pointer, or NULL
// Precondition: n < size of largest available free block
// Postcondition: If a region of size n or greater cannot be found, p = NULL 
//                Else, p points to a location immediately after a header block
//                      for a newly-allocated region of some size >= 
//                      n + header size.

void *vlad_malloc(u_int32_t n)
{
   // TODO for Milestone 2

   // 1. Takes in request to allocate n bytes
   // 2. If n != multiple of 4, round up to next smallest multiple of 4 --> Insert conversion function
   printf("Size of n bytes before all else is: %d", n);
   if (n < MIN_ALLOC) {
      n = MIN_ALLOC;
      printf("After conversion, size of n bytes is: %d", n);
   }
   if (n%MULTIPLE != 0) {
      conv_mult_4(n);
      printf("After conversion, size of n bytes is: %d", n);
   }
   // 3. If n == multiple of 4, then it can be used for allocation

   // 4. Start Traverse, from the region pointed to by FREE_LIST_PTR (the first free region available in the free list)
   // 5. Continue Traversal, until a region is found that is > required n allocation
   // 6. If region >= THRESHOLD, then split into two regions;
         // (1) Allocated Region (multiple of 4)
         // (2) Free Region (at least 2 * FREE_LIST_HEADER size), placed in the correct pos in free list
   // 7. If region < THRESHOLD, then allocate the entire region
   return NULL;
}


// Input: object, a pointer.
// Output: none
// Precondition: object points to a location immediately after a header block
//               within the allocator's memory.
// Postcondition: The region pointed to by object has been placed in the free
//                list, and merged with any adjacent free blocks; the memory
//                space can be re-allocated by vlad_malloc

void vlad_free(void *object)
{
   // TODO for Milestone 3
   vlad_merge();
}

// Input: current state of the memory[]
// Output: new state, where any adjacent blocks in the free list
//            have been combined into a single larger block; after this,
//            there should be no region in the free list whose next
//            reference is to a location just past the end of the region

static void vlad_merge()
{
	// TODO for Milestone 4
}

// Stop the allocator, so that it can be init'ed again:
// Precondition: allocator memory was once allocated by vlad_init()
// Postcondition: allocator is unusable until vlad_int() executed again

void vlad_end(void)
{
   // TODO for Milestone 1
   free(memory);
   assert(memory == NULL);
   printf("Vlad the impaler is dead!\n");
}


// Precondition: allocator has been vlad_init()'d
// Postcondition: allocator stats displayed on stdout

void vlad_stats(void)
{
   // TODO
   // put whatever code you think will help you
   // understand Vlad's current state in this function
   // REMOVE all of these statements when your vlad_malloc() is done
   printf("vlad_stats() won't work until vlad_malloc() works\n");
   return;
}

