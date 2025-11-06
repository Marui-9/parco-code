#include <stdio.h>
#include <stdlib.h>

#define N 1000 // Number of rows
#define M 1000 // Number of columns

int main() {
    double A[N][M];
    double x[M];
    double y[N];

    // Initialize the matrix A with values i + j and vector x with values j
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = i + j;
        }
    }
    for (int j = 0; j < M; j++) {
        x[j] = j;
    }

    // Compute the matrix-vector multiplication y = A * x
    for (int i = 0; i < N; i++) {
        y[i] = 0.0;
        for (int j = 0; j < M; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    // Print a portion of the result vector y
    printf("Sample result y[0] = %f\n", y[0]);
    printf("Sample result y[N-1] = %f\n", y[N-1]);

    return 0;
}


