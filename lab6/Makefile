all: task1 task2 task3 task4

task4: task4.o LineParser.o
	gcc -g -m32 -Wall -o task4 task4.o LineParser.o

task3: task3.o LineParser.o
	gcc -g -m32 -Wall -o task3 task3.o LineParser.o

task2: task2.o
	gcc -g -m32 -Wall -o task2 task2.o

task1: task1.o LineParser.o
	gcc -g -m32 -Wall -o task1 task1.o LineParser.o

task4.o: task4.c
	gcc -m32 -g -Wall -ansi -c -o task4.o task4.c

task3.o: task3.c
	gcc -m32 -g -Wall -ansi -c -o task3.o task3.c

task2.o: task2.c
	gcc -m32 -g -Wall -ansi -c -o task2.o task2.c

task1.o: task1.c
	gcc -m32 -g -Wall -ansi -c -o task1.o task1.c

LineParser.o: LineParser.c
	gcc -m32 -g -Wall -ansi -c -o LineParser.o LineParser.c

.PHONY: clean

clean: 
	rm -f *.o task1 task2 task3 task4
