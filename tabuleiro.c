#include "tabuleiro.h"
#include <stdlib.h>
#include <stdio.h>

// inicializa um novo tabuleiro utilizando os dados na string de entrada
//o parâmetro 'entrada' é um ponteiro para ponteiro, permitindo avançar a posição atual da leitura
Tabuleiro* inicializaTabuleiro(char **entrada) {
    Tabuleiro *tab = malloc(sizeof(Tabuleiro));
    if (!tab)
        return NULL;
    
    // lê as dimensões do tabuleiro
    sscanf(*entrada, "%d %d", &tab->N, &tab->M);
    while (**entrada != '\n' && **entrada != '\0')
        (*entrada)++;
    if (**entrada == '\n')
        (*entrada)++;

    // calcula o total de casas válidas no tabuleiro
    int totalCasas = (tab->N * tab->M + 1) / 2;
    tab->estado = malloc(sizeof(int) * totalCasas);

    // preenche o vetor de estado com os dados dados na entrada
    for (int i = 0; i < totalCasas; i++) {
        sscanf(*entrada, "%d", &tab->estado[i]);
        // avança o ponteiro até encontrar um espaço, fim da linha ou fim da string
        while (**entrada != ' ' && **entrada != '\0' && **entrada != '\n')
            (*entrada)++;
        if (**entrada == ' ')
            (*entrada)++;
    }
    
    // vai para o início da próxima linha
    while (**entrada != '\n' && **entrada != '\0')
        (*entrada)++;
    if (**entrada == '\n')
        (*entrada)++;

    return tab;
}

// libera a memória alocada para o tabuleiro 
void liberaTabuleiro(Tabuleiro *tabuleiro) {
    if (tabuleiro) {
        free(tabuleiro->estado);
        free(tabuleiro);
    }
}
