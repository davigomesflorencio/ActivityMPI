#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VALUE 100000

float *alocarMatriz(int Linhas,int Colunas);
float *lerMatriz(FILE *arquivoMatriz, int *linhas, int *colunas);
void imprimirMatriz(float *m, int linhas, int colunas);
void escreverMatriz(char *nomeDoArquivo, float *m, int linhas, int colunas);
void liberarMatriz(float *m);