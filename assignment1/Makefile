#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: run

# Tool invocations
# Executable "run" depends on the files run.o and run.o.
run: main.o myasm.o
	gcc -m32 -g -Wall -o run main.o myasm.o

# Depends on the source and header files
main.o: main.c
	gcc -g -Wall -m32 -ansi -c -o main.o main.c 
 
myasm.o: myasm.s
	nasm -g -f elf -w+all -o myasm.o myasm.s


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o run