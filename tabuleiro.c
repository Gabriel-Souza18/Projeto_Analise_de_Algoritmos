#include "tabuleiro.h"
#include <stdlib.h>
#include <stdio.h>


Tabuleiro* inicializaTabuleiro(char **entrada) {
    Tabuleiro* tabuleiro = malloc(sizeof(Tabuleiro));
    if (!tabuleiro) return NULL;

    sscanf(*entrada, "%d %d", &tabuleiro->N, &tabuleiro->M);
    while (**entrada != '\n') (*entrada)++;
    (*entrada)++;

    int total = (tabuleiro->N * tabuleiro->M +1) / 2;
    tabuleiro->estado = malloc(sizeof(int) * total);
    for (int i = 0; i < total; i++) {
        sscanf(*entrada, "%d", &tabuleiro->estado[i]);
        while (**entrada != ' ' && **entrada != '\0' && **entrada != '\n') (*entrada)++;
        if (**entrada == ' ') (*entrada)++;
    }
    while (**entrada != '\n' && **entrada != '\0') (*entrada)++;
      if(**entrada == '\n') (*entrada)++;
    return tabuleiro;
}

void liberaTabuleiro(Tabuleiro *tabuleiro) {
    if (tabuleiro) {
        free(tabuleiro->estado);
        free(tabuleiro);
    }
}
void imprimeTabuleiro(Tabuleiro *tabuleiro) {
    if (tabuleiro == NULL || tabuleiro->estado == NULL) {
        printf("Tabuleiro inválido!\n");
        return;
    }

    int total_casas = (tabuleiro->N * tabuleiro->M) / 2;
    int idx = 0; // índice para percorrer o array estado

    printf("\nTabuleiro %dx%d (casas jogáveis marcadas com [N]):\n", tabuleiro->N, tabuleiro->M);
    
    for (int i = 1; i <= tabuleiro->N; i++) {
        for (int j = 1; j <= tabuleiro->M; j++) {
            // Verifica se é uma casa jogável (escura)
            if ((i + j) % 2 == 0) {
                // Imprime o valor da casa jogável com formatação
                printf("[%d]", tabuleiro->estado[idx]);
                idx++;
            } else {
                // Casa não jogável (clara)
                printf(" . ");
            }
        }
        printf("\n");
    }
}
    