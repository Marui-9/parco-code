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
int a = 2;
int b = 16;
int n = 1024;
double total_integral = 0.0;
pthread_mutex_t mutex;

void Usage(char* prog_name);
void *Worker(void* rank);  /* Thread function */
double *Trapezoidal_th(double a, double b, int n, int thread_count);
double f(double x);
/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles; 

   /* Get number of threads from command line */
   if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);  
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);

   thread_handles = malloc (thread_count*sizeof(pthread_t)); 
   for (thread = 0; thread < thread_count; thread++)  {
      pthread_create(&thread_handles[thread], NULL, Worker, (void*) thread);  
      }



    for (thread = 0; thread < thread_count; thread++) {
      pthread_join(thread_handles[thread], NULL); 
    }
   free(thread_handles);
   return 0;
}  /* main */

/*-------------------------------------------------------------------*/
void *Worker(void* rank) {
   
   return Trapezoidal_th(a, b, n, (long) rank);
}  /* Worker     */

/*-------------------------------------------------------------------*/
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads>\n", prog_name);
   fprintf(stderr, "0 < number of threads <= %d\n", MAX_THREADS);
   exit(0);
}  /* Usage */
/*-------------------------------------------------------------------*/
double * Trapezoidal_th(double a, double b, int n, int my_rank) {
    printf("Thread %ld: of %d\n", my_rank, thread_count);
    int my_first_col = my_rank*(n/thread_count);
    int my_last_col = (my_rank+1)*(n/thread_count)-1;
    printf("Thread %ld: my_first_col= %d\n", my_rank, my_first_col);
    printf("Thread %ld: my_last_col= %d\n", my_rank, my_last_col);
    double h = (b - a) / n;
    double *result = malloc(sizeof(double));
    double integral = (f(a) + f(b)) / 2.0;
    for (int i = my_first_col; i < my_last_col; i++) {
        double x = a + i * h;
        integral += f(x);
    }
    *result = integral * h;
    pthread_mutex_lock(&mutex);
    printf("Thread %ld acquires lock: partial integral = %f\n", my_rank, *result);
    total_integral += *result;
    
    printf("Thread %ld: total integral = %f\n", my_rank, total_integral);
    pthread_mutex_unlock(&mutex);
    return result;
}
/*-------------------------------------------------------------------*/
double f(double x) {
    return (x*x+10);
}