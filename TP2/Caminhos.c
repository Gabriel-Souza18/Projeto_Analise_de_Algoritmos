#include "Caminhos.h"
#include <stdio.h>
#include <stdlib.h>

Povo *criarPovo(int id, int peso, int habilidade) {
    Povo *povo = (Povo *)malloc(sizeof(Povo));
    if (!povo) {
        perror("Erro ao alocar memória para o povo");
        return NULL;
    }
    povo->id = id;
    povo->peso = peso;
    povo->habilidade = habilidade;
    return povo;
}

// Renomeado para criarCaminho (singular)
Caminho *criarCaminho(int origem, int destino, int distancia) {
    if (distancia <= 0) {
        fprintf(stderr, "Distância inválida. A distância deve ser maior que zero.\n");
        return NULL;
    }
    Caminho *caminho = malloc(sizeof(Caminho));
    if (!caminho) {
        perror("Erro ao alocar memória para o caminho");
        return NULL;
    }
    caminho->origem = origem;
    caminho->destino = destino;
    caminho->distancia = distancia;
    return caminho;
}

void adicionarPovo(Povos *povos, Povo *povo) {
    // Inicializa a capacidade se for a primeira adição
    // Garante que povos->capacidade seja inicializado para 0 na declaração da struct
    if (povos->numPovos == 0 && povos->capacidade == 0) { 
        povos->capacidade = 1;
        povos->povos = (Povo *)malloc(povos->capacidade * sizeof(Povo));
        if (!povos->povos) {
            perror("Erro ao alocar memória inicial para os povos");
            return;
        }
    } 
    // Dobra a capacidade se o array estiver cheio
    else if (povos->numPovos == povos->capacidade) {
        int novaCapacidade = povos->capacidade * 2;
        Povo *temp = (Povo *)realloc(povos->povos, novaCapacidade * sizeof(Povo));
        if (!temp) {
            perror("Erro ao realocar memória para os povos");
            return;
        }
        povos->povos = temp;
        povos->capacidade = novaCapacidade;
    }
    // Adiciona o novo povo e incrementa o contador
    povos->povos[povos->numPovos] = *povo;
    povos->numPovos++;
}

void adicionarCaminho(Caminhos *caminhos, Caminho *caminho) {
    // Inicializa a capacidade se for a primeira adição
    // Garante que caminhos->capacidade seja inicializado para 0 na declaração da struct
    if (caminhos->numCaminhos == 0 && caminhos->capacidade == 0) { 
        caminhos->capacidade = 1; 
        caminhos->caminhos = (Caminho *)malloc(caminhos->capacidade * sizeof(Caminho));
        if (!caminhos->caminhos) {
            perror("Erro ao alocar memória inicial para os caminhos");
            return;
        }
    } 
    // Dobra a capacidade
    else if (caminhos->numCaminhos == caminhos->capacidade) {
        int novaCapacidade = caminhos->capacidade * 2;
        Caminho *temp = (Caminho *)realloc(caminhos->caminhos, novaCapacidade * sizeof(Caminho));
        if (!temp) {
            perror("Erro ao realocar memória para os caminhos");
            return;
        }
        caminhos->caminhos = temp;
        caminhos->capacidade = novaCapacidade;
    }
    // Adiciona o novo caminho e incrementa o contador
    caminhos->caminhos[caminhos->numCaminhos] = *caminho;
    caminhos->numCaminhos++;
}

void destruirPovos(Povos *povos) {
    if (povos != NULL) {
        free(povos->povos);
        povos->povos = NULL;
        povos->numPovos = 0;
        povos->capacidade = 0;
    }
}

void destruirCaminhos(Caminhos *caminhos) {
    if (caminhos != NULL) {
        free(caminhos->caminhos);
        caminhos->caminhos = NULL;
        caminhos->numCaminhos = 0;
        caminhos->capacidade = 0;
    }
}

void printarPovos(Povos *povos) {
    printf("--- Povos ---\n");
    for (int i = 0; i < povos->numPovos; i++) {
        printf("Povo %d: Peso %d, Habilidade %d\n", povos->povos[i].id, povos->povos[i].peso, povos->povos[i].habilidade);
    }
    printf("---------------\n");
}

void printarCaminhos(Caminhos *caminhos) {
    printf("--- Caminhos ---\n");
    for (int i = 0; i < caminhos->numCaminhos; i++) {
        printf("Caminho de %d para %d: Distância %d\n", caminhos->caminhos[i].origem, caminhos->caminhos[i].destino, caminhos->caminhos[i].distancia);
    }
    printf("---------------\n");
}