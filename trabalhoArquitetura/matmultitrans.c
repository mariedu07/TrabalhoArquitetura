#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLOCK 32   // tamanho do bloco (ponto importante da otimização)

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Uso: %s <N>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);

    double *A  = malloc(N * N * sizeof(double));
    double *B  = malloc(N * N * sizeof(double));
    double *BT = malloc(N * N * sizeof(double));
    double *C  = calloc(N * N, sizeof(double));

    if (!A || !B || !BT || !C) {
        printf("Erro de memoria\n");
        return 1;
    }

    // inicialização
    for (int i = 0; i < N * N; i++) {
        A[i] = (double)(i % 100);
        B[i] = (double)((i + 1) % 100);
    }

    // transposição de B
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            BT[j * N + i] = B[i * N + j];
        }
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // ---------------- TILING ----------------
    for (int ii = 0; ii < N; ii += BLOCK) {
        for (int jj = 0; jj < N; jj += BLOCK) {
            for (int kk = 0; kk < N; kk += BLOCK) {

                for (int i = ii; i < ii + BLOCK && i < N; i++) {
                    for (int j = jj; j < jj + BLOCK && j < N; j++) {

                        double soma = C[i * N + j];

                        for (int k = kk; k < kk + BLOCK && k < N; k++) {
                            soma += A[i * N + k] * BT[j * N + k];
                        }

                        C[i * N + j] = soma;
                    }
                }
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double tempo = (end.tv_sec - start.tv_sec) +
                   (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("N = %d\n", N);
    printf("Tempo de execucao: %.6f s\n", tempo);

    free(A);
    free(B);
    free(BT);
    free(C);

    return 0;
}