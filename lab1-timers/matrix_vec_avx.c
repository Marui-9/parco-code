#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define RAND_MAX = 10:

int main() {
	const int ROWS = 8000;
	const int COLS = 8000;
	//int *m = (int *)malloc(ROWS * COLS * sizeof(int));
    	long long sum;
	clock_t start, end;
	double elapsed_seconds;


    int *m = (int *)malloc((size_t)ROWS * (size_t)COLS * sizeof(int));
    int *x = (int *)malloc((size_t)COLS * sizeof(int));
    long long *y = (long long *)malloc((size_t)ROWS * sizeof(long long));

    if (!m || !x || !y) {
        fprintf(stderr, "Allocation failed. Needed ~%zu MB for matrix alone.\n",
                ((size_t)ROWS * (size_t)COLS * sizeof(int)) / (1024 * 1024));
        free(m); free(x); free(y);
        return 1;
    }

    srand(time(NULL));
    start = clock();
    printf("Initializing the matrix...\n");
	#pragma omp simd
	for (int i = 0; i < ROWS * COLS; i++) {
		m[i] = rand() % 10;
	}
	end = clock();
	elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  Initialization Time taken: %f seconds\n", elapsed_seconds);
	


	//printf("Summing using row-major access...\n");
	start = clock();
	printf("Computing y = M * x...\n");
    for (size_t i = 0; i < (size_t)ROWS; i++) {
        // long long sum = 0;
        size_t base = i * (size_t)COLS;
		#pragma omp simd
        for (size_t j = 0; j < (size_t)COLS; j++) {
            sum += (long long)m[base + j] * (long long)x[j];
        }
        y[i] = sum;

    }
	end = clock();	
	elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;

    printf("  Sum: %lld\n", sum);
	printf("  Time taken: %f seconds\n", elapsed_seconds);


	// printf("Summing using column-major access...\n");
	// start = clock();
	// end = clock();
	// elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;

	// printf("  Time taken: %f seconds\n", elapsed_seconds);
  

	free(m);
    free(x);
    free(y);

	return 0;
}
