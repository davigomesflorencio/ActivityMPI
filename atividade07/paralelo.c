#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

double f(double x)
{
    double return_val = 0.0;
    // return_val = x*x*x*x - x*x*x + x*x - x + 1;
    //return_val = exp(x);
    return_val = cos(x * log(1.0 / x));
    return return_val;
}

int main(int argc, char *argv[])
{
    double integral = 0;
    double local = 0;
    double a, b, h, x;
    int numtasks, rank, len, n;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &len);

    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoi(argv[3]);

    h = (b - a) / n;
    x = (((b - a) / numtasks) * rank) + a;

    for (int i = 0; i < (n / numtasks); i++)
    {
        x += h;
        local += f(x);
    }
    MPI_Reduce(&local, &integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        integral += (f(a) - f(b)) / 2.0;
        integral *= h;
        printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral);
    }
    MPI_Finalize();
    return 0;
}