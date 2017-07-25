all: do

do: task2.o LineParser.o JobControl.o
	gcc -g -m32 -Wall -o do task2.o LineParser.o JobControl.o

task2.o: task2.c
	gcc -m32 -g -Wall -ansi -c -o task2.o task2.c

LineParser.o: LineParser.c
	gcc -m32 -g -Wall -ansi -c -o LineParser.o LineParser.c
	
JobControl.o: JobControl.c
	gcc -m32 -g -Wall -ansi -c -o JobControl.o JobControl.c

.PHONY: clean

clean: 
	rm -f *.o do
