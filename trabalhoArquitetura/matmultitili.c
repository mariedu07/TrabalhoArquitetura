#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Uso: %s <N> <num_threads>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    if (N <= 0 || num_threads <= 0) {
        printf("Erro: N e num_threads devem ser positivos\n");
        return 1;
    }

    omp_set_num_threads(num_threads);

    double *A = malloc(N * N * sizeof(double));
    double *B = malloc(N * N * sizeof(double));
    double *C = calloc(N * N, sizeof(double));

    if (!A || !B || !C) {
        printf("Erro de memoria\n");
        return 1;
    }

    // Inicialização
    for (int i = 0; i < N * N; i++) {
        A[i] = (double)(i % 100);
        B[i] = (double)((i + 1) % 100);
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // ---------------- MATMUL PARALELO ----------------
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {

            double soma = 0.0;

            for (int k = 0; k < N; k++) {
                soma += A[i * N + k] * B[k * N + j];
            }

            C[i * N + j] = soma;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double tempo = (end.tv_sec - start.tv_sec) +
                   (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("N = %d\n", N);
    printf("Threads = %d\n", num_threads);
    printf("Tempo de execucao: %.6f s\n", tempo);

    free(A);
    free(B);
    free(C);

    return 0;
}