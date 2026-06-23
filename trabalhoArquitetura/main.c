#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<omp.h>

/**/
void gerarMatriz(float *Matriz, int tamMatriz){
    for(int i = 0; i < tamMatriz; i++){
        for(int j = 0; j < tamMatriz; j++){
            Matriz[i * tamMatriz + j] = rand() % 10;
        }
    }
}

void zerarMatriz(float *M, int N){
    for(int i = 0; i < N * N; i++)
        M[i] = 0.0f;
}

void multiplicacaoBase(float *matrizA, float *matrizB, float *matrizC, int tamMatriz){
    for(int i = 0; i<tamMatriz; i++){
        for(int j = 0; j<tamMatriz; j++){
            matrizC[i*tamMatriz + j] = 0.0f;
            for(int k=0; k<tamMatriz; k++){
                matrizC[i*tamMatriz+j] += matrizA[i*tamMatriz + k] * matrizB[k*tamMatriz + j];
            }
        }
    }
}

void transformaTransposta(float *matrizB, float *matrizBT, int tamMatriz){
    
    for (int i = 0; i < tamMatriz; i++) {
        for (int j = 0; j < tamMatriz; j++) {
            matrizBT[j * tamMatriz + i] = matrizB[i * tamMatriz + j];
        }
    }
}

void otimizacaoTransposta(float *matrizA, float *matrizTranspostaB, float *matrizC, int tamMatriz){
    for(int i = 0; i < tamMatriz; i++){
        for(int j = 0; j < tamMatriz; j++){
            float soma = 0.0f;

            for(int k = 0; k < tamMatriz; k++)
                soma += matrizA[i * tamMatriz + k] * matrizTranspostaB[j * tamMatriz + k];

            matrizC[i * tamMatriz + j] = soma;
        }
    }
}

void otimizacaoTiling(float *matrizA, float *matrizB, float *matrizC, int tamMatriz, int Bsize){
     for(int ii = 0; ii < tamMatriz; ii += Bsize){
        for(int jj = 0; jj < tamMatriz; jj += Bsize){
            for(int kk = 0; kk < tamMatriz; kk += Bsize){
                // percorre dentro do bloco
                for(int i = ii; i < ii + Bsize && i < tamMatriz; i++){
                    for(int j = jj; j < jj + Bsize && j < tamMatriz; j++){
                        float soma;
                        soma = matrizC[i * tamMatriz + j];

                        for(int k = kk; k < kk + Bsize && k < tamMatriz; k++){
                            soma += matrizA[i * tamMatriz + k] * matrizB[k * tamMatriz + j];
                        }

                        matrizC[i * tamMatriz + j] = soma;
                    }
                }
            }
        }
    }
}

void otimizacaoOmpBase(float*matrizA, float *matrizB, float *matrizC, int tamMatriz){
    //isso vai dizer pra usar o omp
    #pragma omp parallel for schedule(static)
    for(int i = 0; i<tamMatriz; i++){
        for(int j = 0; j<tamMatriz; j++){
            matrizC[i*tamMatriz + j] = 0.0f;
            for(int k=0; k<tamMatriz; k++){
                matrizC[i*tamMatriz+j] += matrizA[i*tamMatriz + k] * matrizB[k*tamMatriz + j];
            }
        }
    }
}

void otimzacaoOmpTransposta(float*matrizA, float *matrizTranspostaB, float *matrizC, int tamMatriz){
    //isso vai dizer pra usar o omp
    #pragma omp parallel for schedule(static)
    for(int i = 0; i < tamMatriz; i++){
        for(int j = 0; j < tamMatriz; j++){
            float soma = 0.0f;

            for(int k = 0; k < tamMatriz; k++)
                soma += matrizA[i * tamMatriz + k] * matrizTranspostaB[j * tamMatriz + k];

            matrizC[i * tamMatriz + j] = soma;
        }
    }
}

