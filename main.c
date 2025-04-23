#include "tabuleiro.h"
#include "io.h"
#include "algoritmos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char **argv){
    if (argc < 3 || argv[1][0] != '-' || argv[1][1] != 'i'){
        printf("Uso: %s -i arquivo_entrada.txt\n", argv[0]);
        return 1;
    }
    // lê o conteúdo do arquivo de entrada
    char *entrada = lerEntrada(argv[2]);
    if (!entrada){
        return 1;
    }
    char *ptr = entrada;
    char saida[4096] = "";
    // tempo de início da execução
    clock_t inicio = clock();
    // processa até encontrar "0 0"
    while (1){
        int N, M;
        if (sscanf(ptr, "%d %d", &N, &M) != 2 || (N == 0 && M == 0)){
            break;
        }
        // avança o ponteiro para o início da linha seguinte
        while (*ptr != '\n' && *ptr){
            ptr++;
        }
        if (*ptr == '\n'){
            ptr++;
        }
        // inicializa o tabuleiro a partir dos dados da entrada
        Tabuleiro *tab = inicializaTabuleiro(&ptr);
        // calcula o resultado utilizando a busca de movimentos mínimos
        char *res = buscaMovimentosMinimos(tab);
        strcat(saida, res);
        free(res);
        liberaTabuleiro(tab);
    }

    // tempo final e tempo total de execução
    clock_t fim = clock();
    escreverSaida("saida.txt", saida);
    // printa o tempo total
    printf("Tempo total: %.2f ms\n", (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC);
    free(entrada);
    return 0;
}