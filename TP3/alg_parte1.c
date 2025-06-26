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

// Algoritmo de Programação Dinâmica para Casamento Aproximado [cite: 4]
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
    sprintf(string_ocorrencias, "%s", padrao);
    
    for (int i = 0; i <= tam_padrao; i++) {
        coluna_anterior[i] = i;
    }

    for (int j = 1; j <= tam_texto; j++) {
        coluna_atual[0] = 0; 
        for (int i = 1; i <= tam_padrao; i++) {
            resultado.num_comparacoes++;
            int custo = (texto[j - 1] == padrao[i - 1]) ? 0 : 1;
            coluna_atual[i] = minimoDeTres(coluna_atual[i - 1] + 1,      // Inserção
                                         coluna_anterior[i] + 1,         // Deleção
                                         coluna_anterior[i - 1] + custo); // Substituição/Match
        }

        if (coluna_atual[tam_padrao] <= k_erros && j >= tam_padrao) {
            char temp[32];
            sprintf(temp, " %d", j - tam_padrao + 2); // Ocorrência termina em j, começa em j-m+1 (base 1)
            strcat(string_ocorrencias, temp);
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

// Algoritmo Shift-And para Casamento Aproximado [cite: 5]
ResultadoParte1 ShiftAnd(char *texto, int tam_texto, char *padrao, int tam_padrao, int k_erros) {
    if (tam_padrao > (sizeof(unsigned long) * CHAR_BIT)) {
         ResultadoParte1 resultado = {0};
         resultado.saida_ocorrencias = (char*)malloc(100);
         sprintf(resultado.saida_ocorrencias, "%s ERRO: Padrão muito longo para ShiftAnd (max %zu bits)\n", padrao, sizeof(unsigned long) * CHAR_BIT);
         return resultado;
    }

    ResultadoParte1 resultado = {0};
    clock_t inicio = clock();

    unsigned long B[TAMANHO_ALFABETO];
    for (int i = 0; i < TAMANHO_ALFABETO; i++) B[i] = 0;
    for (int i = 0; i < tam_padrao; i++) B[(unsigned char)padrao[i]] |= (1UL << i);

    char *string_ocorrencias = (char *)malloc(MAX_LINHA_SAIDA);
    sprintf(string_ocorrencias, "%s", padrao);

    unsigned long mascara_final = 1UL << (tam_padrao - 1);
    unsigned long R[k_erros + 1];
    for(int i = 0; i <= k_erros; i++) R[i] = 0;

    for (int j = 0; j < tam_texto; j++) {
        resultado.num_comparacoes++; // Conta cada caracter do texto processado

        unsigned long R_anterior[k_erros + 1];
        for(int i = 0; i <= k_erros; i++) R_anterior[i] = R[i];

        // Acerto/Substituição
        R[0] = ((R_anterior[0] << 1) | 1) & B[(unsigned char)texto[j]];
        
        for (int i = 1; i <= k_erros; i++) {
            unsigned long substituicao = ((R_anterior[i] << 1) | 1) & B[(unsigned char)texto[j]];
            unsigned long delecao = R_anterior[i - 1];
            unsigned long insercao = R[i-1];
            unsigned long acerto = R_anterior[i-1] << 1;
            R[i] = substituicao | delecao | insercao | acerto;
        }

        if (R[k_erros] & mascara_final) {
            char temp[32];
            sprintf(temp, " %d", j - tam_padrao + 2); // Ocorrência termina em j (base 0)
            strcat(string_ocorrencias, temp);
        }
    }

    strcat(string_ocorrencias, "\n");
    resultado.saida_ocorrencias = string_ocorrencias;
    resultado.tempo = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;
    return resultado;
}