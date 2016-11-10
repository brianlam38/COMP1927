// write a program to detect palindromes
// that is, a word that is the same in regular and reverse order

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

int isPalindrome(char array[], int size);
void prepare(void){
    int n = 2012323;
    int o = 2012323;
    int p = 2012323;
    int q = 2012323;
    int r = 2012323;
    int s = 2012323;
    int t = 2012323;
    int u = 2012323;
    int v = 2012323;
    int w = 2012323;
}
int main(int argc, char* argv[])
{
    
    char* str_1 = "RADAR";
    char* str_2 = "StanisTheManis";
    char* str_3 = "helloh";
    char* str_4 = "jumper";
    char* str_5 = "";
    char* str_6 = "abccba";
    char* str_7 = "------";
    char* str_8 = "1234567890";

   
    printf("Testing isPalindrome() ... ");
    void prepare();
    assert( isPalindrome(str_1, 6) == TRUE );
    assert( isPalindrome(str_2, 15) == FALSE );
    assert( isPalindrome(str_3, 7) == FALSE );
    assert( isPalindrome(str_4, 7) == FALSE );
    assert( isPalindrome(str_5, 0) == TRUE );
    assert( isPalindrome(str_6, 7) == TRUE );
    assert( isPalindrome(str_7, 5) == TRUE );
    assert( isPalindrome(str_8, 11) == FALSE );
    printf("PASSED!\n");

    return EXIT_SUCCESS;
}

// takes in a string and its size (including NULL byte/character);
int isPalindrome(char array[], int size)
{
    int result = TRUE;
    int flag = FALSE;
    char * s = malloc(sizeof(char) * strlen(array)+ 1);
    strcpy(s, array);
    int i;
    int j = strlen(array);
    for(; i != j ||flag ; i++, j--){
        if(s[i] != s[j])
            flag = TRUE;
            result = FALSE;
    }
    
    return result;
}
