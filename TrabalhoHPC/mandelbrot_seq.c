#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080
#define MAX_ITER 1000

int mandelbrot(double real, double imag) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;

    while ((zr * zr + zi * zi <= 4.0) && (iter < MAX_ITER)) {
        double temp = zr * zr - zi * zi + real;
        zi = 2.0 * zr * zi + imag;
        zr = temp;
        iter++;
    }

    return iter;
}

int main() {
    FILE *fp = fopen("mandelbrot_seq.ppm", "w");

    if (!fp) {
        printf("Erro ao criar arquivo.\n");
        return 1;
    }

    fprintf(fp, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    clock_t inicio = clock();

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            double real = -2.0 + (3.0 * x / WIDTH);
            double imag = -1.5 + (3.0 * y / HEIGHT);

            int iter = mandelbrot(real, imag);

            int color = (int)(255.0 * iter / MAX_ITER);

            fprintf(fp, "%d %d %d ",
                    color,
                    color / 2,
                    255 - color);
        }
        fprintf(fp, "\n");
    }

    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    fclose(fp);

    printf("Imagem gerada: mandelbrot_seq.ppm\n");
    printf("Tempo de execucao: %.4f segundos\n", tempo);

    return 0;
}
