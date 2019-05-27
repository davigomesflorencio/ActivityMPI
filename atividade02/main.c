#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VALUE 10000
float *alocar_vetor(int n)
{
    float *vetor = malloc(sizeof(float) * n);
    return vetor;
}
float **alocar_matriz(int n, int m)
{
    float **matriz = (float **)malloc(sizeof(float *) * n);
    for (int i = 0; i < n; i++)
    {
        matriz[i] = malloc(sizeof(float) * m);
    }

    return matriz;
}
float *multiplica_matrizes(float **m1, float *m2, int l1, int c1, int l2, int c2)
{
    float *v = alocar_vetor(l1);
    for (int i = 0; i < l1; i++)
    {
        for (int j = 0; j < c1; j++)
        {
            v[i] = 0;
            for (int k = 0; k < c1; k++)
            {
                v[i] += m1[i][k] * m2[k];
            }
        }
    }
    return v;
}
int main(int argc, char const *argv[])
{
    if (argc == 4)
    {
        float *vetor;
        float **matriz;

        //vetor
        FILE *arq;
        char ch[MAX_VALUE];
        int num_linha_arq1 = 0, col_vetor = 0, count1 = 0;
        arq = fopen(argv[1], "r");
        if (arq == NULL)
        {
            printf("ERROR: nao foi possivel abrir o arquivo que contem o vetor\n");
        }
        else
        {
            while ((fscanf(arq, "%s", ch)) != EOF)
            {
                num_linha_arq1++;
                if (num_linha_arq1 == 1)
                {
                    col_vetor = atoi(ch);
                    vetor = alocar_vetor(col_vetor);
                }
                else
                {
                    char str[MAX_VALUE];
                    size_t destination_size = sizeof(ch);
                    strncpy(str, ch, destination_size);
                    str[destination_size - 1] = '\0';
                    char *token = strtok(str, ":");
                    while (token != NULL && count1 < col_vetor)
                    {
                        vetor[count1] = atof(token);
                        count1++;
                        token = strtok(NULL, ":");
                    }
                }
            }
        }
        fclose(arq);
        //MATRIZ
        int num_linha_arq2 = 0, col_matriz = 0, lin_matriz = 0, count2 = 0;
        arq = fopen(argv[2], "r");
        if (arq == NULL)
        {
            printf("ERROR: nao foi possivel abrir o arquivo que contem a matriz\n");
        }
        else
        {
            while ((fscanf(arq, "%s", ch)) != EOF)
            {
                num_linha_arq2++;
                count2 = 0;
                if (num_linha_arq2 == 1)
                {
                    lin_matriz = atoi(ch);
                }
                else if (num_linha_arq2 == 2)
                {
                    col_matriz = atoi(ch);
                    matriz = alocar_matriz(lin_matriz, col_matriz);
                }
                else if (num_linha_arq2 >= 2 && num_linha_arq2 <= lin_matriz + 2)
                {
                    char str[MAX_VALUE];
                    size_t destination_size = sizeof(ch);
                    strncpy(str, ch, destination_size);
                    str[destination_size - 1] = '\0';
                    char *token = strtok(str, ":");
                    while (token != NULL && count2 < col_matriz)
                    {
                        matriz[num_linha_arq2 - 3][count2] = atof(token);
                        count2++;
                        token = strtok(NULL, ":");
                    }
                }
            }
        }
        fclose(arq);
        FILE *arq_resultado;
        arq_resultado = fopen(argv[3], "w");
        if (arq_resultado == NULL)
            printf("ERROR: nao foi possivel abrir o arquivo do resultado\n");
        else
        {
            fprintf(arq_resultado, "%d\n", lin_matriz);
            float *m = multiplica_matrizes(matriz, vetor, lin_matriz, col_matriz, 1, col_vetor);
            for (int i = 0; i < 1; i++)
            {
                for (int j = 0; j < lin_matriz; j++)
                {
                    if (j == lin_matriz - 1)
                    {
                        fprintf(arq_resultado, "%.2f\n", m[j]);
                    }
                    else
                    {
                        fprintf(arq_resultado, "%.2f:", m[j]);
                    }
                }
            }
        }
        fclose(arq_resultado);
    }else{
        printf("ERROR: Programa finalizado por falta de parametros.\n");
    }
    return 0;
}
