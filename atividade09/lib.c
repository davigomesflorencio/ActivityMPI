#include "lib.h"

double *alocarMatriz(int Linhas, int Colunas)
{
    double *p;
    int i;
    p = (double *)malloc(Linhas * Colunas * sizeof(double));
    if (!p)
        return (NULL);

    for (int i = 0; i < Linhas; i++)
    {
        for (int j = 0; j < Colunas; j++)
        {
            p[(i * Colunas) + j] = 0;
        }
    }

    return p;
}

double *lerMatriz(FILE *arquivoMatriz, int *linhas, int *colunas)
{

    if (arquivoMatriz == NULL)
    {
        printf("ERROR: nao foi possivel abrir o arquivo que contem a matriz\n");
    }
    else
    {
        double *m;
        int num_linha_arq2 = 0, col_matriz = 0, lin_matriz = 0, count2 = 0;
        char ch[MAX_VALUE];
        while ((fscanf(arquivoMatriz, "%s", ch)) != EOF)
        {
            num_linha_arq2++;
            count2 = 0;
            if (num_linha_arq2 == 1)
            {
                lin_matriz = atoi(ch);
                *linhas = lin_matriz;
            }
            else if (num_linha_arq2 == 2)
            {
                col_matriz = atoi(ch);
                *colunas = col_matriz;
                m = alocarMatriz(lin_matriz,col_matriz);
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
                    *(m + ((num_linha_arq2 - 3) * *colunas) + count2) = atof(token);
                    count2++;
                    token = strtok(NULL, ":");
                }
            }
        }
        return m;
    }
    return NULL;
}
void liberarMatriz(double *m)
{
    if (m != NULL)
    { 
        printf("SUCESS: Matriz Liberada\n");
        free(m);
    }
    else
    {
        printf("ERROR: Matriz nula\n");
    }
}

void imprimirMatriz(double *m, int linhas, int colunas)
{
    if (m != NULL)
    {
        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                if (j == colunas - 1)
                {
                    printf("%.4lf\n", *(m + (i * colunas) + j));
                }
                else
                {
                    printf("%.4lf:", *(m + (i * colunas) + j));
                }
            }
        }
    }
    else
    {
        printf("ERROR: Matriz nula\n");
    }
}
void escreverMatriz(char *nomeDoArquivo, double *m, int linhas, int colunas)
{
    FILE *arq_resultado;
    arq_resultado = fopen(nomeDoArquivo, "w+");
    if (arq_resultado == NULL)
    {
        printf("ERROR: nao foi possivel abrir o arquivo do resultado\n");
    }
    else if (m == NULL)
    {
        printf("ERROR: Matriz nula\n");
    }
    else if (linhas < 0 && colunas < 0)
    {
        printf("ERROR: Parametros Invalidos : linhas<0 ou colunas<0\n");
    }
    else
    {
        fprintf(arq_resultado, "%d\n", linhas);
        fprintf(arq_resultado, "%d\n", colunas);
        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                if (j == colunas - 1)
                {
                    fprintf(arq_resultado, "%05.4lf\n", *(m + (i * colunas) + j));
                }
                else
                {
                    fprintf(arq_resultado, "%05.4lf:", *(m + (i * colunas) + j));
                }
            }
        }
    }
}
