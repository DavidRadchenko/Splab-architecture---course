all: hexeditplus counter

counter: task4.o
	gcc -g -m32 -Wall -o counter task4.o
	
task4.o: task4.c
	gcc -m32 -g -Wall -ansi -c -o task4.o task4.c
	

hexeditplus: task1.o 
	gcc -g -m32 -Wall -o hexeditplus task1.o

task1.o: task1.c
	gcc -m32 -g -Wall -ansi -c -o task1.o task1.c

.PHONY: clean

clean: 
	rm -f *.o hexeditplus counter
