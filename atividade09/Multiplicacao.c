#include "lib.h"
#include <stdio.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    if (argc == 4)
    {
        //Recupera os nomes dos arquivos da matriz, vetor e resultado.
        char *nomearq1 = argv[1];
        char *nomearq2 = argv[2];
        char *nomearq3 = argv[3];
        //Carrega a matriz e o vetor na memória.
        int m1_l, m1_c, m2_l, m2_c;
        FILE *arq;
        arq = fopen(nomearq1, "r");
        double *m1 = lerMatriz(arq, &m1_l, &m1_c);
        arq = fopen(nomearq2, "r");
        double *m2 = lerMatriz(arq, &m2_l, &m2_c);

        if (m1_c != m2_l)
        {
            printf("Impossivel realizar multiplicação de matrizes");
            exit(0);
        }
        double *m3 = alocarMatriz(m1_l, m2_c);
        int a, b, c;
        
        #pragma omp parallel for shared(m1_l, m1_c, m2_c, m3, m2, m1) private(a, b, c) schedule (static)
        for (a = 0; a < m1_l; a++)
        {
            for (b = 0; b < m2_c; b++)
            {
                for (c = 0; c < m1_c; c++)
                {
                    m3[(a * m1_l) + b] += m1[(a * m1_c) + c] * m2[(c * m2_l) + b];
                }
            }
        }

        escreverMatriz(nomearq3, m3, m1_l, m2_c);
        liberarMatriz(m1);
        liberarMatriz(m2);
        liberarMatriz(m3);
        return 0;
    }
    else
    {
        printf("ERROR: Parametros em falta");
        exit(0);
    }
}
