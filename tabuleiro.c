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

