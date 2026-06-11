#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

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

int main(int argc, char *argv[]) {

    int threads = 4;

    if (argc > 1)
        threads = atoi(argv[1]);

    omp_set_num_threads(threads);

    int *image = (int *)malloc(WIDTH * HEIGHT * sizeof(int));

    double inicio = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            double real = -2.0 + (3.0 * x / WIDTH);
            double imag = -1.5 + (3.0 * y / HEIGHT);

            image[y * WIDTH + x] = mandelbrot(real, imag);
        }
    }

    FILE *fp = fopen("mandelbrot_omp.ppm", "w");

    fprintf(fp, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            int iter = image[y * WIDTH + x];
            int color = (int)(255.0 * iter / MAX_ITER);

            fprintf(fp, "%d %d %d ",
                    color,
                    color / 2,
                    255 - color);
        }
        fprintf(fp, "\n");
    }

    double fim = omp_get_wtime();

    fclose(fp);
    free(image);

    printf("Imagem gerada: mandelbrot_omp.ppm\n");
    printf("Threads utilizadas: %d\n", threads);
    printf("Tempo de execucao: %.4f segundos\n", fim - inicio);

    return 0;
}
