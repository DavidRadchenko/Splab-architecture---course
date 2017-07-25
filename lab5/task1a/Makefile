all: do

do: main.o LineParser.o
	gcc -g -m32 -Wall -o do main.o LineParser.o

main.o: main.c
	gcc -m32 -g -Wall -ansi -c -o main.o main.c

LineParser.o: LineParser.c
	gcc -m32 -g -Wall -ansi -c -o LineParser.o LineParser.c

.PHONY: clean

clean: 
	rm -f *.o do