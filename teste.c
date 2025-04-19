#include "tabuleiro.h"
#include "io.h"
#include "algoritmos.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "-i") != 0) {
        printf("Uso: %s -i entrada.txt\n", argv[0]);
        return 1;
    }

    char *entrada = lerEntrada(argv[2]);
    if (!entrada) return 1;

    struct timeval inicio, fim;
    struct rusage uso;
    gettimeofday(&inicio, NULL);

    char *ptr = entrada;
    char saida[4096] = "";

    while (1) {
        int N, M;
        sscanf(ptr, "%d %d", &N, &M);
        if (N == 0 && M == 0) break;

        Tabuleiro *tab = inicializaTabuleiro(&ptr);
        char *resultado = forcaBruta(tab); // Ou minMovimentos(tab)
        strcat(saida, resultado);
        free(resultado);
        liberaTabuleiro(tab);
    }

    gettimeofday(&fim, NULL);
    getrusage(RUSAGE_SELF, &uso);

    escreverSaida("saida.txt", saida);

    printf("Tempo usuário: %ld us\n", uso.ru_utime.tv_sec * 1000000 + uso.ru_utime.tv_usec);
    printf("Tempo sistema: %ld us\n", uso.ru_stime.tv_sec * 1000000 + uso.ru_stime.tv_usec);

    free(entrada);
    return 0;
}