#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "Caminhos.h"

// Estrutura que armazena a solução de uma instância
typedef struct Resultado {
    int habilidadeTotal;        // soma total da habilidade dos soldados recrutados
    int *visitados;             // vetor com os ids dos povos visitados
    int *quantidadeRecrutada;   // quantidade de soldados recrutados em cada povo
    int tamanho;                // número de povos visitados
} Resultado;

// Resolve a instância usando uma heurística gulosa
Resultado* resolverComHeuristica(Povos *povos, Caminhos *caminhos, int D, int W);

// Resolve a instância com programação dinâmica 
Resultado* resolverComPD(Povos *povos, Caminhos *caminhos, int D, int W);

// Libera memória da estrutura Resultado
void liberarResultado(Resultado *res);

// Gera uma string formatada com a resposta 
char* gerarSaida(Resultado *res);

#endif