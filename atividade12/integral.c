#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include <omp.h>

double f(double x)
{
    double return_val = 0.0;
    // return_val = x*x*x*x - x*x*x + x*x - x + 1;
    //return_val = exp(x);
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
    double integral = 0;
    double local = 0;
    double a, b;
    double mpi_h , mpi_n , mpi_loc_a , mpi_loc_b ,mpi_loc_n;
    int numtasks, rank, len, n;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &len);

    a = atof(argv[1]);
    b = atof(argv[2]);
    n = atoi(argv[3]);

    
    mpi_loc_n=n/numtasks;
    mpi_h = (b -a)/n;
    mpi_loc_a = a + rank * mpi_loc_n * mpi_h;
    mpi_loc_b = mpi_loc_a + mpi_loc_n * mpi_h;

    //printf("[%lf , %lf , %lf ,%lf]\n",mpi_loc_n,mpi_h,mpi_loc_a,mpi_loc_b);
    
    #pragma omp parallel
    trap(mpi_loc_a, mpi_loc_b, mpi_loc_n, &local);

    MPI_Reduce(&local, &integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("%d trapézios, estimativa de %.2f a %.2f = %.5f\n", n, a, b, integral);
    }
    MPI_Finalize();
    return 0;
}