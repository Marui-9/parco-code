#include <stdio.h>
#include <omp.h>

#define SIZE 1000

int main() {
    int array[SIZE][SIZE];
    long long sum = 0; // Use long long to store large sums

    // Initialize the array with values array[i][j] = i * j
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            array[i][j] = i * j;
        }
    }

    // Calculate the sum of all elements in the array
    #pragma omp parallel for collapse(2) reduction(+:sum)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            sum += array[i][j];
        }
    }

    // Print the result
    printf("Sum of all elements in the array: %lld\n", sum);

    return 0;
}
