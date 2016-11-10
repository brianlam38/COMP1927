// diamond.c
// practice question for mini prac
// peter kydd, pkydd@cse.unsw.edu.au
// program takes in an odd positive argument

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void diamond(long in);

int main (int argc, char * argv[])
{
    if( argc != 2 ){
        fputs("Incorrect number of arguments. ex: \n./diamond n (where n >=3 and n <= 25)\n", stderr );
        abort();
    }
    long input = strtol(argv[1], NULL, 10);
    if( input % 2 != 1 || input < 3 || input > 25 ){
        fputs("Please enter an odd, positive value greater or equal to 3, less than or equal to 25 for the argument. ex:\n./diamond n (where n >= 3 and n <= 25)\n", stderr);
        abort();
    }

    diamond(input);

    return EXIT_SUCCESS;
}
/* Given a positive integer between */
void diamond (long in)
{
    printf("input: %lu\n", in);

    // your code here


    // peter version - to remove
    int i, j;
    int mid = ((in/2));

    // first half
    for(i = 0; i < mid; ++i){
        for(j = 0; j< in; ++j){

            if(j == mid-i ){
                for(; j < mid+i; ++j){
                    printf(" ");
                }
                printf(" ");
            }else{
                printf("%c",j+'A');
            }
        }
        printf("\n");
    }


    //second half
    for(; i < in; ++i){
        for(j = 0; j< in; ++j){

            if(j == mid-((in-1)-i)){
                for(; j < mid+((in-1)-i); ++j){
                    printf(" ");
                }
                printf(" ");
            }else{
                printf("%c",j+'A');
            }
        }
        printf("\n");
    }



}
