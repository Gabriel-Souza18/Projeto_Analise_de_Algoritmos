#ifndef TABULEIRO_H
#define TABULEIRO_H

// estrutura que representa o tabuleiro 
// armazena as dimensões e o vetor de estado das casas válidas
typedef struct tabuleiro{
    int N;      // número de linhas 
    int M;      // número de colunas
    int *estado; // vetor que guarda as casas válidas
}Tabuleiro;

// inicializa um tabuleiro utilizando os dados presentes na entrada
// a função avança o ponteiro de entrada quando os valores são lidos
Tabuleiro* inicializaTabuleiro(char** entrada);

// libera a memória alocada para o tabuleiro
void liberaTabuleiro(Tabuleiro *tab);

#endif 