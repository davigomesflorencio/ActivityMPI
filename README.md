# ComputacaoParalela-MPI
Repositório de atividades da disciplina de computação paralela UFC Campus Quixadá 2019.1 
- ATIVIDADE 01
>
    Objetivo:
    Este programa deverá aproximar o valor de π através do método de Monte Carlo.
- ATIVIDADE 02
>
    Objetivo
    O objetivo desta atividade é realizar a implementação serial da multiplicação matriz/vetor. Nas próximas atividades iremos paralelizar essa versão inicial
- ATIVIDADE 03
>
    Objetivo
    Praticar o uso das primitivas de comunicação ponto-a-ponto do MPI. Coloque o código dessa atividade na pasta atividades/atividade03.

    Problema
    Vamos transformar o programa da Atividade 01 em uma versão paralela. A diferença é que o cálculo do π deve ser distribuído entre vários processos MPI.
- ATIVIDADE 04
> 
    Objetivo:
    Praticar o uso das primitivas de comunicação coletivas do MPI. Coloque o código dessa atividade na pasta atividades/atividade04.

    Problema:
    Resolva o mesmo problema apresentado na Atividade 03, porém agora usando exclusivamente chamadas coletivas. Pode ser qualquer chamada, mas nenhuma operação ponto-a-ponto deve ser utilizada.
- ATIVIDADE EXTRA
> 
    Objetivo:
    O objetivo dessa atividade é propor uma reorganização do código para auxiliar nas próximas atividades. Vocês tem liberdade de organizar o código como desejar. Entretanto, já devem ter observado que várias atividades envolvem a manipulação de matrizes no formato textual que descrevi na Atividade 02. O que proponho nesta atividade é a definição de funções para lidar com esses arquivos com maior facilidade.
- Atividades 05
>
    Objetivo:
    O objetivo desta atividade é realizar a implementação paralela da multiplicação matriz/vetor. É uma expansão do que já foi feito na Atividade 02. Nesta atividade, só devem ser usadas operações ponto-a-ponto bloqueantes do MPI.
- ATIVIDADE 06
 >
    Objetivo:
    Atividade 05 usando operações coletivas
- ATIVIDADE 07
> 
    Objetivo:
    Desenvolver um programa paralelo que faça o cálculo da integral de uma função usando o método do trapézio.
- ATIVIDADE 08
> 
    Objetivo:
    Escrever duas funções em um programa MPI que faça um broadcast de uma mensagem definida pelo processo 0 à todos os outros processos. Cada função irá aplicar uma topologia diferente. Somente primitivas ponto-a-ponto podem ser utilizadas. Para este exercício, vamos considerar um número fixo de 16 processos.

- ATIVIDADE 09
> 
    Objetivo:
    O objetivo desta atividade é desenvolver a multiplicação de matrizes utilizando OpenMP para paralelismo.