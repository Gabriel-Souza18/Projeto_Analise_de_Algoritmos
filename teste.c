#include "tabuleiro.h"
#include "io.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    char *entrada = lerEntrada("entrada.txt");
    if (!entrada) return 1;

    char *ptr = entrada;
    char saida[1024] = "";

    Tabuleiro **Tabuleiros = NULL;
    int numTabuleiros = 0;

    while (1) {
        int N, M;
        sscanf(ptr, "%d %d", &N, &M);
        if (N == 0 && M == 0) break;

        Tabuleiro *tab = inicializaTabuleiro(&ptr);
        imprimeTabuleiro(tab);

        strcat(saida, "0\n"); // valor fictício por enquanto
        liberaTabuleiro(tab);
    }

    escreverSaida("saida.txt", saida);
    free(entrada);
    return 0;
}
