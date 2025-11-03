#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int import_matrix(const char *filename, 
            float **out_matrix, 
            int *out_rows, 
            int *out_cols);
float * generate_matrix(int rows, int cols, int percent_nonzero);
float * generate_vector(int size);


int import_matrix(const char *filename, 
            float **out_matrix, 
            int *out_rows, 
            int *out_cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }
//if fscanf
        return -1;
    }
float * generate_matrix(int rows, int cols, int percent_nonzero) {
    if (rows <= 0 || cols <= 0 || percent_nonzero < 0 || percent_nonzero > 100)
        return NULL;

    size_t total_size = (size_t)rows * (size_t)cols;
    float *matrix = malloc(total_size * sizeof(float));
    if (!matrix) {
        fprintf(stderr, "Allocation failed. Needed ~%zu MB for matrix alone.\n",
                total_size * sizeof(float) / (1024 * 1024));
        return NULL;
    }

    // Fill matrix
    for (size_t i = 0; i < total_size; ++i) {
        float val = rand() % 100;
        matrix[i] = (val < percent_nonzero) ? (float)(val + 1) : 0.0f;
    }

    return matrix;
}
float * generate_vector(int size) {
    float *vector = (float *)malloc((size_t)size * sizeof(float));
    if (!vector) {
        fprintf(stderr, "Allocation failed. Needed ~%zu MB for vector alone.\n",
                ((size_t)size * sizeof(float)) / (1024 * 1024));
        return NULL;
    }

    srand(time(NULL));
    #pragma omp simd
    for (int i = 0; i < size; i++) {
        vector[i] = (float)(rand() % 100);
    }
    return vector;
}