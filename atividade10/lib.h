#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_VALUE 100000

double *alocarMatriz(int Linhas,int Colunas);
double *lerMatriz(FILE *arquivoMatriz, int *linhas, int *colunas);
void imprimirMatriz(double *m, int linhas, int colunas);
void escreverMatriz(char *nomeDoArquivo,double *m, int linhas, int colunas);
void liberarMatriz(double *m);