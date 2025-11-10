/* File:  
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP
#include <omp.h>
#else
static inline void omp_set_num_threads(int n) { (void)n; }
static inline int omp_get_max_threads(void) { return 1; }
static inline double omp_get_wtime(void) { return (double)clock() / CLOCKS_PER_SEC; }
#endif

/* Problem parameters */
int* a = NULL;
int length = 900000;
int * merge_sort_serial(int *array, int length);
int * merge_sort_parallel(int *array, int length);

int * generate_array(int length);
//void print_array(int *array, int length);
double f(double x);
/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
    /* Usage: merge_sort [num_threads] [length]
       num_threads is optional (default 4). length is optional (default from file).
    */
    int num_threads = 4; // default
    if (argc >= 2) {
        int v = (int)strtol(argv[1], NULL, 10);
        if (v > 0) num_threads = v;
    }
    if (argc >= 3) {
        int v = (int)strtol(argv[2], NULL, 10);
        if (v > 0) length = v;
    }

    omp_set_num_threads(num_threads);
    printf("OpenMP max threads (for next parallel region): %d\n", omp_get_max_threads());

    srand((unsigned)time(NULL));
    a = generate_array(length);
    printf("Unsorted array:\n");
   // print_array(a, length);
   
   double start_time = omp_get_wtime();
   int * sorted = merge_sort_serial(a, length);
   double end_time = omp_get_wtime();
   double serial_time = end_time - start_time;
   printf("Elapsed time serial = %.6f seconds\n", serial_time);
   free(sorted);

   /* Parallel run: create a single parallel region and spawn tasks inside it. */
   start_time = omp_get_wtime();
   int * sorted_parallel = NULL;
   #pragma omp parallel
   {
       #pragma omp single
       {
           sorted_parallel = merge_sort_parallel(a, length);
       }
   }
   end_time = omp_get_wtime();
   double parallel_time = end_time - start_time;

   printf("Elapsed time parallel = %.6f seconds\n", parallel_time);
   printf("Speedup = %.2fx\n", parallel_time > 0 ? (serial_time / parallel_time) : 0.0);
   free(sorted_parallel);
   return 0;
}
int * generate_array(int length) {
    int *array = (int *)malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) {
        array[i] = rand() % 100; // Fill with random integers
    }
    return array;
}
void print_array(int *array, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
/*-------------------------------------------------------------------*/
int * merge_sort_serial(int *array, int len) {
    if (len <= 0) return NULL;
    if (len == 1) {
        int *single = malloc(sizeof(int));
        if (!single) { fprintf(stderr, "malloc failed\n"); exit(1); }
        single[0] = array[0];
        return single;
    }

    int mid = len / 2;
    /* Recursively get sorted halves (each call returns a newly allocated array) */
    int *left_sorted = merge_sort_serial(array, mid);
    int *right_sorted = merge_sort_serial(array + mid, len - mid);

    /* Merge the two sorted halves */
    int *sorted_parallel = malloc(len * sizeof(int));
    if (!sorted_parallel) { fprintf(stderr, "malloc failed\n"); exit(1); }
    int i = 0, j = 0, k = 0;
    while (i < mid && j < (len - mid)) {
        if (left_sorted[i] <= right_sorted[j]) sorted_parallel[k++] = left_sorted[i++];
        else sorted_parallel[k++] = right_sorted[j++];
    }
    while (i < mid) sorted_parallel[k++] = left_sorted[i++];
    while (j < (len - mid)) sorted_parallel[k++] = right_sorted[j++];

    free(left_sorted);
    free(right_sorted);
    return sorted_parallel;
}
int * merge_sort_parallel(int *array, int len){
    if (len <= 0) return NULL;
    if (len == 1) {
        int *single = malloc(sizeof(int));
        if (!single) { fprintf(stderr, "malloc failed\n"); exit(1); }
        single[0] = array[0];
        return single;
    }

    /* Use a threshold to avoid creating too many small tasks */
    if (len <= length / (omp_get_max_threads())) {
        return merge_sort_serial(array, len);
    }

    int mid = len / 2;
    int *left_sorted = NULL;
    int *right_sorted = NULL;

    /* Spawn tasks for left and right halves. These tasks will execute on the
       threads from the single outer parallel region created in main(). */
    #pragma omp task shared(left_sorted)
    left_sorted = merge_sort_parallel(array, mid);

    #pragma omp task shared(right_sorted)
    right_sorted = merge_sort_parallel(array + mid, len - mid);

    #pragma omp taskwait

    /* Merge the two sorted halves */
    int *sorted = malloc(len * sizeof(int));
    if (!sorted) { fprintf(stderr, "malloc failed\n"); exit(1); }
    int i = 0, j = 0, k = 0;
    while (i < mid && j < (len - mid)) {
        if (left_sorted[i] <= right_sorted[j]) sorted[k++] = left_sorted[i++];
        else sorted[k++] = right_sorted[j++];
    }
    while (i < mid) sorted[k++] = left_sorted[i++];
    while (j < (len - mid)) sorted[k++] = right_sorted[j++];

    free(left_sorted);
    free(right_sorted);
    return sorted;
}

