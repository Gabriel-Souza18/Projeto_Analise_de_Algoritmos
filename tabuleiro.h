#ifndef TABULEIRO_H
#define TABULEIRO_H

typedef struct  tabuleiro {
    int N, M;
    int * estado;
} Tabuleiro;

Tabuleiro* inicializaTabuleiro(char** entrada);
void imprimeTabuleiro(Tabuleiro *tab);
void liberaTabuleiro(Tabuleiro *tab);
#endif 