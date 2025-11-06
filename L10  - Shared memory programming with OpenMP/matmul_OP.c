#include <stdio.h>
#include <omp.h>

#define SIZE 1000

int main() {
    int A[SIZE][SIZE];
    int B[SIZE][SIZE];
    int C[SIZE][SIZE] = {0};  // Initialize result matrix to zero

    // Initialize matrix A with values A[i][j] = i + j
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            A[i][j] = i + j;
        }
    }

    // Initialize matrix B with values B[i][j] = i - j
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            B[i][j] = i - j;
        }
    }

    // Matrix multiplication: C[i][j] = sum(A[i][k] * B[k][j]) for k = 0 to SIZE-1
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int sum = 0; // private for each thread
            for (int k = 0; k < SIZE; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    // Print a few sample values from matrix C to verify the result
    printf("Sample values from matrix C:\n");
    printf("C[0][0] = %d\n", C[0][0]);
    printf("C[500][500] = %d\n", C[500][500]);
    printf("C[999][999] = %d\n", C[999][999]);

    return 0;
}