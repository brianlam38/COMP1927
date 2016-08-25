//
// COMP1927 Assignment 1 - Memory allocator test driver
// vlad.c ... test the memory allocator
//
// Written by John Shepherd, August 2014
// Modified by John Shepherd, August 2015

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#include "allocator.h"

#define MEMORY_SIZE 4096

typedef unsigned char Byte;

// Main program: reads commands from stdin until EOF
// Assumes we have 26 pointer variables called a..z
// Allows us to perform operations on those variables
// Possible commands:
//    + X N  ... allocate N bytes and assign to X
//    - X    ... free memory associated with X
//    * X N  ... store N in memory referenced by X
//    !      ... show vlad statistics
//    ?      ... show this help message
//    q      ... quit this program (^D also works)
// where X is a single letter in a..z
//       N is an integer value

int main(int argc, char *argv[])
{
   char line[BUFSIZ]; // input line
   char var;          // which "variable"
   int  val;          // value of N
   void *ptr[26];     // array of pointer "variable"s
   int  quiet = 0;    // flag to reduce output "noise"

   setbuf(stdout, NULL); // don't buffer stdout

   // sort out quiet-ness
   if (argc > 1 && argv[1][0] == 'q') quiet = 1;

   // initialise pointer variables
   int i;
   for (i = 'a'; i <= 'z'; i++) {
      ptr[i-'a'] = NULL;
   }

   // start the allocator
   vlad_init(MEMORY_SIZE);

   // main loop ... read command and carry it out
   if (isatty(0) && !quiet) printf("> ");
   while (fgets(line, BUFSIZ, stdin) != NULL) {
      // if reading from a file, echo the command
      if (!isatty(0)) printf("%s\n",line);
      // do some cheap-and-nasty parsing using sscanf
      if (sscanf(line, "+ %[a-z] %d", &var, &val) == 2) {
         // set a pointer variable using vlad_malloc()
         if (ptr[var-'a'] != NULL)
            fprintf(stderr, "Attempt to alloc over already allocated pointer\n");
         else {
            Byte *b = vlad_malloc(val);
            if (b == NULL)
               fprintf(stderr, "Failed to allocate %d bytes for ptr[%c]\n", val, var);
            else {
               ptr[var-'a'] = b;
               if (!quiet) printf("ptr[%c] allocated %p\n", var, b);
            }
         }
      }
      else if (sscanf(line, "- %[a-z]", &var) == 1) {
         // free memory allocated by Vlad
         if (ptr[var-'a'] == NULL)
            fprintf(stderr, "Attempt to free null pointer\n");
         else {
            Byte *b = ptr[var-'a'];
            if (!quiet) printf("Deallocating memory at %p\n", b);
            vlad_free(b);
            ptr[var-'a'] = NULL;
         }
      }
      else if (sscanf(line, "* %[a-z] %d", &var, &val) == 2) {
         // write something into an allocated piece of memory
         if (ptr[var-'a'] == NULL)
            fprintf(stderr, "Attempt to write via unallocated pointer\n");
         else {
            Byte *x = ptr[var-'a'];
            *x = (Byte)val;
            if (!quiet) printf("Memory at %p assigned %d\n", x, *x);
         }
      }
      else if (line[0] == '?') {
         printf("Possible commands:\n");
         printf("+ X N  ... allocate N bytes and assign to X\n");
         printf("- X    ... free memory associated with X\n");
         printf("* X N  ... store N in memory referenced by X\n");
         printf("!      ... show Vlad statistics\n");
         printf("?      ... show this help message\n");
         printf("q      ... quit this program (^D also works)\n");
         printf("       where X is a single letter in a..z\n");
         printf("       and   N is an integer value\n");
      }
      else if (line[0] == '!') {
         vlad_stats();
      }
      else if (line[0] == 'q') {
         break;
      }
      else {
         printf("Invalid command\n");
      }
      if (isatty(0) && !quiet) printf("> ");
   }
   return EXIT_SUCCESS;
}

