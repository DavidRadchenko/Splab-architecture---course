all: encoder

# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
encoder:  encoder.o
	gcc -g -m32 -Wall -o encoder encoder.o

encoder.o: encoder.c
	gcc -m32 -g -Wall -ansi -c -o encoder.o encoder.c 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o encoder