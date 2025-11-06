#include <stdio.h>
#include <unistd.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char** argv){
	
    int i;

#ifdef _OPENMP
    i = omp_get_thread_num();
#else
    i=-1;
#endif

    printf( "hello world %d\n", i );

    return 0;
}
