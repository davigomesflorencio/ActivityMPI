#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <time.h>

int monteCarlos(int n,int rank)
{
    srand(time(NULL) + rank);
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
    int it=0;
    double soma = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &len);
     
    if (rank == 0)
    {
        it = atoi(argv[1]);
        it=it/numtasks;
        int res = monteCarlos(it,rank);
        int buffer;    
        for (int i = 1; i < numtasks; i++)
        {
           MPI_Recv(&buffer, 1, MPI_INT, i, 44, MPI_COMM_WORLD, &status);
           soma+=buffer;
        }
        soma+=res;
        printf("pi = %f\n",((soma* 4 )/(it*numtasks)));
    }
    else if (rank > 0)
    {
        it = atoi(argv[1]);
        it=it/numtasks;
        int res = monteCarlos(it,rank);
        //printf("%d---%d\n",rank,res);
        MPI_Send(&res, 1, MPI_INT, 0, 44, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}
