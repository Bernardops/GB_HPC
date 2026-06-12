/*
 * Universidade do Vale do Rio dos Sinos - UNISINOS
 * Ciência da Computação
 * Computação de Alto Desempenho
 *
 * Trabalho: Geração do Conjunto de Mandelbrot
 * Implementação Paralela com OpenMP
 * (Região Seahorse Valley)
 * Feito por Bernardo Salvador e Victor Carpegiani
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

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

int main(int argc, char *argv[]) {

    /* Número padrão de threads */
    int threads = 4;

    /* Permite definir a quantidade de threads
       via linha de comando */
    if (argc > 1)
        threads = atoi(argv[1]);

    /* Configura o OpenMP */
    omp_set_num_threads(threads);

    /* Vetor utilizado para armazenar os resultados
       calculados para cada pixel */
    int *image = (int *)malloc(WIDTH * HEIGHT * sizeof(int));

    if (image == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    /* Início da medição do tempo */
    double inicio = omp_get_wtime();

    /*
     * Paralelização do laço principal.
     * Cada thread é responsável pelo cálculo de
     * uma parte das linhas da imagem.
     */
    #pragma omp parallel for schedule(dynamic)
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

            /* Armazena o número de iterações calculado */
            image[y * WIDTH + x] = mandelbrot(real, imag);
        }
    }

    /* Cria o arquivo de saída */
    FILE *fp = fopen("mandelbrot_omp_seahorse.ppm", "w");

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

    /* Fim da medição do tempo */
    double fim = omp_get_wtime();

    fclose(fp);
    free(image);

    printf("Imagem gerada: mandelbrot_omp_seahorse.ppm\n");
    printf("Threads utilizadas: %d\n", threads);
    printf("Tempo de execucao: %.4f segundos\n", fim - inicio);

    return 0;
}
