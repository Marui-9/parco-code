/* File:  
 *    pth_hello.c
 *
 * Purpose:
 *    Illustrate basic use of pthreads:  create some threads,
 *    each of which prints a message.
 *
 * Input:
 *    none
 * Output:
 *    message from each thread
 *
 * Compile:  gcc -g -Wall -o pth_hello pth_hello.c -lpthread
 * Usage:    ./pth_hello <thread_count>
 *
 * IPP:   Section 4.2 (p. 153 and ff.)
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <time.h>

const int MAX_THREADS = 64;
clock_t start, end;
/* Global variable:  accessible to all threads */
int thread_count;  

void Usage(char* prog_name);
void *Hello(void* rank);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles; 

   /* Get number of threads from command line */
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

   thread_handles = malloc (thread_count*sizeof(pthread_t)); 
    srand(time(NULL));
     double elapsed_seconds = 0.0;
   for (thread = 0; thread < thread_count; thread++)  {
     start = clock();
      pthread_create(&thread_handles[thread], NULL, Hello, (void*) thread);  
      end = clock();
      elapsed_seconds += (double)(end - start) / CLOCKS_PER_SEC;
      }
      printf("  Thread Creation Time taken: %f seconds\n", elapsed_seconds);
     printf("average creation time per thread: %f seconds\n", elapsed_seconds / thread_count);



       elapsed_seconds = 0.0;
    for (thread = 0; thread < thread_count; thread++) {
        start = clock();
      pthread_join(thread_handles[thread], NULL); 
    end = clock();
    }
    elapsed_seconds += (double)(end - start) / CLOCKS_PER_SEC;
    printf("  Thread Joining Time taken: %f seconds\n", elapsed_seconds);
    printf("average joining time per thread: %f seconds\n", elapsed_seconds / thread_count);


   free(thread_handles);
   return 0;
}  /* main */

/*-------------------------------------------------------------------*/
void *Hello(void* rank) {
   //long my_rank = (long) rank;  /* Use long in case of 64-bit system */ 

  // printf("Thread %ld of %d\n has been created\n", my_rank, thread_count);

   return NULL;
}  /* Hello */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  /* Usage */
