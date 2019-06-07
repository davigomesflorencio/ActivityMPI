#include <stdio.h>
#include "mpi.h"
#include <string.h>
#include <stdlib.h>

#define SIZE 16
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

MPI_Status status;
int left, right, tag, size, rank;

int printArrayForRank(int rank, int *array, int arraySize)
{
   int i;
   char *output = (char *)malloc((arraySize + 100) * 4 * sizeof(char));
   sprintf(output, "Rank %d: ", rank);
   for (int i = 0; i < arraySize; i++)
      sprintf(output, "%s %d", output, array[i]);
   sprintf(output, "%s\n", output);
   printf("%s", output);
   fflush(stdout);
   free(output);
   return i;
}

int MPI_Ring_broadcast(int *array, int arraySize, MPI_Comm comm)
{
   if (rank == 0)
   {
      MPI_Send(array, arraySize, MPI_INT, right, tag, comm);
      MPI_Send(array, arraySize, MPI_INT, left, tag, comm);
   }
   else if (rank >= (size / 2))
   {
      MPI_Recv(array, arraySize, MPI_INT, right, tag, comm, &status);
      MPI_Send(array, arraySize, MPI_INT, left, tag, comm);
   }
   else
   {
      MPI_Recv(array, arraySize, MPI_INT, left, tag, comm, &status);
      MPI_Send(array, arraySize, MPI_INT, right, tag, comm);
   }
}

int MPI_Mesh_broadcast(int *array, int arraySize, MPI_Comm comm)
{
   int dims[2] = {4, 4};
   int periods[2] = {0, 0};
   int vizinhos[4];
   int coords[2];
   MPI_Comm cartcomm;

   MPI_Cart_create(comm, 2, dims, periods, 0, &cartcomm);

   MPI_Cart_shift(cartcomm, 0, 1, &vizinhos[UP], &vizinhos[DOWN]);
   MPI_Cart_shift(cartcomm, 1, 1, &vizinhos[LEFT], &vizinhos[RIGHT]);

   MPI_Cart_coords(cartcomm, rank, 2, coords);

   if (rank == 0)
   {
      MPI_Send(array, arraySize, MPI_INT, vizinhos[RIGHT], tag, MPI_COMM_WORLD);
      MPI_Send(array, arraySize, MPI_INT, vizinhos[DOWN], tag, MPI_COMM_WORLD);
   }
   else if (coords[1] == 0)
   {
      MPI_Recv(array, arraySize, MPI_INT, vizinhos[UP], tag, MPI_COMM_WORLD, &status);
      MPI_Send(array, arraySize, MPI_INT, vizinhos[RIGHT], tag, MPI_COMM_WORLD);
      MPI_Send(array, arraySize, MPI_INT, vizinhos[DOWN], tag, MPI_COMM_WORLD);
   }
   else
   {
      MPI_Recv(array, arraySize, MPI_INT, vizinhos[LEFT], tag, MPI_COMM_WORLD, &status);
      MPI_Send(array, arraySize, MPI_INT, vizinhos[RIGHT], tag, MPI_COMM_WORLD);
   }
}

int main(int argc, char *argv[])
{
   int *array, arraySize;
   tag = 0;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if (size == SIZE)
   {

      arraySize = atoi(argv[1]);
      array = (int *)malloc(arraySize * sizeof(int));

      left = (rank - 1 + size) % size;
      right = (rank + 1) % size;

      // Inicializa o array com 0.
      if (rank == 0)
      {
         for (int i = 0; i < arraySize; i++)
            array[i] = 0;
      }

      // Imprime o array inicial de todos os processos.
      printArrayForRank(rank, array, arraySize);

      // Faz broadcast usando uma topologia anel.
      MPI_Ring_broadcast(array, arraySize, MPI_COMM_WORLD);

      // Barreira de sincronização.
      MPI_Barrier(MPI_COMM_WORLD);

      // Imprime o array após o broadcast na topologia anel.
      printArrayForRank(rank, array, arraySize);

      // Barreira de sincronização.
      MPI_Barrier(MPI_COMM_WORLD);

      // Reconfigura o array com todos os elementos iguais a 1000.
      if (rank == 0)
      {
         for (int i = 0; i < arraySize; i++)
            array[i] = 1000;
      }

      // Faz broadcast usando uma topologia mesh.
      MPI_Mesh_broadcast(array, arraySize, MPI_COMM_WORLD);

      // Barreira de sincronização.
      MPI_Barrier(MPI_COMM_WORLD);

      // Imprime o array após o broadcast na topologia mesh.
      printArrayForRank(rank, array, arraySize);

      free(array);
      MPI_Finalize();
   }
   else
   {
      printf("São necessários %d processos.\n", SIZE);
      MPI_Finalize();
   }
   return 0;
}