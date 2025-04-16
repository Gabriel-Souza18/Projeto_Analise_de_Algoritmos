#include "tabuleiro.h"
#include <stdlib.h>
#include <stdio.h>


Tabuleiro* inicializaTabuleiro(char **entrada) {
    Tabuleiro* tab = malloc(sizeof(Tabuleiro));
    if (!tab) return NULL;

    sscanf(*entrada, "%d %d", &tab->N, &tab->M);
    while (**entrada != '\n') (*entrada)++;
    (*entrada)++;

    int total = (tab->N * tab->M) / 2;
    tab->estado = malloc(sizeof(int) * total);
    for (int i = 0; i < total; i++) {
        sscanf(*entrada, "%d", &tab->estado[i]);
        while (**entrada != ' ' && **entrada != '\0' && **entrada != '\n') (*entrada)++;
        if (**entrada == ' ') (*entrada)++;
    }

    return tab;
}

void liberaTabuleiro(Tabuleiro *tabuleiro) {
    if (tabuleiro) {
        free(tabuleiro->estado);
        free(tabuleiro);
    }
}

void imprimeTabuleiro(Tabuleiro *tab) {
    int k = 0;
    for (int i = 0; i < tab->N; i++) {
        for (int j = 0; j < tab->M; j++) {
            if ((i + j) % 2 == 1) {
                printf(". ");
            } else {
                printf("%d ", tab->estado[k++]);
            }
        }
        printf("\n");
    }
}
