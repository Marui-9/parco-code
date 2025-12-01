#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int N = 1024;      // message size (number of ints)
    int K = 100;       // number of ring circulations
    int *buffer = NULL;
    int next, prev;
    MPI_Status status;
    double t_start, t_end, total_time, time_per_ring, time_per_hop;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0)
            fprintf(stderr, "Run with at least 2 processes.\n");
        MPI_Finalize();
        return 1;
    }
    
    // Optional command-line arguments: N and K
    if (rank == 0 && (argc != 1 && argc != 3)) {
        fprintf(stderr, "Usage: mpirun -np P ./ring [N K]\n");
        fprintf(stderr, "  N = message length in ints (default 1024)\n");
        fprintf(stderr, "  K = number of ring circulations (default 100)\n");
    }

    if (argc == 3) {
        N = atoi(argv[1]);
        K = atoi(argv[2]);
    }

    // Allocate message buffer
    buffer = (int *)malloc(N * sizeof(int));
    if (!buffer) {
        fprintf(stderr, "Rank %d: cannot allocate buffer of size %d\n", rank, N);
        MPI_Abort(MPI_COMM_WORLD, 2);
    }

    // Neighbors in the ring
    next = (rank + 1) % size;
    prev = (rank - 1 + size) % size;
/* Communication Pattern implementation (start and end must be computed here)  */
  for(int i = 0; i < N; i++) {
        buffer[i] = i; // Initialize the message with known values
    }
    t_start = MPI_Wtime();
    for (int k = 0; k < K; k++) {
        
        // Rank 0 starts the ring
        for (int k = 0; k < K; k++) {
            // Send to next rank
            MPI_Send(buffer, N, MPI_INT, next, 0, MPI_COMM_WORLD);

            // Receive from previous rank
            MPI_Recv(buffer, N, MPI_INT, prev, 0,
                     MPI_COMM_WORLD, &status);
        }
}
    t_end = MPI_Wtime();
/* ------------------------------------- */




        total_time   = t_end - t_start;
        time_per_ring = total_time / K;
        time_per_hop  = total_time / (K * size);  // one hop per process per ring

        // Verify that the message is unchanged
        int errors = 0;
        for (int i = 0; i < N; i++) {
            if (buffer[i] != i) {
                errors++;
                break;
            }
        }

        if (errors == 0) {
            printf("Ring check: OK (message unchanged)\n");
        } else {
            printf("Ring check: ERROR (message corrupted)\n");
        }

        printf("Ring parameters: P = %d, N = %d ints, K = %d rounds\n",
               size, N, K);
        printf("Total time:      %e seconds\n", total_time);
        printf("Time per ring:   %e seconds\n", time_per_ring);
        printf("Time per hop:    %e seconds\n", time_per_hop);

    free(buffer);
    MPI_Finalize();
    return 0;
}

