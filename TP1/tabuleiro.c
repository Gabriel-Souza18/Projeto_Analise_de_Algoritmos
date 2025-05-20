#include "tabuleiro.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// inicializa um novo tabuleiro utilizando os dados na string de entrada
// o parâmetro 'entrada' é um ponteiro para ponteiro, permitindo avançar a posição atual da leitura
Tabuleiro* inicializaTabuleiro(char **entrada){
    Tabuleiro *tab = malloc(sizeof(Tabuleiro));
    if (!tab){
        perror("Erro ao alocar memória para tabuleiro");
        return NULL;
    }

    // Lê as dimensões N e M
    if (sscanf(*entrada, "%d %d", &tab->N, &tab->M) != 2) {
        fprintf(stderr, "Erro ao ler dimensões do tabuleiro.\n");
        free(tab);
        return NULL;
    }

    // Avança o ponteiro para o início da linha de estado (após N M\n)
    *entrada = strchr(*entrada, '\n');
    if (!*entrada) {
         fprintf(stderr, "Erro: Formato de entrada inválido - sem newline após dimensões.\n");
         free(tab);
         return NULL;
    }
    (*entrada)++; // Pula o caractere '\n'

    // Calcula o total de casas e aloca memória
    int totalCasas = (tab->N * tab->M + 1) / 2;
    tab->estado = malloc(sizeof(int) * totalCasas);
    if (!tab->estado) {
        perror("Erro ao alocar memória para estado do tabuleiro");
        free(tab);
        return NULL;
    }

    // Preenche o vetor de estado
    char *posicao_atual = *entrada; // Usa um ponteiro local para parsear a linha
    int chars_lidos = 0;
    for(int i = 0; i < totalCasas; i++){
        // Lê o inteiro e conta quantos caracteres foram lidos
        if (sscanf(posicao_atual, "%d%n", &tab->estado[i], &chars_lidos) != 1) {
            fprintf(stderr, "Erro ao ler estado %d do tabuleiro (esperava %d).\n", i, totalCasas);
            free(tab->estado);
            free(tab);
            return NULL; // Retorna NULL em caso de erro de leitura
        }
        posicao_atual += chars_lidos; // Avança o ponteiro local

        // Pula espaços em branco após o número lido
        posicao_atual += strspn(posicao_atual, " \t");

        // Verifica fim inesperado da linha
        if ((*posicao_atual == '\n' || *posicao_atual == '\0') && i < totalCasas - 1) {
             fprintf(stderr, "Erro: Fim inesperado da linha de estado após ler %d de %d estados.\n", i + 1, totalCasas);
             free(tab->estado);
             free(tab);
             return NULL;
        }
    }

    // Atualiza o ponteiro original para apontar para o início da próxima linha
    *entrada = posicao_atual; // Aponta para onde paramos (deve ser o \n ou \0)
    if (**entrada == '\n') { // Se paramos no \n, avança para o próximo caractere
        (*entrada)++;
    }
    // Se **entrada for '\0', o ponteiro já está no fim.

    return tab;
}

// libera a memória alocada para o tabuleiro 
void liberaTabuleiro(Tabuleiro *tabuleiro) {
    if (tabuleiro){
        free(tabuleiro->estado);
        free(tabuleiro);
    }
}