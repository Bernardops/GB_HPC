/*
 * Universidade do Vale do Rio dos Sinos - UNISINOS
 * Ciência da Computação
 * Computação de Alto Desempenho
 *
 * Trabalho: Geração do Conjunto de Mandelbrot
 * Implementação Sequencial em C
 * Feito por Bernardo Salvador e Victor Carpegiani
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* Definições da imagem e do número máximo de iterações */
#define WIDTH 1920
#define HEIGHT 1080
#define MAX_ITER 1000

/*
 * Calcula o número de iterações para um ponto do plano complexo.
 * Recebe as coordenadas (real, imag) e aplica a equação:
 *      z(n+1) = z(n)^2 + c
 */
int mandelbrot(double real, double imag) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;

    /* Executa as iterações enquanto o ponto não divergir
       e o limite máximo de iterações não for atingido */
    while ((zr * zr + zi * zi <= 4.0) && (iter < MAX_ITER)) {

        /* Expansão da multiplicação de números complexos */
        double temp = zr * zr - zi * zi + real;
        zi = 2.0 * zr * zi + imag;
        zr = temp;

        iter++;
    }

    return iter;
}

int main() {

    /* Cria o arquivo de saída da imagem */
    FILE *fp = fopen("mandelbrot_seq.ppm", "w");

    if (!fp) {
        printf("Erro ao criar arquivo.\n");
        return 1;
    }

    /* Cabeçalho do formato PPM */
    fprintf(fp, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    /* Início da medição de tempo */
    clock_t inicio = clock();

    /* Percorre todos os pixels da imagem */
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            /* Mapeia o pixel para um ponto do plano complexo */
            double real = -2.0 + (3.0 * x / WIDTH);
            double imag = -1.5 + (3.0 * y / HEIGHT);

            /* Calcula o número de iterações para o ponto */
            int iter = mandelbrot(real, imag);

            /* Define a cor do pixel de acordo com o número de iterações */
            int color = (int)(255.0 * iter / MAX_ITER);

            fprintf(fp, "%d %d %d ",
                    color,
                    color / 2,
                    255 - color);
        }
        fprintf(fp, "\n");
    }

    /* Fim da medição de tempo */
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    fclose(fp);

    printf("Imagem gerada: mandelbrot_seq.ppm\n");
    printf("Tempo de execucao: %.4f segundos\n", tempo);

    return 0;
}
