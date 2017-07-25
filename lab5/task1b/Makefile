all: do

do: task1b.o LineParser.o
	gcc -g -m32 -Wall -o do task1b.o LineParser.o

task1b.o: task1b.c
	gcc -m32 -g -Wall -ansi -c -o task1b.o task1b.c

LineParser.o: LineParser.c
	gcc -m32 -g -Wall -ansi -c -o LineParser.o LineParser.c

.PHONY: clean

clean: 
	rm -f *.o do
