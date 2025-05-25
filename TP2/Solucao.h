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

typedef struct Vizinho {
    int id;           
    int distancia;     
    struct Vizinho *prox;
} Vizinho;
typedef struct Grafo {
    int numPovos;  
    Vizinho **adj;      
} Grafo;
// Resolve a instância usando uma heurística gulosa
Resultado* resolverComHeuristica(Povos *povos, Caminhos *caminhos, int D, int W);

// Resolve a instância com programação dinâmica 
Resultado* resolverComPD(Povos *povos, Caminhos *caminhos, int D, int W);

//Resolve levando a distancia em consideração
Resultado* resolverComDistancia(Povos *povos, Caminhos *caminhos, int D, int W);

Grafo* criarGrafo(Caminhos *caminhos, int numPovos);
void adicionarAresta(Grafo *grafo, int origem, int destino, int distancia);
void liberarGrafo(Grafo *grafo);
void dfs(Povos *povos, Grafo *grafo, int atual, int Drestante, int *visitados, int *melhorHabilidade, Resultado **melhorRes, int W);
// Libera memória da estrutura Resultado
void liberarResultado(Resultado *res);

// Gera uma string formatada com a resposta 
char* gerarSaida(Resultado *res);

#endif