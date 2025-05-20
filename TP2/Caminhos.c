#include "Caminhos.h"
#include <stdio.h>
#include <stdlib.h>

Povo * criarPovo(int id, int peso, int habilidade){
    Povo *povo = (Povo *)malloc(sizeof(Povo));
    if (!povo){
        perror("Erro ao alocar memória para o povo");
        return NULL;
    }
    povo->id = id;
    povo->peso = peso;
    povo->habilidade = habilidade;
    return povo;
}

Caminho * criarCaminhos(int origem, int destino, int distancia){
    Caminho *caminho = (Caminho *)malloc(sizeof(Caminho));
    if (!caminho){
        perror("Erro ao alocar memória para o caminho");
        return NULL;
    }
    if(distancia < 0){
        printf("Distância inválida. A distância deve ser maior ou igual a zero.\n");
        free(caminho);
    }
    caminho->origem = origem;
    caminho->destino = destino;
    caminho->distancia = distancia;
    return caminho;
}
void adicionarPovo(Povos *povos, Povo *povo){
    if (povos->numPovos == 0){
        povos->povos = (Povo *)malloc(sizeof(Povo));
    } else {
        povos->povos = (Povo *)realloc(povos->povos, (povos->numPovos + 1) * sizeof(Povo));
    }
    povos->povos[povos->numPovos] = *povo;
    povos->numPovos++;
}

void adicionarCaminho(Caminhos *caminhos, Caminho *caminho){
    if (caminhos->numCaminhos == 0){
        caminhos->caminhos = (Caminho *)malloc(sizeof(Caminho));
    } else {
        caminhos->caminhos = (Caminho *)realloc(caminhos->caminhos, (caminhos->numCaminhos + 1) * sizeof(Caminho));
    }
    caminhos->caminhos[caminhos->numCaminhos] = *caminho;
    caminhos->numCaminhos++;
}

void destruirPovos(Povos *povos){
    if (povos != NULL){
        free(povos->povos);
        povos->povos = NULL;
        povos->numPovos = 0;
    }
}
void destruirCaminhos(Caminhos *caminhos){
    if (caminhos != NULL){
        free(caminhos->caminhos);
        caminhos->caminhos = NULL;
        caminhos->numCaminhos = 0;
    }
}
void printarPovos(Povos *povos){
    for (int i = 0; i < povos->numPovos; i++){
        printf("Povo %d: Peso %d, Habilidade %d\n", povos->povos[i].id, povos->povos[i].peso, povos->povos[i].habilidade);
    }
}

void printarCaminhos(Caminhos *caminhos){
    for (int i = 0; i < caminhos->numCaminhos; i++){
        printf("Caminho de %d para %d: Distância %d\n", caminhos->caminhos[i].origem, caminhos->caminhos[i].destino, caminhos->caminhos[i].distancia);
    }
    
}