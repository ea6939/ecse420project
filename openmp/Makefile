parProgram: parProgram.o common.o
	gcc -O0 -fopenmp -o parProgram parProgram.o common.o

parProgram.o: parProgram.c common.h
	gcc -O0 -c parProgram.c

common.o: common.c common.h
	gcc -O0 -c common.c

clean:
	rm *.o parProgram
