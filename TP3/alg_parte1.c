#include "alg_parte1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define TAMANHO_ALFABETO 256

// Função auxiliar para encontrar o mínimo de três números
static int minimoDeTres(int a, int b, int c) {
    if (a < b) {
        return (a < c) ? a : c;
    }
    return (b < c) ? b : c;
}

// Algoritmo de Programação Dinâmica para Casamento Aproximado
ResultadoParte1 ProgramacaoDinamica(char *texto, int tam_texto, char *padrao, int tam_padrao, int k_erros) {
    ResultadoParte1 resultado = {0};
    clock_t inicio = clock();

    int *coluna_anterior = (int *)malloc((tam_padrao + 1) * sizeof(int));
    int *coluna_atual = (int *)malloc((tam_padrao + 1) * sizeof(int));
    if (!coluna_anterior || !coluna_atual) {
        perror("Falha na alocação de memória para a PD");
        exit(1);
    }
    
    char *string_ocorrencias = (char *)malloc(MAX_LINHA_SAIDA);
    if (!string_ocorrencias) {
        perror("Falha na alocação de memória para string_ocorrencias");
        exit(1);
    }
    sprintf(string_ocorrencias, "%s", padrao);
    
    // Inicialização da primeira coluna
    for (int i = 0; i <= tam_padrao; i++) {
        coluna_anterior[i] = i;
    }

    for (int j = 1; j <= tam_texto; j++) {
        coluna_atual[0] = 0; 
        for (int i = 1; i <= tam_padrao; i++) {
            resultado.num_comparacoes++;
            int custo = (texto[j - 1] == padrao[i - 1]) ? 0 : 1;
            coluna_atual[i] = minimoDeTres(
                coluna_atual[i - 1] + 1,      // Inserção
                coluna_anterior[i] + 1,        // Deleção
                coluna_anterior[i - 1] + custo // Substituição/Match
            );
        }

        // Verifica se encontrou uma ocorrência válida
        if (coluna_atual[tam_padrao] <= k_erros && j >= tam_padrao) {
            char temp[32];
                int posicao = j - tam_padrao + 1;
                if (posicao > 0) {  // Filtra posições negativas
                    sprintf(temp, " %d", posicao);
                    strcat(string_ocorrencias, temp);
                }
        }

        // Troca as colunas para a próxima iteração
        int *ponteiro_temp = coluna_anterior;
        coluna_anterior = coluna_atual;
        coluna_atual = ponteiro_temp;
    }

    free(coluna_anterior);
    free(coluna_atual);
    
    strcat(string_ocorrencias, "\n");
    resultado.saida_ocorrencias = string_ocorrencias;
    resultado.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;
    return resultado;
}

// Algoritmo Shift-And para Casamento Aproximado
ResultadoParte1 ShiftAnd(char *texto, int tam_texto, char *padrao, int tam_padrao, int k_erros) {
    if (tam_padrao > (sizeof(unsigned long) * CHAR_BIT)) {
         ResultadoParte1 resultado = {0};
         resultado.saida_ocorrencias = (char*)malloc(100);
         sprintf(resultado.saida_ocorrencias, "%s ERRO: Padrão muito longo para ShiftAnd (max %zu bits)\n", 
                 padrao, sizeof(unsigned long) * CHAR_BIT);
         return resultado;
    }

    ResultadoParte1 resultado = {0};
    clock_t inicio = clock();

    // Inicializa o vetor de caracteres B
    unsigned long B[TAMANHO_ALFABETO] = {0};
    for (int i = 0; i < tam_padrao; i++) {
        B[(unsigned char)padrao[i]] |= (1UL << i);
    }

    char *string_ocorrencias = (char *)malloc(MAX_LINHA_SAIDA);
    if (!string_ocorrencias) {
        perror("Falha na alocação de memória para string_ocorrencias");
        exit(1);
    }
    sprintf(string_ocorrencias, "%s", padrao);

    // Casamento exato (k_erros = 0)
    if (k_erros == 0) {
        unsigned long R = 0;
        unsigned long mascara_final = 1UL << (tam_padrao - 1);
        
        for (int j = 0; j < tam_texto; j++) {
            resultado.num_comparacoes++;
            R = ((R << 1) | 1) & B[(unsigned char)texto[j]];
            
     
            if ((R & mascara_final) && (j >= tam_padrao - 1)) {
                char temp[32];
                int posicao = j - tam_padrao + 2;
                if (posicao > 0) {  // Filtra posições negativas
                    sprintf(temp, " %d", posicao);
                    strcat(string_ocorrencias, temp);}
                }
        }
    } 
    // Casamento aproximado (k_erros > 0)
    else {
        unsigned long *R = (unsigned long *)malloc((k_erros + 1) * sizeof(unsigned long));
        if (!R) {
            perror("Falha na alocação de memória para R no ShiftAnd aproximado");
            exit(1);
        }

        // Inicializa R com todos os bits 1
        for (int i = 0; i <= k_erros; i++) {
            R[i] = ~0UL;
        }

        unsigned long mascara_final = 1UL << (tam_padrao - 1);

        for (int j = 0; j < tam_texto; j++) {
            resultado.num_comparacoes++;
            unsigned long R_antigo = R[0]; // Salva o estado anterior de R[0]

            // Atualiza R[0]
            R[0] = ((R[0] << 1) | 1) & B[(unsigned char)texto[j]];
            
            // Atualiza os estados para erros
            for (int i = 1; i <= k_erros; i++) {
                unsigned long temp = R[i];
                // Combina operações: substituição, deleção e inserção
                R[i] = ((R[i] << 1) | 1) & B[(unsigned char)texto[j]]; // Match/substituição
                R[i] |= R_antigo; // Deleção
                R[i] |= (R[i-1] << 1); // Inserção
                R_antigo = temp; // Atualiza para próxima iteração
            }

            // Verifica ocorrência
            if (R[k_erros] & mascara_final) {
                char temp[32];
                int posicao = j - tam_padrao + 2;
                if (posicao > 0) {  // Filtra posições negativas
                    sprintf(temp, " %d", posicao);
                    strcat(string_ocorrencias, temp);
                }
            }
        }
        free(R);
    }

    strcat(string_ocorrencias, "\n");
    resultado.saida_ocorrencias = string_ocorrencias;
    resultado.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;
    return resultado;
}