all: myELF

myELF: task3.o task2.o task1.o
	gcc -g -m32 -Wall -o myELF task3.o task2.o task1.o

task3.o: task3.c
	gcc -m32 -g -Wall -ansi -c -o task3.o task3.c

task2.o: task2.c
	gcc -m32 -g -Wall -ansi -c -o task2.o task2.c

task1.o: task1.c
	gcc -m32 -g -Wall -ansi -c -o task1.o task1.c

.PHONY: clean

clean: 
	rm -f *.o myELF
