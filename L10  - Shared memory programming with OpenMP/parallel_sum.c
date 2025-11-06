#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 100000000  // Large array size for performance testing
#define NUM_RUNS 10      // Number of runs per thread count

int main() {
    int *array = (int*)malloc(SIZE * sizeof(int));
    for (int i = 0; i < SIZE; i++) array[i] = 1;  // Initialize all elements to 1 for simplicity

    // Serial execution for baseline
    double start = omp_get_wtime();
    long long sum_serial = 0;
    for (int i = 0; i < SIZE; i++) {
        sum_serial += array[i];
    }
    double end = omp_get_wtime();
    double time_serial = end - start;

    printf("Serial Sum = %lld, Serial Time = %f seconds\n\n", sum_serial, time_serial);

    // Open CSV file for writing results
    FILE *file = fopen("scaling_results.csv", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    // Write header for CSV
    fprintf(file, "Num_Threads,Parallel_Sum,Avg_Parallel_Time,Avg_Speedup,Avg_Efficiency\n");

    // Test performance with thread counts of 1, 2, 4, 8, 16, 32, and 64
    for (int num_threads = 1; num_threads <= 64; num_threads *= 2) {
        omp_set_num_threads(num_threads);

        double total_time_parallel = 0.0;
        long long sum_parallel = 0;

        for (int run = 0; run < NUM_RUNS; run++) {
            sum_parallel = 0;
            start = omp_get_wtime();

            #pragma omp parallel for reduction(+:sum_parallel)
            for (int i = 0; i < SIZE; i++) {
                sum_parallel += array[i];
            }

            end = omp_get_wtime();
            double time_parallel = end - start;
            total_time_parallel += time_parallel;
        }

        // Compute average parallel time, speedup, and efficiency
        double avg_time_parallel = total_time_parallel / NUM_RUNS;
        double avg_speedup = time_serial / avg_time_parallel;
        double avg_efficiency = avg_speedup / num_threads;

        // Print results for each thread count
        printf("%11d | %12lld | %17f | %7.2f | %9.2f%%\n", 
               num_threads, sum_parallel, avg_time_parallel, avg_speedup, avg_efficiency * 100);

        // Write results to CSV
        fprintf(file, "%d,%lld,%f,%f,%f\n", num_threads, sum_parallel, avg_time_parallel, avg_speedup, avg_efficiency * 100);
    }

    // Close CSV file and free allocated memory
    fclose(file);
    free(array);

    return 0;
}

