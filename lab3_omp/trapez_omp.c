/* File:  
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
clock_t start, end;
/* Problem parameters */
int a = 2;
int b = 16;
int n = 1024;

double Trapezoidal(double a, double b, int n);
double f(double x);
/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   /* Optional: allow user to override number of trapezoids */
   if (argc == 2) {
      int val = strtol(argv[1], NULL, 10);
      if (val > 0) n = val;
   } else if (argc > 2) {
      fprintf(stderr, "usage: %s [n_trapezoids]\n", argv[0]);
      return 1;
   }

   start = clock();
   double integral = Trapezoidal(a, b, n);
   end = clock();

   printf("With n = %d trapezoids, integral = %.6f\n", n, integral);
   printf("Elapsed time = %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
   return 0;
}

/*-------------------------------------------------------------------*/
double Trapezoidal(double a, double b, int n) {
   double h = (b - a) / (double) n;
   double integral = (f(a) + f(b)) / 2.0;
   for (int i = 1; i <= n - 1; i++) {
      double x = a + i * h;
      integral += f(x);
   }
   return integral * h;
}
/*-------------------------------------------------------------------*/
double f(double x) {
   return (x * x + 10);
}