#include <stdio.h>
#include <unistd.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char** argv)
{
    int i;
#ifdef _OPENMP
#pragma omp parallel private(i)
{
    i = omp_get_thread_num();
	sleep(1);
	printf( "hello world %d\n", i );
}
#else
    i=-1;
	printf( "hello world %d\n", i );
#endif 

    return 0;
}

