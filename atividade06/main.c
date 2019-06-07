#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include "lib.h"
#define MATRIZ 1000
#define TAM_MATRIZ 2000
#define MATRIZ_LOCAL 4000
#define VETOR 6000
#define TAM_VETOR 7000

int main(int argc, char *argv[])
{
    if (argc == 4)
    {
        int numtasks, rank, len, tam_local;
        int m1_l, m1_c, m2_l, m2_c;
        float *m1, *m2, *bufferlocal, *m3;
        char hostname[MPI_MAX_PROCESSOR_NAME];
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Get_processor_name(hostname, &len);

        //Recupera os nomes dos arquivos da matriz, vetor e resultado.
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
            //Calcula a quantidade de trabalho local (número de linhas)
            tam_local = (m1_l / numtasks) * m1_c;
            fclose(arq);
        }
        //Envia o vetor para os outros processos
        MPI_Bcast(&m2_l, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank > 0)
        {
            m2 = alocarMatriz(m2_l, 1);
        }
        MPI_Bcast(m2, m2_l, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //Envia as partições das matrizes para os outros processos.
        MPI_Bcast(&tam_local, 1, MPI_INT, 0, MPI_COMM_WORLD);
        bufferlocal = alocarMatriz(tam_local, 1);
        MPI_Scatter(m1, tam_local, MPI_FLOAT, bufferlocal, tam_local, MPI_FLOAT, 0, MPI_COMM_WORLD);

        //Faz seu cálculo local.
        float *calc_local = alocarMatriz((tam_local / m2_l), 1);
        for (int i = 0; i < (tam_local / m2_l); i++)
        {
            for (int j = 0; j < 1; j++)
            {
                for (int k = 0; k < m2_l; k++)
                {
                    calc_local[i] += bufferlocal[(i * m2_l) + k] * m2[k];
                }
            }
        }
        if (rank == 0)
        {
            m3 = alocarMatriz(m1_l, 1);
        }

        //enviando seu resultado para o processo 0
        MPI_Gather(&calc_local[0], (tam_local / m2_l), MPI_FLOAT, m3, (tam_local / m2_l), MPI_FLOAT, 0, MPI_COMM_WORLD);
        //Consolida o resultado na memória e salva em arquivo.
        if (rank == 0)
        {
            char *nomearq3 = argv[3];
            escreverMatriz(nomearq3, m3, m1_l, 1);
            liberarMatriz(m1);
            liberarMatriz(m2);
            liberarMatriz(m3);
        }
        MPI_Finalize();
    }
    else
    {
        exit(0);
        MPI_Finalize();
    }
}
