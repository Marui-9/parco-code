
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
/* Return a small pseudo-random integer (0..99). */
static int get_random(void)
{
        return rand() % 100;
}

int main
(int argc, char** argv) {

    int iterations = argc >= 2 ? strtol(argv[1], NULL, 10) : 1000000;
    if (argc > 2) {
        fprintf(stderr, "usage: %s [num_iterations]\n", argv[0]);
        return 1;
    }
 // Initialize MPI
 MPI_Init(&argc, &argv);
 int my_rank, comm_size;
 MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
 MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
 // require at least two processes for this example
 if (comm_size < 2) {
         if (my_rank == 0) fprintf(stderr, "Error: need at least 2 MPI ranks\n");
         MPI_Finalize();
         return 1;
 }



 double start_time = MPI_Wtime();
 //repeatedly exchange a message n times between process 0 and process 1 in a ping pong fashion
 
 for (int i = 0; i < iterations; i++) { 
        int ch = 'a';
// Send Recv communication between two processes
        if (my_rank == 0 ){
        MPI_Send(&ch, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        }
        else if (my_rank == 1){
            MPI_Recv(&ch, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //printf("Process 1 received number %d from process 0 \n", number);

        }
        else {
        printf("Hi I'm Process %d, I am not involved in the communication\n", my_rank);

        }
 }
 double end_time = MPI_Wtime();
 if (my_rank == 0) {
     printf("Elapsed time: %f seconds\n", end_time - start_time);
     printf("Average round trip time: %f microseconds\n", 
         ((end_time - start_time) / (iterations)) * 1e6);
     printf("Average one-way time: %f microseconds\n", 
         ((end_time - start_time) / (2 * iterations)) * 1e6);           
}
 MPI_Finalize();
 return 0;
}
