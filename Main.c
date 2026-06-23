#include<stdio.h>
#include<stdlib.h>

float* gerarMatriz(int tamMatriz){
    float *Matriz = malloc(tamMatriz * tamMatriz * sizeof(float));

    if (Matriz == NULL) {
        printf("Erro de alocacao\n");
        return NULL;
    }

    for(int i = 0; i < tamMatriz; i++){
        for(int j = 0; j < tamMatriz; j++){
            Matriz[i * tamMatriz + j] = rand() % 10;
        }
    }

    return Matriz;
}

void imprimirMatriz(float *Matriz, int tamMatriz){
    for(int i = 0; i < tamMatriz; i++){
        for(int j = 0; j < tamMatriz; j++){
            printf("%.2f ", Matriz[i * tamMatriz + j]);
        }
        printf("\n");
    }
}

void zerarMatriz(float *Matriz, int tamMatriz){
    for(int i = 0; i < tamMatriz; i++)
        for(int j = 0; j<tamMatriz; j++)
             Matriz[i * tamMatriz +j] = 0.0f;
}

void multiplicacaoMatriz(float *matrizA, float *matrizB, float *matrizC, int tamMatriz){
    for (int i = 0; i<tamMatriz; i++){
        for(int j = 0; j<tamMatriz; j++){
            matrizC[i*tamMatriz + j] = 0.0f;

            for(int k = 0; k < tamMatriz; k++){
                matrizC[i*tamMatriz+j] += matrizA[i*tamMatriz + k] * matrizB[k*tamMatriz + j];
            }
        }
    }
}

int main(){
    int tamMatriz;
    float *matrizA, *matrizB;
    printf("Ordem das matrizes: ");
    
    scanf("%d",&tamMatriz);
    matrizA = gerarMatriz(tamMatriz);
    matrizB = gerarMatriz(tamMatriz);

    printf("Matriz A gerada!\n");
    imprimirMatriz(matrizA, tamMatriz);

    printf("\n\nMatriz B gerada!\n");
    imprimirMatriz(matrizB, tamMatriz);
}

//otimizacao transposta (recebe matriz A, matriz B transposta, e c q guarda, e tam matriz), faz um codigo pra transpor uma matriz
//otimizacao omp