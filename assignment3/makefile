all: clean ass3 

ass3: ass3.o scheduler.o printer.o coroutines.o cell.o
		gcc -g -m32 -o ass3 ass3.o scheduler.o printer.o coroutines.o cell.o

ass3.o: ass3.s
	nasm -g -f elf -o ass3.o ass3.s

scheduler.o: scheduler.s
	nasm -g -f elf -o scheduler.o scheduler.s

printer.o: printer.s
	nasm -g -f elf -o printer.o printer.s

coroutines.o: coroutines.s
	nasm -g -f elf -o coroutines.o coroutines.s

cell.o: cell.c
	gcc -g -Wall -m32 -ansi -c -o cell.o cell.c 

clean:
	rm -f *.o ass3

run: clean all
	rm -f *.o ./ass3 -d *.txt 1 2 3 5 5

