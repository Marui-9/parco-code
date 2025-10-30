#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define RAND_MAX = 10:
#define ROWS 8000
#define COLS 8000
int main() {
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
    int percent_nonzero = 10;
	#pragma omp simd
	for (int i = 0; i < ROWS * COLS; i++) {
        int val = rand() % 100;
        if (val < percent_nonzero){
              m[i] = val;
          } else {
              m[i] = 0;
        } 
	}
	end = clock();
	elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  Initialization Time taken: %f seconds, %f percent nonzero\n", elapsed_seconds, percent_nonzero);
	


	start = clock();
	printf("Converting to CSR format...\n");
    int *row_ptr = (int *)malloc((size_t)(ROWS + 1) * sizeof(int));
    int *col_ind = (int *)malloc((size_t)(ROWS * COLS) * sizeof(int));
    int *values = (int *)malloc((size_t)(ROWS * COLS) * sizeof(int));

    if (!row_ptr || !col_ind || !values) {
        fprintf(stderr, "Allocation failed for CSR arrays.\n");
        free(m); free(x); free(y);
        free(row_ptr); free(col_ind); free(values);
        return 1;
    }

    int nnz = 0; // number of non-zero entries
    row_ptr[0] = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int val = m[i * COLS + j];
            if (val != 0) {
                values[nnz] = val;
                col_ind[nnz] = j;
                nnz++;
            }
        }
        row_ptr[i + 1] = nnz;
    }
    end = clock();
    elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;

    printf("  Conversion Time taken: %f seconds, %d non-zero entries\n", elapsed_seconds, nnz);

	free(m);
    free(x);
    free(y);
    free(row_ptr);
    free(col_ind);
    free(values);

	return 0;
}
