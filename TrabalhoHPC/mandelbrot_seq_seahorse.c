/*
 * Universidade do Vale do Rio dos Sinos - UNISINOS
 * Ciência da Computação
 * Computação de Alto Desempenho
 *
 * Trabalho: Geração do Conjunto de Mandelbrot
 * Implementação Sequencial (Região Seahorse Valley)
 * Feito por Bernardo Salvador e Victor Carpegiani
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* Configurações da imagem */
#define WIDTH 3840
#define HEIGHT 2160
#define MAX_ITER 2000

/*
 * Calcula o número de iterações para um ponto do plano complexo
 * utilizando a equação do conjunto de Mandelbrot:
 *
 *      z(n+1) = z(n)^2 + c
 *
 * onde z0 = 0.
 */
int mandelbrot(double real, double imag) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;

    /* Executa as iterações até que o ponto diverja
       ou o limite máximo seja atingido. */
    while ((zr * zr + zi * zi <= 4.0) &&
           (iter < MAX_ITER)) {

        /* Expansão da multiplicação de números complexos */
        double temp = zr * zr - zi * zi + real;
        zi = 2.0 * zr * zi + imag;
        zr = temp;

        iter++;
    }

    return iter;
}

int main() {

    /* Cria o arquivo de saída no formato PPM */
    FILE *fp = fopen("mandelbrot_seq_seahorse.ppm", "w");

    if (!fp) {
        printf("Erro ao criar arquivo.\n");
        return 1;
    }

    /* Cabeçalho do arquivo PPM */
    fprintf(fp, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    /* Início da medição do tempo de execução */
    clock_t inicio = clock();

    /* Percorre todos os pixels da imagem */
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            /* Limites da região Seahorse Valley */
            double xmin = -0.8;
            double xmax = -0.7;
            double ymin = 0.05;
            double ymax = 0.15;

            /* Converte o pixel para um ponto do plano complexo */
            double real = xmin + (xmax - xmin) * x / WIDTH;
            double imag = ymin + (ymax - ymin) * y / HEIGHT;

            /* Calcula o número de iterações do ponto */
            int iter = mandelbrot(real, imag);

            /* Define a cor do pixel proporcionalmente
               ao número de iterações */
            int color = (int)(255.0 * iter / MAX_ITER);

            fprintf(fp, "%d %d %d ",
                    color,
                    color / 2,
                    255 - color);
        }
        fprintf(fp, "\n");
    }

    /* Fim da medição do tempo */
    clock_t fim = clock();
    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    fclose(fp);

    printf("Imagem gerada: mandelbrot_seq_seahorse.ppm\n");
    printf("Tempo de execucao: %.4f segundos\n", tempo);

    return 0;
}
