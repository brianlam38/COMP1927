/*
 * Sample Prac Question
 * q2.c
 * Read the question sheet first
 *
 * Any questions email riyasat.saber@student.unsw.edu.au
 * but please ask your tutor first
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void diamond (long in){
    printf("input: %lu\n", in);

    // write your function here
}

int main (int argc, char * argv[]){
    if( argc != 2 ){
        printf("Incorrect number of arguments. ex: \n%s n (where n >=3 and n <= 25)\n",argv[0]);
        abort();
    } 
    long input = strtol(argv[1], NULL, 10);
    if( input % 2 != 1 || input < 3 || input > 25 ){
        printf("Please enter an odd, positive value greater or equal to 3, less than or equal to 25 for the argument. ex:\n%s n (where n >=3 and n <= 25)\n",argv[0]);
        abort();
    }

    diamond(input);

    return EXIT_SUCCESS;
}