void otimizacaoOmpTiling(float*matrizA, float *matrizB, float *matrizC, int tamMatriz, int Bsize){
    //isso vai dizer pra usar o omp
    #pragma omp parallel for schedule(static)
    for(int ii = 0; ii < tamMatriz; ii += Bsize){
        for(int jj = 0; jj < tamMatriz; jj += Bsize){
            for(int kk = 0; kk < tamMatriz; kk += Bsize){
                // percorre dentro do bloco
                for(int i = ii; i < ii + Bsize && i < tamMatriz; i++){
                    for(int j = jj; j < jj + Bsize && j < tamMatriz; j++){
                        float soma;
                        soma = matrizC[i * tamMatriz + j];

                        for(int k = kk; k < kk + Bsize && k < tamMatriz; k++){
                            soma += matrizA[i * tamMatriz + k] * matrizB[k * tamMatriz + j];
                        }

                        matrizC[i * tamMatriz + j] = soma;
                    }
                }
            }
        }
    }
}

void execucao(int tamMatriz){
    
    
    printf("\nTamanho da matriz = %d x %d \n\n", tamMatriz, tamMatriz);

    float *matrizA = malloc(tamMatriz * tamMatriz * sizeof(float));
    float *matrizB = malloc(tamMatriz * tamMatriz * sizeof(float));
    float *matrizBT = malloc(tamMatriz * tamMatriz * sizeof(float));
    float *matrizC = malloc(tamMatriz * tamMatriz * sizeof(float));

    

    if(!matrizA || !matrizB || !matrizBT || !matrizC)
    {
        printf("Erro de memoria\n");
        free(matrizA); free(matrizB); free(matrizBT); free(matrizC);
        return;
    }

    zerarMatriz(matrizA, tamMatriz);
    zerarMatriz(matrizB, tamMatriz);
    zerarMatriz(matrizBT, tamMatriz);
    zerarMatriz(matrizC, tamMatriz);


    gerarMatriz(matrizA, tamMatriz);
    gerarMatriz(matrizB, tamMatriz);

    clock_t inicio, fim;
    double tempoBasico, tempoTransposta, tempoTiling, tempoOMP, tempoOMPTransposta, tempoOMPTiling;

    /*
    // multiplicacao basica
    inicio = clock();
    multiplicarBasico(matrizA, matrizB, matrizC, tamMatriz);
    fim = clock();

    tempoBasico = (double)(fim - inicio) / CLOCKS_PER_SEC;
    */
    
    // multiplicacao com otimizacao transposta
    transformaTransposta(matrizB, matrizBT, tamMatriz);

    inicio = clock();
    otimizacaoTransposta(matrizA, matrizBT, matrizC, tamMatriz);
    fim = clock();

    tempoTransposta = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("Baseline:   %.6f s\n", tempoBasico);
    printf("Transposta: %.6f s\n", tempoTransposta);


    /*
    // multiplicacao com otimizacao tiling
    int Bsize = 32; // pode testar 16, 32, 64

    inicio = clock();
    otimizacaoTiling(tamMatriz, matrizA, matrizB, matrizC, Bsize);
    fim = clock();

    tempoTiling = (double)(fim - inicio)/CLOCKS_PER_SEC;
    
    printf("Tiling: %.6f s\n", tempoTiling);

    // multiplicacao com otimizacao OpenMP
    inicio = clock();
    otimizacaoOmpBase(matrizA, matrizB, matrizC, tamMatriz);
    fim = clock();

    // multiplicacao com otimizacao OpenMP e Transposta
    inicio = clock();
    otimzacaoOmpTransposta(matrizA, matrizBT, matrizC, tamMatriz);
    fim = clock();

    tempoOMPTransposta = (double)(fim - inicio) / CLOCKS_PER_SEC;

    // multiplicacao com otimizacao OpenMP e Tiling
    int Bsize = 32; // pode testar 16, 32, 64

    inicio = clock();
    otimizacaoOmpTiling(tamMatriz, matrizA, matrizB, matrizC, Bsize);
    fim = clock();

    tempoOMPTiling = (double)(fim - inicio) / CLOCKS_PER_SEC;
    */

    free(matrizA);
    free(matrizB);
    free(matrizBT);
    free(matrizC);

} /* */

int main(){
    int tamMatriz;
    printf("Digite o tamanho da matriz: ");
    scanf("%d", &tamMatriz);
    execucao(tamMatriz);
    return 0;
}