all:compara

compara: lib.o main.o
	gcc -o compara lib.o main.o

lib.o:lib.c
	gcc -o lib.o -c lib.c

main.o:comparamatrizes.c lib.h
	gcc -o main.o -c comparamatrizes.c 

clean:
	rm -rf *.o

mrproper:clean
	rm -rf compara