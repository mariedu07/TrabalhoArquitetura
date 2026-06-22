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

#define START_STOPWATCH( prm ) {gettimeofday( &prm.mStartTime, 0);} //relogio comeca a contar

#define STOP_STOPWATCH( prm ) {                                                        \
  gettimeofday( &prm.mEndTime, 0); /*pega o horario exato q parou*/                                                    \
  prm.mElapsedTime = (1000.0f * ( prm.mEndTime.tv_sec - prm.mStartTime.tv_sec) + (0.001f * (prm.mEndTime.tv_usec - prm.mStartTime.tv_usec)) );  \
  prm.mElapsedTime /= 1000.0f;                                                         \
}// relogio para de contar e ve quanto tempo se passou

typedef struct
{
  struct timeval mStartTime;
  struct timeval mEndTime;
  double mElapsedTime;
} STOPWATCH; //estrutura do cronometro

#define EPSILON 1E-30
#define ALING 64

typedef struct {
    int linhas, colunas;
    double *vetor;
}MATRIZ;

//ASSINATURAS DAS FUNCOES

size_t saveBin(MATRIZ *A, char *nomeArqv); //grava vet e matrizes no formato bin, *A estrutura de matriz que vai ser gravada em disco, nomeArqv nome do arquivobinario e return qtd bytes lidos

size_t loadBin(MATRIZ *A, char *nomeArqv); //size-t é um tipo especifico para tamanhos em c

void multMatriz(MATRIZ *  __restrict__ C, //endereco da memoria da matriz resultado
                  MATRIZ *  __restrict__ A, //endereco da memoria da matriz A
                  MATRIZ *  __restrict__ B, //endereco da memoria da matriz B
                  unsigned int nThreads){ // quantas threads vao ser usadas na multiplicacao

     #pragma omp parallel // avisa ao compilador que o bloco dele deve ser executado de forma paralela
     {
	#pragma omp for //abre o bloco
        for (int j = 0; j < C->linhas; j++){ 
            for (int i = 0; i < C->colunas; i++){ 
                double soma = 0.0f; //vai acumular a soma e salvar no elemento de C
                for (int indice = 0; indice < A->colunas; indice++){ 
                    int elementoA = j * A->colunas + indice; //qual elemento vai multiplicar pelo elemento de B
                    int elementoB = (j * A->colunas + indice) * B->colunas + i; //qual elemento vai multiplicar pelo elemento de A
                    soma += A->vetor[elementoA] * B->vetor[elementoB]; //A*B que dps vai somar com os outros A*B
                }

                int elemento = j * C->colunas +  i;
                C->vetor[elemento] = soma; //salva em c
            }

        }
      }
}

// ./multi-mat-cpu.exec r2000x2000.bin i2000x2000.bin resultado.bin 4
int main (int ac, char **av){

    MATRIZ A, B, C;
    STOPWATCH stopwatch;
    START_STOPWATCH(stopwatch);


    char  *nomeArqvA = av[1],
          *nomeArqvB = av[2],
          *nomeArqvC = av[3];

    unsigned int nThreads = atoi(av[4]);
    double contaBytes = 0.0,
           *elapsedtime = NULL; //vetor para armazenar o tempo gasto por cada thread


    contaBytes  =  (double)loadBin(&A, nomeArqvA); //ve quantos bytes tem em a
    contaBytes  += (double) loadBin(&B, nomeArqvB); // ve quantos bytes tem em a e b
    C.colunas = A.linhas;
    C.linhas = B.colunas;

    C.vetor = (double *) aligned_alloc(ALING, C.colunas *  C.linhas * sizeof(double)); // aloca a memoria para a matriz resultado, o tamanho é m*n e cada elemento é um double, o alinhamento é de 64 bytes
    contaBytes  += (double) C.colunas *  C.linhas * sizeof(double); // ve quantos bytes tem em a, b e c
    bzero(C.vetor, C.colunas *  C.linhas * sizeof(double)); // limpa o espaco q acabamos de alocar


    elapsedtime = (double *) aligned_alloc(ALING, nThreads * sizeof(double)); //aloca a memoria para o vetor de tempo gasto por cada thread
    printf("\nMultiplicação de matrizes\n");
    printf("\t  Matriz A: %s \n", nomeArqvA);
    printf("\t  Matriz B: %s \n", nomeArqvB);
    printf("\t  Matriz C: %s \n", nomeArqvC);
    printf("\t   Threads: %u \n", nThreads);
    printf("\t   Memória: %lf MBytes \n", ((contaBytes) / 1048576));

    multMatriz(&C, &A, &B, nThreads);
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
    aux = fwrite(&A->colunas, sizeof(A->colunas), 1, ptr); bytes_written += aux * sizeof(A->colunas);
    aux = fwrite(&A->linhas, sizeof(A->linhas), 1, ptr); bytes_written += aux * sizeof(A->linhas);
    aux = fwrite(A->vetor, sizeof(double), A->colunas * A->linhas, ptr); bytes_written += aux * sizeof(double);

    fclose(ptr);

}

size_t loadBin(MATRIZ *A, char *filename){
    FILE *ptr = fopen(filename, "rb");
    assert(ptr != NULL);
    double *a = NULL;
    size_t bytes_read = 0, aux;
    //numread = fread( list, sizeof( char ), 25, stream );

    aux = fread(&A->colunas, sizeof(A->colunas), 1, ptr); bytes_read += aux * sizeof(A->colunas);
    aux = fread(&A->linhas, sizeof(A->linhas), 1, ptr); bytes_read += aux * sizeof(A->linhas);

    a = (double *) aligned_alloc(ALING,  A->colunas *  A->linhas * sizeof(double));
    assert(a != NULL);

    aux = fread(a, sizeof(double), A->colunas * A->linhas, ptr); bytes_read += aux * sizeof(double);

    A->vetor = a;
    fclose(ptr);
    return bytes_read;
    //print2Console(A);
}
