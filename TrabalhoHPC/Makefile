CC = gcc
CFLAGS = -O2 -Wall
OMPFLAGS = -fopenmp

all:
	$(CC) $(CFLAGS) mandelbrot_seq.c -o seq -lm
	$(CC) $(CFLAGS) $(OMPFLAGS) mandelbrot_omp.c -o omp -lm

clean:
	rm -f seq omp *.ppm
