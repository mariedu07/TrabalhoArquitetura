#include<stdio.h>
#include<string.h>

#include<stdlib.h>
#include<assert.h>

#define ALINHAR 64

typedef struct{
    int m; //linhas
    int n; //colunas
    double* vetor; //ponteiro para os elementos da matriz
} MATRIZ;


void imprime(){
    printf("Exemplo: ./geradorMat 10 10 1 r10x10.bin \n oq vem depois do nome do exec é, respectivamente, linhas, colunas, indica matriz randomica (1) ou identidade(0), onde a matriz vai ser guardada(passamos o nome do arquivo que vai ser criado)");
}

void criarIdentidade(MATRIZ *A) {
    int i, j;

    //navegar por toda a matriz
    for (i = 0; i < A->m; i++) {
        for (j = 0; j < A->n; j++) {
            
            // A conta para achar a posição correta no vetor
            int posicao = i * A->n + j;

            // Se for vdd estamos na diagonal principal, ai bota 1
            if (i == j) {
                A->vetor[posicao] = 1.0;
            } else {
                A->vetor[posicao] = 0.0; 
            }
        }
    }
}

void criarRandomica(MATRIZ *A) {
    int i, j;

    // Dois laços "for" para passar por todas as linhas e colunas
    for (i = 0; i < A->m; i++) {
        for (j = 0; j < A->n; j++) {
            
            // A mesma conta para achar a posição no vetor único
            int posicao = i * A->n + j;

            // Gerando um número aleatório double entre 0.0 e 100.0
            double numeroAleatorio = ((double)rand() / (double)RAND_MAX) * 100.0;

            // Guarda o número na posição atual da matriz
            A->vetor[posicao] = numeroAleatorio;
        }
    }
}

void printNoArqv(MATRIZ *A, const char *nomeArqv) {
    // 1. Abre o arquivo no modo "wb"
    FILE *arquivo = fopen(nomeArqv, "wb");
    
    // verifica se criou msm
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArqv);
        return; 
    }
    
    // grava primeiro as dimensões da matriz pra ajudaa o proximo programa a saber o tamanho da matriz que vai ler
    fwrite(&(A->m), sizeof(int), 1, arquivo);
    fwrite(&(A->n), sizeof(int), 1, arquivo);
    
    // grava todos os numeros do vetor de uma vez so
    int totalElementos = A->m * A->n;
    fwrite(A->vetor, sizeof(double), totalElementos, arquivo);
    
    fclose(arquivo);
    
    printf("Matriz salva com sucesso em '%s'!\n", nomeArqv);
}

int main(int ac, char **av){ //ac quantos argumentos foram passados, av vetor de argumentos

    MATRIZ A;
    
    if(ac!=5){ //obriga a ter 5 parametros
        imprime();
        return 0;
    }

    A.m = atoi(av[1]);
    A.n = atoi(av[2]);
    int matTipo = atoi(av[3]);
    char* nomeArqv = av[4];
    //recebi os dados da matriz (os parametros)

    char sTipoMat[11]; //identidade ou randomica

    switch (matTipo){
        case 0:

        sTipoMat[0] = 'I';
        sTipoMat[1] = 'E';
        sTipoMat[2] = 'N';
        sTipoMat[3] = 'T';
        sTipoMat[4] = 'I';
        sTipoMat[5] = 'D';
        sTipoMat[6] = 'A';
        sTipoMat[7] = 'D';
        sTipoMat[8] = 'E';
        sTipoMat[9] = '\0';
        break;

        case 1:
            sTipoMat[0] = 'R';
            sTipoMat[1] = 'A';
            sTipoMat[2] = 'N';
            sTipoMat[3] = 'D';
            sTipoMat[4] = 'O';
            sTipoMat[5] = 'M';
            sTipoMat[6] = 'I';
            sTipoMat[7] = 'C';
            sTipoMat[8] = 'A';
            sTipoMat[9] = '\0';
            break;
    } //boto uma vetor dizendo se ela vai ser randomica ou identidade
    char *ponteiroSTipoMat = sTipoMat;

    printf("\n Gerando matriz... \nMatriz( %d x %d ) do tipo %s \n", A.m, A.n, ponteiroSTipoMat);
    printf("\n Nome do Arquivo: %s", nomeArqv);

    //assert() ele testa, se o conteudo dele for positivo nao faz nada, se for negativo, trava o programa, vamos usar pra saber se alocamos memoria ou nao
    //posix_memalign() é tipo o malloc, mas tambem alinha o que recebe, eh importante pra otimizacao por causa dos processadores que acabam sabendo exatamente onde ta cada coisa, ele recebe 3 parametros
    // (void**)&A.vetor finja que &A.vetor é um ponteiro generico

    assert(posix_memalign((void**)&A.vetor, ALINHAR, A.m * A.n * sizeof(double)) == 0);

    //memset, funcao para limpar ou preencher
    //memset(onde comeca, 0x00, tamanho total da matriz em bytes)
    //0x00 é o byte 0 em hexadecimal
    memset(A.vetor, 0x00,  A.m *  A.n * sizeof(double));

    switch (matTipo){
        case 0:
            criarIdentidade(&A);
            break;
        case 1:
            criarRandomica(&A);
            break;

    }

    free(A.vetor); //ja que salvamos a matriz no arquivo, nao precisamos mais dessa memoria

    printNoArqv(&A, nomeArqv);

    return 0;
}