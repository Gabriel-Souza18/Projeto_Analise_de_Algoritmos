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
        printf("Uso correto: %s -i entrada.txt\n", argv[0]);
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

        // Escolha aqui qual função usar
        char *resultado = forcaBruta(tab);           // ou minMovimentos(tab)
        strcat(saida, resultado);
        free(resultado);

        liberaTabuleiro(tab);
    }

    gettimeofday(&fim, NULL);
    getrusage(RUSAGE_SELF, &uso);

    escreverSaida("saida.txt", saida);

    long tempoUsuario = uso.ru_utime.tv_sec * 1000000 + uso.ru_utime.tv_usec;
    long tempoSistema = uso.ru_stime.tv_sec * 1000000 + uso.ru_stime.tv_usec;

    printf("Tempo de usuário: %ld us\n", tempoUsuario);
    printf("Tempo de sistema: %ld us\n", tempoSistema);

    free(entrada);
    return 0;
}
