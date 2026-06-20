#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>

/*SE QUISER FAZER MOSTRAR O TEMPO EM MS, TIRA A LINHA 18*/

#define START_STOPWATCH( prm ) {gettimeofday( &prm.mStartTime, 0);} 

#define STOP_STOPWATCH( prm ) {                                                        \
  gettimeofday( &prm.mEndTime, 0);                                                 \
  prm.mElapsedTime = (1000.0f * ( prm.mEndTime.tv_sec - prm.mStartTime.tv_sec) + (0.001f * (prm.mEndTime.tv_usec - prm.mStartTime.tv_usec)) );  \
  prm.mElapsedTime /= 1000.0f;                                                         \
}

typedef struct
{
  struct timeval mStartTime;
  struct timeval mEndTime;
  double mElapsedTime;
} STOPWATCH; 

#define EPSILON 1E-30
#define ALING 64

typedef struct {
    int n, m;
    double *vetor;
}MATRIZ;


size_t saveBin(MATRIZ *A, char *nomeArqv); 
size_t loadBin(MATRIZ *A, char *nomeArqv);

void multMatrizOt1(MATRIZ *  __restrict__ C,
                  MATRIZ *  __restrict__ A, 
                  MATRIZ *  __restrict__ B, 
                  unsigned int nThreads){
     #pragma omp parallel
     {
	#pragma omp for 
        for (int i = 0; i < C->n; i++){
            for (int k = 0; k A->m; k++){
                    int ak = i * A->m + k;
                    double numeroA = A->vetor[ak];

                for (int j = 0; j < C->m; j++){
                    int bk = k * B->m + i;
                    int ck = i * c->m + j;
                   
                        C->vetor[ck] += numeroA * B->vetor[bk];    
                }
            }
        }
      }
}


int main (int ac, char **av){

    MATRIZ A, B, C;
    STOPWATCH stopwatch;
    START_STOPWATCH(stopwatch);


    char  *nomeArqvA = av[1],
          *nomeArqvB = av[2],
          *nomeArqvC = av[3];

    unsigned int nThreads = atoi(av[4]);
    double contaBytes = 0.0,
           *elapsedtime = NULL;


    contaBytes  =  (double)loadBin(&A, nomeArqvA); 
    contaBytes  += (double) loadBin(&B, nomeArqvB); 
    C.m = A.n;
    C.n = B.m;

    C.vetor = (double *) aligned_alloc(ALING, C.m *  C.n * sizeof(double)); 
    bzero(C.vetor, C.m *  C.n * sizeof(double)); 


    elapsedtime = (double *) aligned_alloc(ALING, nThreads * sizeof(double)); 
    printf("\nMultiplicação de matrizes\n");
    printf("\t  Matriz A: %s \n", nomeArqvA);
    printf("\t  Matriz B: %s \n", nomeArqvB);
    printf("\t  Matriz C: %s \n", nomeArqvC);
    printf("\t   Threads: %u \n", nThreads);
    printf("\t   Memória: %lf MBytes \n", ((contaBytes) / 1048576));

    multiMatrizOt1 (&C, &A, &B, nThreads);
    saveBin(&C, nomeArqvC);

    printf("----------------------------------------------------------------\n");
    for (unsigned int i = 0; i < nThreads; i++){
        printf(" Tempo gasto pela thread [%.3u] foi de %15.8lf segundos\n", i, elapsedtime[i]);
    }

    free(A.vetor);
    free(B.vetor);
    free(C.vetor);


    STOP_STOPWATCH(stopwatch);
    printf("----------------------------------------------------------------\n");
    printf("\tTempo total de execução: %lf\n", stopwatch.mElapsedTime);
    printf("----------------------------------------------------------------\n");
    return EXIT_SUCCESS;
}

size_t saveBin(MATRIZ *A, char * filename){
    FILE *ptr = fopen(filename, "wb");
    size_t bytes_written = 0, aux;
    assert(ptr != NULL);
    aux = fwrite(&A->m, sizeof(A->m), 1, ptr); bytes_written += aux * sizeof(A->m);
    aux = fwrite(&A->n, sizeof(A->n), 1, ptr); bytes_written += aux * sizeof(A->n);
    aux = fwrite(A->vetor, sizeof(double), A->m * A->n, ptr); bytes_written += aux * sizeof(double);

    fclose(ptr);

}

size_t loadBin(MATRIZ *A, char *filename){
    FILE *ptr = fopen(filename, "rb");
    assert(ptr != NULL);
    double *a = NULL;
    size_t bytes_read = 0, aux;


    aux = fread(&A->m, sizeof(A->m), 1, ptr); bytes_read += aux * sizeof(A->m);
    aux = fread(&A->n, sizeof(A->n), 1, ptr); bytes_read += aux * sizeof(A->n);

    a = (double *) aligned_alloc(ALING,  A->m *  A->n * sizeof(double));
    assert(a != NULL);

    aux = fread(a, sizeof(double), A->m * A->n, ptr); bytes_read += aux * sizeof(double);

    A->vetor = a;
    fclose(ptr);
    return bytes_read;

}