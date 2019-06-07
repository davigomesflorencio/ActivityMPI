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
        int numtasks, rank, len, tam;
        char hostname[MPI_MAX_PROCESSOR_NAME];
        MPI_Status status;
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Get_processor_name(hostname, &len);

        if (rank == 0)
        {
            //Recupera os nomes dos arquivos da matriz, vetor e resultado.
            char *nomearq1 = argv[1];
            char *nomearq2 = argv[2];
            char *nomearq3 = argv[3];

            //Carrega a matriz e o vetor na memória.
            int m1_l, m1_c, m2_l, m2_c;
            FILE *arq;
            arq = fopen(nomearq1, "r");
            float *m1 = lerMatriz(arq, &m1_l, &m1_c);
            arq = fopen(nomearq2, "r");
            float *m2 = lerMatriz(arq, &m2_l, &m2_c);

            //Calcula a quantidade de trabalho local (número de linhas)
            tam = m1_l / numtasks;
            int valor = tam * m1_c;

            //Envia o vetor para os outros processos
            //Envia as partições das matrizes para os outros processos.
            for (int k = 1; k < numtasks; k++)
            {
                MPI_Send(&m2_l, 1, MPI_INT, k, TAM_VETOR, MPI_COMM_WORLD);
                MPI_Send(m2, m2_l, MPI_FLOAT, k, VETOR, MPI_COMM_WORLD);
                MPI_Send(&valor, 1, MPI_INT, k, TAM_MATRIZ + k, MPI_COMM_WORLD);
                MPI_Send(&m1[valor * k], valor, MPI_FLOAT, k, MATRIZ + k, MPI_COMM_WORLD);
            }

            //Faz seu cálculo local.
            float *m3 = alocarMatriz(m1_l, m2_c);
            for (int i = 0; i < tam; i++)
            {
                for (int j = 0; j < m2_c; j++)
                {
                    for (int k = 0; k < m1_c; k++)
                    {
                        m3[i] += m1[(i * m1_c) + k] * m2[k];
                    }
                }
            }
            //Recebe as contribuições de todos os outros processos.
            for (int k = 1; k < numtasks; k++)
            {
                MPI_Recv(&m3[tam * k], tam, MPI_FLOAT, k, MATRIZ_LOCAL + k, MPI_COMM_WORLD, &status);
            }
            escreverMatriz(nomearq3, m3, m1_l, m2_c);
            liberarMatriz(m1);
            liberarMatriz(m2);
            liberarMatriz(m3);
            fclose(arq);
        }
        else
        {
            //Recebe tamvetor e vetor do processo 0
            int tam_vetor;
            MPI_Recv(&tam_vetor, 1, MPI_INT, 0, TAM_VETOR, MPI_COMM_WORLD, &status);
            float buffervetor[tam_vetor];
            MPI_Recv(&buffervetor, tam_vetor, MPI_FLOAT, 0, VETOR, MPI_COMM_WORLD, &status);

            //Recebe tam_matriz e a matriz do processo 0
            int tam_matriz;
            MPI_Recv(&tam_matriz, 1, MPI_INT, 0, TAM_MATRIZ + rank, MPI_COMM_WORLD, &status);
            float buffermatriz[tam_matriz];
            MPI_Recv(&buffermatriz, tam_matriz, MPI_FLOAT, 0, MATRIZ + rank, MPI_COMM_WORLD, &status);

            //Realizando calculo local enviando seu resultado para o processo 0
            float *matriz_local = alocarMatriz((tam_matriz / tam_vetor), 1);
            for (int i = 0; i < (tam_matriz / tam_vetor); i++)
            {
                for (int j = 0; j < 1; j++)
                {
                    for (int k = 0; k < tam_vetor; k++)
                    {
                        matriz_local[i] += buffermatriz[(i * tam_vetor) + k] * buffervetor[k];
                    }
                }
            }
            MPI_Send(matriz_local, (tam_matriz / tam_vetor), MPI_FLOAT, 0, MATRIZ_LOCAL + rank, MPI_COMM_WORLD);
        }
        MPI_Finalize();
    }
    else
    {
        exit(0);
        MPI_Finalize();
    }
}
