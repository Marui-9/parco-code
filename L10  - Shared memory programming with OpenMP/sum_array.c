#include <stdio.h>

#define ARRAY_SIZE 1000000

int main() {
    int arr[ARRAY_SIZE];
    long long sum = 0;

    // Initialize the array with some values
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = i; 
    }

    // Compute the sum
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += arr[i];
    }

    printf("Sum of array elements (serial) = %lld\n", sum);
    return 0;
}
