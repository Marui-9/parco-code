#include <stdio.h>
#include <omp.h>

#define ARRAY_SIZE 1000000

int main() {
    int arr[ARRAY_SIZE];
    long long sum = 0;

    // Initialize the array with some values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = i; 
    }

    // Compute the sum in parallel
    #pragma omp parallel for 
    for (int i = 0; i < ARRAY_SIZE; i++) {
		#pragma omp critical
        sum += arr[i];
    }

    printf("Sum of array elements (parallel) = %lld\n", sum);
	return 0;
}
