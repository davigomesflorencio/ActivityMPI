#include <stdio.h>
#include "mpi.h"
#include <omp.h>
#include <stdlib.h>
#include "lib.h"
#define MATRIZ 1000
#define TAM_MATRIZ 2000
#define MATRIZ_LOCAL 4000
#define VETOR 6000
#define TAM_VETOR 7000

int main(int argc, char *argv[])
{
    if (argc == 5)
    {
        int numtasks, rank, len, local;
        int m1_l, m1_c, m2_l, m2_c;
        double *m1, *m2, *bufferlocal, *m3;
        char hostname[MPI_MAX_PROCESSOR_NAME];
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Get_processor_name(hostname, &len);
        //numero de threads

        int numthreads = atoi(argv[4]);
        //Recupera os nomes dos arquivos da matriz 1, matriz 2 e matriz resultado.
        if (rank == 0)
        {
            char *nomearq1 = argv[1];
            char *nomearq2 = argv[2];
            //Carrega a matriz e o vetor na memória.
            FILE *arq;
            arq = fopen(nomearq1, "r");
            m1 = lerMatriz(arq, &m1_l, &m1_c);
            arq = fopen(nomearq2, "r");
            m2 = lerMatriz(arq, &m2_l, &m2_c);
            if (m1_c != m2_l)
            {
                printf("ERROR: Impossivel realizar multiplicação");
                MPI_Abort(MPI_COMM_WORLD, 2);
            }
            if (m1_l % numtasks != 0)
            {
                printf("ERROR: Impossivel particionar matriz");
                MPI_Abort(MPI_COMM_WORLD, 2);
            }
            //Calcula a quantidade de trabalho local (número de linhas)
            local = (m1_l / numtasks);
            fclose(arq);
        }

        //Envia as dimensoes da matriz 2 para os outros processos
        MPI_Bcast(&m2_l, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&m2_c, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank > 0)
        {
            m2 = alocarMatriz(m2_l, m2_c);
        }
        //Envia a matriz 2 para os outros processos
        MPI_Bcast(m2, m2_l * m2_c, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Envia as partições das matriz 1 para os outros processos.
        //(local/m2_l) representa as linha da matriz local
        MPI_Bcast(&local, 1, MPI_INT, 0, MPI_COMM_WORLD);
        bufferlocal = alocarMatriz(local, m2_l);
        MPI_Scatter(m1, local * m2_l, MPI_DOUBLE, bufferlocal, local * m2_l, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        //Faz seu cálculo local.
        //(local) representa as linha da matriz local
        double *calc_local = alocarMatriz(local, m2_c);
        
        int i, j, k;
#pragma omp parallel for shared(local, m2_l, m2_c, calc_local, bufferlocal, m2) private(i, j, k) schedule(static) num_threads(numthreads)
        for (i = 0; i < local; i++)
        {
            for (j = 0; j < m2_c; j++)
            {
                for (k = 0; k < m2_l; k++)
                {
                    calc_local[i * (m2_c) + j] += bufferlocal[i * (m2_l) + k] * m2[(k * m2_c) + j];
                }
            }
        }

        if (rank == 0)
        {
            m3 = alocarMatriz(m1_l, m2_c);
        }

        //enviando seu resultado para o processo 0
        MPI_Gather(calc_local, (local * m2_c), MPI_DOUBLE, m3, (local * m2_c), MPI_DOUBLE, 0, MPI_COMM_WORLD);
        //Consolida o resultado na memória e salva em arquivo.

        if (rank == 0)
        {
            char *nomearq3 = argv[3];
            escreverMatriz(nomearq3, m3, m1_l, m2_c);
            liberarMatriz(m1);
            liberarMatriz(m2);
            liberarMatriz(m3);
        }
        MPI_Finalize();
    }
    else
    {
        printf("ERROR: Falta de parâmetros");
        exit(0);
        MPI_Finalize();
    }
}
