#include<stdio.h>

void gerarMatriz(float *Matriz, int tamMatriz){
    for(int i = 0; i < tamMatriz; i++){
        for(int j = 0; j < tamMatriz; j++){
            Matriz[i * tamMatriz + j] = rand() % 10; //%10
        }
    }
}

void zerarMatriz(float *Matriz, int tamMatriz){
    for(int i = 0; i < tamMatriz * tamMatriz; i++)
        Matriz[i] = 0.0f;
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

//otimizacao transposta (recebe matriz A, matriz B transposta, e c q guarda, e tam matriz), faz um codigo pra transpor uma matriz
