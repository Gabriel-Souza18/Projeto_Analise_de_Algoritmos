#ifndef CAMINHOS_H
#define CAMINHOS_H

// Representa um povo
typedef struct Povo{
    int id;         
    int peso;       
    int habilidade; 
} Povo;

// Caminho entre dois povos
typedef struct Caminho{
    int origem;     // id do povo de origem
    int destino;    // id do povo de destino
    int distancia;  
} Caminho;

// Armazenar os povos 
typedef struct Povos{
    Povo *povos;    
    int numPovos;   
    int capacidade; 
} Povos;

// Armazena os caminhos
typedef struct Caminhos{
    Caminho *caminhos;  
    int numCaminhos;    
    int capacidade;     
} Caminhos;

Povo * criarPovo(int id, int peso, int habilidade);

void adicionarPovo(Povos *povos, Povo *povo);

void destruirPovos(Povos *povos);

void printarPovos(Povos *povos);

Caminho * criarCaminhos(int origem, int destino, int distancia);

void adicionarCaminho(Caminhos *caminhos, Caminho *caminho);

void destruirCaminhos(Caminhos *caminhos);

void printarCaminhos(Caminhos *caminhos);

#endif