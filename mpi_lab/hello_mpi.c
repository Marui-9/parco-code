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
 /* seed RNG once per process */
 srand((unsigned)time(NULL) + (unsigned)my_rank);
 int number = get_random();
// Send Recv communication between two processes
 if (my_rank == 0 ){
 MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

}
 else if (my_rank == 1){
 MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
 printf("Process 1 received number %d from process 0 \n", number);

}
 else {
 printf("Hi I'm Process %d, I am not involved in the communication\n", my_rank);

 }
 MPI_Finalize();
 return 0;
}