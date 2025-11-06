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

    // Parallel section with a local variable for each thread's partial sum
    #pragma omp parallel
    {
        long long local_sum = 0; // Local variable for each thread
        #pragma omp for
        for (int i = 0; i < ARRAY_SIZE; i++) {
            local_sum += arr[i];
        }
        // Update the global sum with each thread's local sum
        #pragma omp critical
        sum += local_sum;
    }

    printf("Sum of array elements (parallel) = %lld\n", sum);
	return 0;
}
