#include <stdio.h>
#include <stdlib.h>

int n = 6;
int a[] = {0,1,2,3,4,5};
int b[] = {6, 5, 4, 3, 2, 1};
int c[6];
int sum ()
{
    int i;
    int s=0;
    for (i=0; i<n; i++)
        s += a[i] + b[i];
    return s;
}
int main ()
{
    int *a = (int*) malloc(n * sizeof(int));
    int *b = (int*) malloc(n * sizeof(int));
    int *c = (int*) malloc(n * sizeof(int));
    printf("Sum: %d\n", sum());
    return 0;
}