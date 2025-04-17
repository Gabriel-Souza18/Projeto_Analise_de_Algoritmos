#include "tabuleiro.h"
#include "io.h"
#include "algoritmos.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>



int main() {
    char *entrada = lerEntrada("entrada.txt");
    if (!entrada) return 1;

    char *ptr = entrada;
    char saida[2048] = "";

    Tabuleiro **Tabuleiros = NULL;
    int numTabuleiros = 0;

    while (1) {
        int N, M;
        sscanf(ptr, "%d %d", &N, &M);
        if (N == 0 && M == 0) break;

        Tabuleiro *tab = inicializaTabuleiro(&ptr);
        imprimeTabuleiro(tab);

        Tabuleiros = realloc(Tabuleiros, (numTabuleiros+1)*sizeof *Tabuleiros);
        Tabuleiros[numTabuleiros++] = tab;

        strcat(saida, "0\n"); // valor fictício por enquanto
        printf("%s", saida);
    }

    printf("%s", forcaBruta(Tabuleiros[0]));
    printf("%s", minMovimentos(Tabuleiros[0]));


    for (int i = 0; i < numTabuleiros; i++) {
        liberaTabuleiro(Tabuleiros[i]);
    }
    free(Tabuleiros);
    escreverSaida("saida.txt", saida);
    free(entrada);
    return 0;
}
