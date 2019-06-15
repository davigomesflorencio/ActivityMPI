#include "lib.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        //Recupera os nomes dos arquivos da matriz, vetor e resultado.
        char *nomearq1 = argv[1];
        char *nomearq2 = argv[2];
        //Carrega a matriz e o vetor na memória.
        int m1_l, m1_c, m2_l, m2_c;
        FILE *arq;
        arq = fopen(nomearq1, "r");
        double *m1 = lerMatriz(arq, &m1_l, &m1_c);
        arq = fopen(nomearq2, "r");
        double *m2 = lerMatriz(arq, &m2_l, &m2_c);

        if ((m1_l != m2_l) || (m1_c != m2_c))
        {
            printf("Matrizes com dimensões distintas");
            exit(0);
        }
        for (int a = 0; a < m1_l; a++)
        {
            for (int b = 0; b < m1_c; b++)
            {
                if (m1[(a * m1_c) + b] != m2[(a * m1_c) + b])
                {
                    printf("ERROR: Campos distintos:Matrizes distintas\n");
                    return 0;
                }
            }
        }
        printf("Matrizes iguais\n");
        return 0;
    }
    else
    {
        printf("ERROR: Parametros em falta");
        exit(0);
    }
}
