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

// ALLOCATION STRATEGY
#define BEST_FIT       1
#define WORST_FIT      2
#define RANDOM_FIT     3

// BRIAN'S MACROS
#define MIN_SIZE       1024
#define POWER          2      // possibly remove this, as value is intuitive enough???
#define TRUE           0
#define FALSE          1

// DATA TYPES
typedef unsigned char byte;
typedef u_int32_t vsize_t;
typedef u_int32_t vlink_t;
typedef u_int32_t vaddr_t;

// HEADER STRUCT FOR FREE BLOCK
typedef struct free_list_header {
   u_int32_t magic;  // ought to contain MAGIC_FREE
   vsize_t size;     // # bytes in this block (including header)
   vlink_t next;     // memory[] index of next free block
   vlink_t prev;     // memory[] index of previous free block
} free_header_t;

// HEADER STRUCT FOR ALLOCATED BLOCKS
typedef struct alloc_block_header {
   u_int32_t magic;  // ought to contain MAGIC_ALLOC
   vsize_t size;     // # bytes in this block (including header)
} alloc_header_t;

// GLOBAL DATA - TO REFERENCE THE BLOCK OF MEMORY YOU WANT TO USE FOR THE ALLOCATOR
static byte *memory = NULL;   // pointer to start of allocator memory
static vaddr_t free_list_ptr; // index in memory[] of first block in free list
static vsize_t memory_size;   // number of bytes malloc'd in memory[]
static u_int32_t strategy;    // allocation strategy (by default BEST_FIT)

// Private functions

// Determine if size is power 2
static int is_power_2(u_int32_t size) {
   assert(size >= MIN_SIZE);                    // size should at least be = 1024
   int val = size; 
   while ((val != POWER) && (val%POWER == 0)) { // divide until power or mod is reached
      val = (val/POWER);
   }
   if (val == POWER) {
      return TRUE;
   } else {
      return FALSE;
   }
}

// Convert to next smallest power of 2
static u_int32_t conv_power(u_int32_t size) {
   assert(size >= MIN_SIZE);               // size should at least be = 1024
   //printf("Before conversion, size is: %d bytes\n", size);
   while (is_power_2(size) == FALSE) {     // keep adding until power 2 is reached
      size = size + POWER;
   }
   //printf("After conversion, size is: %d bytes\n", size);
   return size;
}

static void vlad_merge();


// Input: size - number of bytes to make available to the allocator
// Output: none              
// Precondition: Size >= 1024
// Postcondition: `size` bytes are now available to the allocator
// 
// (If the allocator is already initialised, this function does nothing,
//  even if it was initialised with different size)

   // 1. Allocate overall memory block using malloc()
   // 2. Set up initial region header

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
   free_list_ptr = (vaddr_t) 0;
   free_header_t *init_header = (free_header_t *) memory;
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
   return NULL; // temporarily
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

