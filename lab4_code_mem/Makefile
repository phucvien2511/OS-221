all: main.o mem.o
	gcc main.o mem.o -o main
main.o: main.c mem.h
	gcc -c main.c
mem.o: mem.c mem.h
	gcc -c mem.c
run:
	./main
clean:
	rm *.o && rm main