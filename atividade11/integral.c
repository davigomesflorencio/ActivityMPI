#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double f(double x)
{
    double return_val = 0.0;
    // return_val = x*x*x*x - x*x*x + x*x - x + 1;
    //return_val =exp(x);
    return_val = cos(x * log(1.0 / x));
    return return_val;
}
void trap(double a, double b, int n, double *total)
{
    double x = 0, local_a, local_b, h, integral;
    int i, local_n, tid, th;
    th = omp_get_num_threads();
    tid = omp_get_thread_num();
    h = (b - a) / n;
    local_n = n / th;
    local_a = a + tid * local_n * h;
    local_b = local_a + local_n * h;
    //printf("[%lf %lf]\n", local_a, local_b);
    integral = (f(local_a) + f(local_b)) / 2.0;
    x = local_a;
    for (i = 1; i < local_n; i++)
    {
        x += h;
        integral += f(x);
    }

    integral *= h;
#pragma omp critical
    {
        *total += integral;
    }
}
int main(int argc, char *argv[])
{
    double a, b, h, total = 0;
    int n, i;

    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoi(argv[3]);

#pragma omp parallel
    trap(a, b, n, &total);

    printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, total);

    return 0;
}