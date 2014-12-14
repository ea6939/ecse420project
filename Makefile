serialProgram: serialProgram.o common.o
	gcc -O0 -o serialProgram serialProgram.o common.o

serialProgram.o: serialProgram.c common.h
	gcc -O0 -c serialProgram.c

common.o: common.c common.h
	gcc -O0 -c common.c

clean:
	rm *.o serialProgram