#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <time.h>

int monteCarlos(int n, int rank)
{
    srand(time(NULL) + (rank * rank));
    int acertos = 0, i = 0;
    double x, y, z;
    for (i = 0; i <= n; i++)
    {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        z = x * x + y * y;
        if (z < 1)
            acertos++;
    }
    return acertos;
}

int main(int argc, char *argv[])
{
    int numtasks, rank, len, rc;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;
    int it = 0;
    int soma = 0;
    if (argc == 2)
    {
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Get_processor_name(hostname, &len);

        it = atoi(argv[1]);
        it = it / numtasks;
        int res = monteCarlos(it, rank);
        MPI_Reduce(&res, &soma, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            double f = ((double)soma / (it * numtasks)) * 4;
            printf("pi = %lf\n", f);
        }
        MPI_Finalize();
    }
    else
    {
        printf("ERROR: Falta de parâmetros\n");
        exit(0);
        MPI_Finalize();
    }
}
