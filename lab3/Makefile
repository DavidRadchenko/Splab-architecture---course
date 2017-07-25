all: do

do: task1c.o
	gcc -g -m32 -Wall -o do task1c.o

task1c.o: task1c.c
	gcc -m32 -g -Wall -ansi -c -o task1c.o task1c.c

.PHONY: clean

clean: 
	rm -f *.o do
