# Geração do Conjunto de Mandelbrot com OpenMP

Trabalho desenvolvido para a disciplina de **Computação de Alto Desempenho** do curso de **Ciência da Computação - UNISINOS**.

O projeto consiste na implementação do algoritmo de geração do **Conjunto de Mandelbrot** em linguagem C, contendo uma versão **sequencial** e uma versão **paralela utilizando OpenMP**, com o objetivo de analisar o desempenho e o ganho obtido através da paralelização.

## Estrutura do projeto

- `mandelbrot_seq.c` → Implementação sequencial.
- `mandelbrot_omp.c` → Implementação paralela utilizando OpenMP.
- `mandelbrot_seq_seahorse.c` → Versão sequencial configurada para a região *Seahorse Valley*, utilizada nos testes de desempenho.
- `mandelbrot_omp_seahorse.c` → Versão paralela da região *Seahorse Valley*, utilizada nos experimentos.

## Compilação

Versão sequencial:

```bash
gcc -O2 -Wall mandelbrot_seq.c -o seq -lm
```

Versão paralela:

```bash
gcc -O2 -Wall -fopenmp mandelbrot_omp.c -o omp -lm
```

## Execução

Executar a versão sequencial:

```bash
./seq
```

Executar a versão paralela (exemplo com 4 threads):

```bash
./omp 4
```

## Testes de desempenho

Os experimentos foram realizados utilizando diferentes quantidades de threads (1, 2, 4 e 8), comparando os tempos de execução da implementação sequencial e paralela. A análise foi baseada nas métricas de **speed-up** e **eficiência**, calculadas a partir da média de três execuções para cada configuração.

## Autor
Realizado por Bernardo Salvador e Victor Carpegiani.
Trabalho acadêmico desenvolvido para fins educacionais na disciplina de Computação de Alto Desempenho.
