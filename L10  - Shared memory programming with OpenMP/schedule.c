#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define ARRAY_SIZE 1000000

int main() {
    double array[ARRAY_SIZE];
    double sum;
    int i;

    // Initialize array with random values
    for (i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() / (double)RAND_MAX;
    }

    // Test different scheduling strategies and chunk sizes
    int chunk_sizes[] = {10, 100, 1000};
    char* schedule_types[] = {"static", "dynamic", "guided"};
    
    for (int s = 0; s < 3; s++) {
        for (int c = 0; c < 3; c++) {
            sum = 0.0;
            double start_time = omp_get_wtime();

            #pragma omp parallel for reduction(+:sum) schedule(static, chunk_sizes[c])
            for (i = 0; i < ARRAY_SIZE; i++) {
                sum += array[i];  // Or perform a heavier computation if needed
            }

            double end_time = omp_get_wtime();
            printf("Schedule: %s, Chunk Size: %d, Time: %f seconds\n", schedule_types[s], chunk_sizes[c], end_time - start_time);
        }
    }

    return 0;
}
