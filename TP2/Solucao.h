#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "Caminhos.h"

#define MAX_OUTPUT_STRING_SIZE 4096 // tamanho do buffer pra string de saída
#define MAX_FILENAME_LENGTH 50      // tamanho do buffer pros nomes no arquivo de saída

// Estrutura para armazenar o resultado da solução
typedef struct Resultado {
    int habilidadeTotal;        
    int *visitados;             // vetor com os id's dos povos visitados em ordem
    int *quantidadeRecrutada;   // vetor com a quantidade recrutada em cada povo
    int tamanho;                // número de povos visitados
} Resultado;

// Estrutura para representar um estado na busca (usada na fila de prioridade)
typedef struct Estado {
    int povo;                   // id do povo atual
    int distancia_restante;     // distância restante que Zorc pode percorrer
    int peso_usado;             // peso total dos soldados recrutados até agora
    int habilidade;             // habilidade total acumulada
    int *caminho;               // vetor dinâmico para o caminho percorrido (IDs dos povos)
    int *quantidades;           // vetor dinâmico para quantidades recrutadas por ID de povo
    int tamanho_caminho;        // tamanho atual do caminho
} Estado;   

// Nó da fila de prioridade
typedef struct NoFila {
    Estado estado;
    struct NoFila *prox;
} NoFila;

// Fila de Prioridade genérica (com comparador)
typedef struct FilaPrioridade {
    NoFila *cabeca;     
    int (*comparador)(const Estado*, const Estado*); // Ponteiro para função de comparação
} FilaPrioridade;

// Estrutura para representar um vizinho no grafo
typedef struct Vizinho {
    int id;             // id do povo vizinho
    int distancia;      // distância até este vizinho
    struct Vizinho *prox;   // Próximo vizinho na lista de adjacência
} Vizinho;

// Estrutura para representar o grafo
typedef struct Grafo {
    int numPovos;   // Número total de povos (incluindo id 0 se for o caso)
    Vizinho **adj;  // Lista de adjacência (array de ponteiros para Vizinho)
} Grafo;

// Funções para criação/liberação de Resultado
Resultado* criarResultado(int tamanho);
void liberarResultado(Resultado *res);

// Funções para as soluções
Resultado* resolverComHeuristica(Povos *povos, Caminhos *caminhos, int D, int W);
Resultado* resolverComPD(Povos *povos, Caminhos *caminhos, int D, int W);

// Funções para manipulação do Grafo
// Adicionado 'Povos *povos' para criarGrafo poder determinar max_id
Grafo* criarGrafo(Caminhos *caminhos, Povos *povos); 
void adicionarAresta(Grafo *grafo, int origem, int destino, int distancia);
void liberarGrafo(Grafo *grafo);

// Função para gerar a string de saída
char* gerarSaida(Resultado *res);

// Funções auxiliares para a heurística
Povos filtrarPorDistancia(Povos *original, Grafo *grafo, int D);
int comparePovosByRatio(const void *a, const void *b); // Comparador para qsort

// Funções para a fila de prioridade (para programação dinâmica)
FilaPrioridade* criar_fila(int (*comparador)(const Estado*, const Estado*));
void liberar_fila(FilaPrioridade *fila);
void enfileirar(FilaPrioridade *fila, Estado estado);
Estado desenfileirar(FilaPrioridade *fila); // Retorna uma cópia do estado
int fila_vazia(FilaPrioridade *fila);
int comparador_habilidade(const Estado *a, const Estado *b); // Comparador específico para habilidade

#endif