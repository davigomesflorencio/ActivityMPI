all:matmult_mpi

matmult_mpi: lib.o mult.o
	mpicc -o matmult_mpi lib.o mult.o -fopenmp

lib.o:lib.c
	gcc -o lib.o -c lib.c

mult.o:main.c lib.h
	mpicc -o mult.o -c main.c -fopenmp

criamatriz:criaMatriz.c
	gcc criaMatriz.c -o criamatriz

clean:
	rm -rf *.o

mrproper:clean
	rm -rf matmult_mpi
	rm -rf criamatriz