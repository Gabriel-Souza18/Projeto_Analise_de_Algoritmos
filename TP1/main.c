#include "tabuleiro.h"
#include "io.h"         // Para lerEntrada e escreverSaida
#include "algoritmos.h" // Para buscaForcaBruta e buscaMovimentosMinimos
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int main(int argc, char **argv){
    clock_t start, end;
    double cpu_time_used;

    start = clock(); // Tempo inicial

    if (argc < 3 || strcmp(argv[1], "-i") != 0){ // Verifica argumento "-i"
        fprintf(stderr, "Uso: %s -i <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    // Lê todo o conteúdo relevante do arquivo de entrada
    char *entrada_completa = lerEntrada(argv[2]);
    if (!entrada_completa){
        // lerEntrada já deve ter impresso o erro
        return 1;
    }

    char *ptr = entrada_completa; // Ponteiro para a posição atual na string de entrada
    char buffer_saida[1024] = ""; // Buffer maior para acumular a saída
    int caso_num = 1;

  

    // Loop para processar cada caso de teste na string 'entrada_completa'
    while (*ptr != '\0') { // Continua enquanto não chegar ao fim da string

        // Pula espaços em branco ou linhas vazias no início
        while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        if (*ptr == '\0') break; // Chegou ao fim após pular espaços

        // Verifica a condição de parada "0 0" ANTES de chamar inicializaTabuleiro
        if (strncmp(ptr, "0 0", 3) == 0) {
             char *check_end = ptr + 3;
             while (*check_end != '\0' && isspace((unsigned char)*check_end)) {
                 check_end++;
             }
             if (*check_end == '\0') {
                 break; // É o final "0 0"
             }
             // Se não for o fim real, trata como fim por segurança.
             if (strncmp(ptr, "0 0", 3) == 0) break;
        }


        // Inicializa o tabuleiro. A função avança o ponteiro 'ptr'.
        Tabuleiro *tabuleiro = inicializaTabuleiro(&ptr);

        if (tabuleiro) {
            // --- Execute algoritmos ---
        
            //char* resultado= buscaForcaBruta(tabuleiro);
            char* resultado= buscaMovimentosMinimos(tabuleiro);


            strcat(buffer_saida, resultado); // Acumula o resultado

            liberaTabuleiro(tabuleiro); // Libera o tabuleiro atual APÓS usar N e M

        } else {
            fprintf(stderr, "Erro ao inicializar tabuleiro a partir da posição atual. Interrompendo.\n");
            break; // Sai do loop principal
        }
    }
    end = clock(); // Tempo final

    // Cálculo do tempo de CPU em milissegundos
    cpu_time_used = ((double) (end - start) * 1000.0) / CLOCKS_PER_SEC;

    printf("Tempo Total: %.3f ms\n", cpu_time_used);
    // Escreve a saída acumulada no arquivo "saida.txt" usando a função de io.c
    escreverSaida("saida.txt", buffer_saida);

    free(entrada_completa); // Libera a memória alocada por lerEntrada
    return 0;
}