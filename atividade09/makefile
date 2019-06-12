all:matmult

matmult: lib.o mult.o 
	gcc -o maltmult lib.o mult.o 

matmultfopenmp: lib.o mult2.o
	gcc -o maltmult lib.o mult.o -fopenmp

lib.o:lib.c
	gcc -o lib.o -c lib.c

mult.o:Multiplicacao.c lib.h
	gcc -o mult.o -c Multiplicacao.c

mult2.o:
	gcc -o mult.o -c Multiplicacao.c -fopenmp
	
criamatriz:
	gcc criaMatriz.c -o criamatriz

clean:
	rm -rf *.o

mrproper:clean
	rm -rf maltmult
	rm -rf criamatriz