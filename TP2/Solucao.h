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
typedef struct {
    int povo;
    int distanciaa_restante;
    int peso_usado;
    int habilidade;
    int *caminho; 
    int *quantidades;
    int tamanho_caminho; 
} Estado;  
typedef struct NoFila {
    Estado estado;
    struct NoFila *prox;
} NoFila;

typedef struct FilaPrioridade {
    NoFila *cabeca;
    int (*comparador)(const Estado*, const Estado*);
} FilaPrioridade;
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

Povos filtrarPorDistancia(Povos *original, Grafo *grafo, int D);

FilaPrioridade* criar_fila(int (*comparador)(const Estado*, const Estado*));
void liberar_fila(FilaPrioridade *fila);
void enfileirar(FilaPrioridade *fila, Estado estado);
Estado desenfileirar(FilaPrioridade *fila);
int fila_vazia(FilaPrioridade *fila);
int comparador_habilidade(const Estado *a, const Estado *b);
#endif