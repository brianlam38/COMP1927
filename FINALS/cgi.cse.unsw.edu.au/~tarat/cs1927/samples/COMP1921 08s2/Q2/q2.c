#include <stdio.h>
#include <string.h>

#define NUMELS 100

int kthsmallest(int A[], int n, int k);

int main(void)
{
	int arr[NUMELS];
	int numels = 0, i, rank;
	char suffix[3];

	// get and store up to NUMELS integers from user

	printf("Enter the number of integers (<%d) to store: ", NUMELS);
	while(scanf("%d", &numels) != 1 || numels < 1 || numels > NUMELS);

	printf("Enter %d integers:\n", numels);

	for (i=0; i<numels; i++)
		scanf("%d", &arr[i]);

	// echo the user's data

	printf("Numbers entered:\n");
	for (i=0; i<numels; i++)
		printf("arr[%d] = %d\n", i, arr[i]);

	// obtain the rank of the element being sought from the user

	printf("Enter the rank in increasing order being sought: ");
	scanf("%d", &rank);

	// ensure the rank is within bounds

	if (rank > numels) 
	    rank = numels;

	if (rank <= 0)
		rank = 1;

	// determine the suffix for printing the rank

	if (rank > 10 && rank < 21)
		strcpy(suffix, "th");
	else if (rank%10 == 1)
		strcpy(suffix,"st");
	else if (rank%10 == 2)
		strcpy(suffix,"nd");
	else if (rank%10 == 3)
		strcpy(suffix,"rd");
	else
		strcpy(suffix,"th");

	// --------------------------------------------------------------
	// your function called here
	// --------------------------------------------------------------

	printf("The %d-%s smallest number is: %d\n", rank, suffix, kthsmallest(arr, numels, rank));

	return 0;
}

/*
 * Return the value of the k-th smallest (1 <= k <= n) element of A
 */
int kthsmallest(int A[], int n, int k)
{
    return A[0]; // implement your solution and modify this line
}
