#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "Caminhos.h"

#define MAX_OUTPUT_STRING_SIZE 4096 // tamanho do buffer pra string de saída
#define MAX_FILENAME_LENGTH 50      // tamanho do buffer pros nomes no arquivo de saída

typedef struct Resultado {
    int habilidadeTotal;        
    int *visitados;             // vetor com os id's dos povos visitados em ordem
    int *quantidadeRecrutada;  
    int tamanho;                
} Resultado;

typedef struct Estado{
    int povo;                   // id do povo atual
    int distancia_restante;     // distância restante que Zorc pode percorrer
    int peso_usado;             // peso total dos soldados recrutados até agora
    int habilidade;             // habilidade total acumulada
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
    int id;                 // id do povo vizinho
    int distancia;          // distância até este vizinho
    struct Vizinho *prox;   
} Vizinho;

typedef struct Grafo {
    int numPovos;   
    Vizinho **adj;  
} Grafo;

Resultado* resolverComHeuristica(Povos *povos, Caminhos *caminhos, int D, int W);

Resultado* resolverComPD(Povos *povos, Caminhos *caminhos, int D, int W);

Grafo* criarGrafo(Caminhos *caminhos, int numPovos);

void adicionarAresta(Grafo *grafo, int origem, int destino, int distancia);

void liberarGrafo(Grafo *grafo);

void liberarResultado(Resultado *res);

char* gerarSaida(Resultado *res);

Povos filtrarPorDistancia(Povos *original, Grafo *grafo, int D);

// funções para a fila de prioridade
FilaPrioridade* criar_fila(int (*comparador)(const Estado*, const Estado*));
void liberar_fila(FilaPrioridade *fila);
void enfileirar(FilaPrioridade *fila, Estado estado);
Estado desenfileirar(FilaPrioridade *fila);
int fila_vazia(FilaPrioridade *fila);
int comparador_habilidade(const Estado *a, const Estado *b);

int comparePovosByRatio(const void *a, const void *b);

#endif