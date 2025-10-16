#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	const int ROWS = 20000;
	const int COLS = 20000;
	int *m = (int *)malloc(ROWS * COLS * sizeof(int));

	printf("Initializing the matrix...\n");
	for (int i = 0; i < ROWS * COLS; i++) {
		m[i] = 1;
	}

	long long sum;
	clock_t start, end;
	double elapsed_seconds;

	printf("Summing using row-major access...\n");
	sum = 0;
	start = clock();
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			sum += m[i * COLS + j];
		}
	}
	end = clock();
	elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;

	printf("  Sum: %lld\n", sum);
	printf("  Time taken: %f seconds\n", elapsed_seconds);


	printf("Summing using column-major access...\n");
	sum = 0;
	start = clock();
	for (int j = 0; j < COLS; j++) {
		for (int i = 0; i < ROWS; i++) {
			sum += m[i * COLS + j];
		}
	}
	end = clock();
	elapsed_seconds = (double)(end - start) / CLOCKS_PER_SEC;

	printf("  Sum: %lld\n", sum);
	printf("  Time taken: %f seconds\n", elapsed_seconds);


	free(m);

	return 0;
}
