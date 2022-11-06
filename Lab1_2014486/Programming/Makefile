all: factorial.o readline.o main.o
	gcc main.o factorial.o readline.o -o myfactorial
factorial.o: factorial.c factorial.h
	gcc -c factorial.c
readline.o: readline.c readline.h
	gcc -c readline.c
main.o: main.c factorial.h readline.h
	gcc -c main.c
clean:
	rm -f *.o myfactorial
