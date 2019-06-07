#include <stdio.h>
#include "lib.h"

float *multiplica_matrizes(float *m1, float *m2, int l1, int c1, int l2, int c2)
{

    if (c1 == l2)
    {
        float *v = alocarMatriz(l1, c2);
        for (int i = 0; i < l1; i++)
        {
            for (int j = 0; j < c2; j++)
            {
                for (int k = 0; k < c1; k++)
                {
                    *(v + i) += *(m1 + (i * c1) + k) * *(m2 + k);
                }
            }
        }
        return v;
    }
    else
    {
        printf("ERRO: Parametros invalidos COlUNAS DA 1º MATRIZ diferentes da LINHA DA 2º MATRIZ\n");
        return NULL;
    }
}
int main(int argc, char const *argv[])
{
    if (argc == 4)
    {
        int m1_l, m1_c, m2_l, m2_c;
        FILE *arq;
        arq = fopen(argv[1], "r");
        float *m1 = lerMatriz(arq, &m1_l, &m1_c);

        arq = fopen(argv[2], "r");
        float *m2 = lerMatriz(arq, &m2_l, &m2_c);

        float *m3 = multiplica_matrizes(m1, m2, m1_l, m1_c, m2_l, m2_c);
        char *c = (char *)argv[3];

        if (m3 != NULL)
        {
            escreverMatriz(c, m3, m1_l, m2_c);
        }
        liberarMatriz(m1);
        liberarMatriz(m2);
        liberarMatriz(m3);
        fclose(arq);
    }
    else
    {
        printf("ERROR : Parametros invalidos\n");
    }
    return 0;
}
