all: do

do: task1.o LineParser.o
	gcc -g -m32 -Wall -o do task1.o LineParser.o

task1.o: task1.c
	gcc -m32 -g -Wall -ansi -c -o task1.o task1.c

LineParser.o: LineParser.c
	gcc -m32 -g -Wall -ansi -c -o LineParser.o LineParser.c

.PHONY: clean

clean: 
	rm -f *.o do
