#include <stdio.h>
#include <omp.h>

int main() {
  #pragma omp parallel sections default(none) shared(stdout)
  {
    #pragma omp section
    {
      //  printf("thread %d\n", omp_get_thread_num());
      printf("Section A executed by thread %d\n", omp_get_thread_num());
    }
    #pragma omp section
    {
      printf("Section B executed by thread %d\n", omp_get_thread_num());
    }
  } // implicit barrier: wait for all sections
  return 0;
}
