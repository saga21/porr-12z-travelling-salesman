/* randompermute.c - A program will generate "random permutations of n elements"
   if at all points the n! possible permutations have all the same probability 
   of being generated.
*/

#include <stdio.h>
#include <stdlib.h>

// It returns a random permutation of 0..n-1
int* rpermute(int n) {
	
    int k, j, temp;

    int *result = (int*)malloc(n*sizeof(int));

    for (k = 0; k < n; k++)
		result[k] = k;
    
	for (k = n-1; k > 0; k--) {
		j = rand() % (k+1);
		temp = result[j];
		result[j] = result[k];
		result[k] = temp;
    }
    
	return result;
}

// Print a 8 elements per line
void printarray(int n, int* a) {

	int k;

    for (k = 0; k < n; k++)
		printf("%2d ", a[k]);

}

int main(void) {
    int limit = 22;
    int *a;
    int k;
    // Print 7 permutations
    for (k = 0; k < 7; k++) {
		a = rpermute(limit);
		printarray(limit, a);
		printf("\n\n");
    }
    getchar();
    return 0;
}
