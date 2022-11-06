main.o: main.c hello.h
	gcc -c main.c
hello.o: hello.c hello.h
	gcc -c hello.c
all: main.o hello.o
	gcc main.o hello.o -o hello
clean: 
	rm -f *.o hello
