#define TRUE 1
#define FALSE 0

/*
 Consider the following sorting algorithm running on a machine that can execute 1 million steps per millisecond.
 Assume comparing items (line 14) and swapping items (line 15) are the only two operations and take equal time to execute.
*/
void eesort(int items[], int n) {
   int i, j;
   int done = FALSE;
   for (i = n - 1; i > 0 && !done; i--){
      done = TRUE; // Assume sorted
      for (j = 1; j <= i; j++) {
         if (items[j - 1] > items[j]){
            swap(j, j - 1,items);
            done = FALSE;
         }
      }
   }
}

/* QUESTIONS
For an input size of n = 100 000:
a) Make comments about the complexity of lines 11-16 in Big-O Notation.
b) What kind of input would result in the best case scenario/shortest running time? How long would this take?
c) What kind of input would result in the worst case scenario/longest running time? How long would this take?
d) Remove the done flag and therefore the early exit feature. Does this affect your answer to part b) and c)?
*/