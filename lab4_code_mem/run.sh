gcc -c main.c
gcc -c mem.c
gcc main.o mem.o -o main && ./main
rm -f *o && rm main
