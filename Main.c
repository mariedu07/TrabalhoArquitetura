#include<stdio.h>
#include<stdlib.h>
#include<time.h>

float* gerarMatriz(int tamMatriz){
    float *Matriz = malloc(tamMatriz * tamMatriz * sizeof(float));

    if (Matriz == NULL) {
        printf("Erro de alocacao\n");
        return NULL;
    }

    for(int i = 0; i < tamMatriz; i++){
        for(int j = 0; j < tamMatriz; j++){
            Matriz[i * tamMatriz + j] = rand() %10;
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

/// ---------------------------------------------------------------------------------- ///
/// ---------------------------------------------------------------------------------- ///
/// ---------------------------------------------------------------------------------- ///

float* multiplicacaoMatriz(float* matrizA, float* matrizB, int tamMatriz){

    float* resultado = malloc(tamMatriz * tamMatriz * sizeof(float));

    for (int i = 0; i<tamMatriz; i++){
        for(int j = 0; j<tamMatriz; j++){
            resultado[i*tamMatriz + j] = 0.0f;

            for(int k = 0; k < tamMatriz; k++){
                resultado[i*tamMatriz+j] += matrizA[i*tamMatriz + k] * matrizB[k*tamMatriz + j];
            }
        }
    }
    return resultado;
}

/// ---------------------------------------------------------------------------------- ///
/// ---------------------------------------------------------------------------------- ///
/// ---------------------------------------------------------------------------------- ///

float* transporMatriz(float *Matriz, int tamMatriz){
    float *Transposta = malloc(tamMatriz * tamMatriz * sizeof(float));

    for(int i = 0; i < tamMatriz; i++){
        for(int j = 0; j < tamMatriz; j++){
            Transposta[i * tamMatriz + j] = Matriz[j * tamMatriz + i];
        }
    }
    free(Matriz);
    return Transposta;

}

float* otimizacao1(float *matrizA, float *matrizB, int tamMatriz){ //aproveita a cache pq ao inves de varrer a linha i de a e a coluna j de b, varre a linha i de a e a coluna k de b, fixa uma linha de b e vai andando os elementos, ou seja, aproveita a cache

    float* transB = transporMatriz(matrizB, tamMatriz);
    float* matrizC = malloc(tamMatriz * tamMatriz * sizeof(float));
    zerarMatriz(matrizC, tamMatriz);

    for (int i = 0; i<tamMatriz; i++){
        for(int j = 0; j<tamMatriz;j++){
            float soma = 0.0;
            
            for(int k = 0; k < tamMatriz; k++){ 
                soma += matrizA[i * tamMatriz + k] * transB[j * tamMatriz + k];
            }

            matrizC[i * tamMatriz + j] = soma;
        }
    }
    free(transB);
    return matrizC;
}

/// ---------------------------------------------------------------------------------- ///
/// ---------------------------------------------------------------------------------- ///
/// ---------------------------------------------------------------------------------- ///

float* otimizacao1(float *matrizA, float *matrizB, int tamMatriz){ //aproveita a cache pq ao inves de varrer a linha i de a e a coluna j de b, varre a linha i de a e a coluna k de b, fixa uma linha de b e vai andando os elementos, ou seja, aproveita a cache

    float* resultado = malloc(tamMatriz * tamMatriz * sizeof(float));
    
}

int main(){

    srandom(time(NULL)); //pra aleatorizar os numeros

    int tamMatriz;
    float *matrizA, *matrizB, *matrizC;
    printf("Ordem das matrizes: ");
    
    scanf("%d",&tamMatriz);
    matrizA = gerarMatriz(tamMatriz);
    matrizB = gerarMatriz(tamMatriz);

    printf("Matriz A gerada!\n");
    imprimirMatriz(matrizA, tamMatriz);

    printf("\n\nMatriz B gerada!\n");
    imprimirMatriz(matrizB, tamMatriz);

    matrizC = otimizacao1(matrizA, matrizB, tamMatriz);

    printf("\n\nMatriz C gerada!\n");
    imprimirMatriz(matrizC, tamMatriz);

    free(matrizA);
    free(matrizB);
    free(matrizC);

}


//otimizacao omp