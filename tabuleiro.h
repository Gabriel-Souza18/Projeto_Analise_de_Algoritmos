#ifndef TABULEIRO_H
#define TABULEIRO_H

typedef struct  tabuleiro {
    int N;
    int M;
    int * estado; // Tabuleiro 8x8
} Tabuleiro;

Tabuleiro* inicializaTabuleiro(char** entrada);
void imprimeTabuleiro(Tabuleiro *tab);
void liberaTabuleiro(Tabuleiro *tab);
#endif 