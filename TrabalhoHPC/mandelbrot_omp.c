/*
 * Universidade do Vale do Rio dos Sinos - UNISINOS
 * Ciência da Computação
 * Computação de Alto Desempenho
 *
 * Trabalho: Geração do Conjunto de Mandelbrot
 * Implementação Paralela utilizando OpenMP
 * Feito por Bernardo Salvador e Victor Carpegiani
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

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

int main(int argc, char *argv[]) {

    /* Número padrão de threads */
    int threads = 4;

    /* Permite informar o número de threads pela linha de comando */
    if (argc > 1)
        threads = atoi(argv[1]);

    /* Configura a quantidade de threads do OpenMP */
    omp_set_num_threads(threads);

    /* Vetor auxiliar para armazenar os resultados dos pixels */
    int *image = (int *)malloc(WIDTH * HEIGHT * sizeof(int));

    if (image == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    /* Início da medição de tempo */
    double inicio = omp_get_wtime();

    /*
     * Paralelização do laço principal.
     * Cada thread calcula uma parte das linhas da imagem,
     * pois o cálculo de cada pixel é independente.
     */
    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            /* Mapeia o pixel para um ponto do plano complexo */
            double real = -2.0 + (3.0 * x / WIDTH);
            double imag = -1.5 + (3.0 * y / HEIGHT);

            /* Armazena o número de iterações calculado */
            image[y * WIDTH + x] = mandelbrot(real, imag);
        }
    }

    /* Cria o arquivo de saída */
    FILE *fp = fopen("mandelbrot_omp.ppm", "w");

    if (!fp) {
        printf("Erro ao criar arquivo.\n");
        free(image);
        return 1;
    }

    /* Cabeçalho do formato PPM */
    fprintf(fp, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

    /* Converte os valores calculados em cores */
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

    /* Fim da medição de tempo */
    double fim = omp_get_wtime();

    fclose(fp);
    free(image);

    printf("Imagem gerada: mandelbrot_omp.ppm\n");
    printf("Threads utilizadas: %d\n", threads);
    printf("Tempo de execucao: %.4f segundos\n", fim - inicio);

    return 0;
}
