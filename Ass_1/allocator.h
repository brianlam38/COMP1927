//
//  COMP1927 Assignment 1 - Vlad: The memory allocator
//  allocator.h ... interface
//
//  Created by Liam O'Connor on 18/07/12.
//  Modified by John Shepherd in 2014/2015/2016
//  Copyright (c) 2012-2016 UNSW. All rights reserved.
//

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>

// Allocate "size" bytes to be used by the sub-allocator
void vlad_init(u_int32_t size);

// Allocate a chunk of memory with size >= n, if one is available
void *vlad_malloc(u_int32_t n);

// Release chunk of allocated memory and return to free list for re-ue
void vlad_free(void *object);

// Stop the allocator, so that it can be init'ed again:
void vlad_end(void);

// Function to display details of memory layout (for debugging)
void vlad_stats(void);

#endif
