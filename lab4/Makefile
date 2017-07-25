all: flame

flame: start.o flame.o util.o
	ld -m elf_i386 start.o flame.o util.o -o flame

start.o: start.s
	nasm -f elf start.s -o start.o

flame.o: flame.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector flame.c -o flame.o

util.o: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o	

.PHONY: clean

clean: 
	rm -f *.o flame
