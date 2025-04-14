#include "tabuleiro.h"
#include <stdlib.h>
#include <stdio.h>


Tabuleiro* inicializaTabuleiro(char* entrada){
    Tabuleiro* tabuleiro = (Tabuleiro*)malloc(sizeof(Tabuleiro));
    if (tabuleiro == NULL) {
        printf("Erro ao alocar memória para o tabuleiro");
        return NULL;
    }

    // Inicializa o tabuleiro com os valores de entrada
    sscanf(entrada, "%d %d", &(tabuleiro->N), &(tabuleiro->M));
    tabuleiro->estado = (int*)malloc(tabuleiro->N * tabuleiro->M * sizeof(int));
    if (tabuleiro->estado == NULL) {
        printf("Erro ao alocar memória para o estado do tabuleiro");
        free(tabuleiro);
        return NULL;
    }

    for (int i = 0; i < tabuleiro->N; i++) {
        for (int j = 0; j < tabuleiro->M; j++) {
            sscanf(entrada, "%d", &(tabuleiro->estado[i * tabuleiro->M + j]));
            entrada += 2; // Pular o número e o espaço
        }
    }

    return tabuleiro;
}
void liberaTabuleiro(Tabuleiro *tabuleiro){
    if (tabuleiro != NULL) {
        free(tabuleiro->estado);
        free(tabuleiro);
    }
}
void imprimeTabuleiro(Tabuleiro *tabuleiro){
    for (int i = 0; i < tabuleiro->N; i++) {
        for (int j = 0; j < tabuleiro->M; j++) {
            printf("%d ", tabuleiro->estado[i * tabuleiro->M + j]);
        }
        printf("\n");
    }
}