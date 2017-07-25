all: lab9 

lab9: task3.o
	ld -melf_i386 task3.o -o lab9

task3.o: task3.s
	nasm -f elf task3.s -o task3.o


clean:
	rm -f *.o lab9

