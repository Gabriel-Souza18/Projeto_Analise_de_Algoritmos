#include "Solucao.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h> //pra usaar o INT_MAX

Resultado* criarResultado(int tamanho) {
    Resultado *res = (Resultado *)malloc(sizeof(Resultado));
    res->visitados = (int *)malloc(tamanho * sizeof(int));
    res->quantidadeRecrutada = (int *)malloc(tamanho * sizeof(int));
    res->tamanho = 0;
    res->habilidadeTotal = 0;
    return res;
}

void liberarResultado(Resultado *res) {
    if (res) {
        free(res->visitados);
        free(res->quantidadeRecrutada);
        free(res);
    }
}

char* gerarSaida(Resultado *res) {
    if (!res || res->tamanho == 0) return NULL;
    char *saida = (char *)malloc(1024 * sizeof(char));
    char buffer[64];
    sprintf(saida, "%d", res->habilidadeTotal);
    for (int i = 0; i < res->tamanho; i++) {
        sprintf(buffer, " %d %d", res->visitados[i], res->quantidadeRecrutada[i]);
        strcat(saida, buffer);
    }
    strcat(saida, "\n");
    return saida;
}


static int *bfs_distancias(Grafo *grafo,int inicio ){
    int num = grafo->numPovos;
    int *dist = malloc(num * sizeof *dist);
    int *visitados  = calloc(num, sizeof *visitados);
    int *fila = malloc(num * sizeof *fila);
    int ini = 0, fim = 0;

    for (int i = 0; i < num; i++) dist[i] = INT_MAX;
    dist[inicio] = 0;
    visitados[inicio]  = 1;
    fila[fim++] = inicio;

    while (ini < fim) {
        int u = fila[ini++];
        for (Vizinho *v = grafo->adj[u]; v; v = v->prox) {
            if (!visitados[v->id]) {
                visitados[v->id]   = 1;
                dist[v->id]  = dist[u] + 1;
                fila[fim++] = v->id;
            }
        }
    }

    free(visitados);
    free(fila);
    return dist;
}

Povos filtrarPorDistancia(Povos *original, Grafo *grafo, int D) {
    Povos f = { .povos = NULL,
                .numPovos = 0 };
    int *dist = bfs_distancias(grafo,1);

    for (int i = 0; i < original->numPovos; i++) {
        int id = original->povos[i].id;
        if (id < grafo->numPovos && dist[id] <= D) {
            Povo *p = criarPovo(
                id,
                original->povos[i].peso,
                original->povos[i].habilidade);
            adicionarPovo(&f, p);
        }
    }
    free(dist);
    return f;
}


Resultado* resolverComHeuristica(Povos *povos, Caminhos *caminhos,int D, int W){
    Grafo *grafo = criarGrafo(caminhos, povos->numPovos);
    Povos filtrados = filtrarPorDistancia(povos, grafo, D);
    liberarGrafo(grafo);

    int n = filtrados.numPovos;
    int cap = W;
    Resultado *melhorRes = criarResultado(n);

    int *indices = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }

    for(int i =0 ; i<n; i++){//ordenar por peso/habilidade
        for(int j = i+1; j<n; j++){
            float ri = (float)filtrados.povos[indices[i]].habilidade 
                        / filtrados.povos[indices[i]].peso;
            float rj = (float)filtrados.povos[indices[j]].habilidade 
                        / filtrados.povos[indices[j]].peso;

            if (ri < rj) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }
    for(int i = 0; i < n && cap > 0; i++) {
        Povo *povo = &filtrados.povos[indices[i]];
        int quantidade = cap/povo->peso;

        if (quantidade>0) {
            melhorRes->visitados[melhorRes->tamanho] = povo->id;
            melhorRes->quantidadeRecrutada[melhorRes->tamanho] = quantidade;
            melhorRes->habilidadeTotal += quantidade * povo->habilidade;
            melhorRes->tamanho++;
            cap -= quantidade * povo->peso;
        }
    }
    free(indices);
    destruirPovos(&filtrados);
    return melhorRes;
}

Resultado *resolverComPD(Povos* povos, Caminhos *caminhos, int D, int pesoMax) {
    Grafo *grafo = criarGrafo(caminhos, povos->numPovos);
    Resultado *melhor = criarResultado(povos->numPovos);
    FilaPrioridade *fila = criar_fila(comparador_habilidade);

    for (int i = 0; i < povos->numPovos; i++) {
        int povo_id = povos->povos[i].id;
        Estado estado_inicial = {
            .povo = povo_id,
            .distanciaa_restante = D,
            .peso_usado = 0,
            .habilidade = 0,
            .tamanho_caminho = 1,
            .caminho = (int *)malloc((povos->numPovos + 1) * sizeof(int)),
            .quantidades = (int *)calloc(povos->numPovos + 1, sizeof(int))
        };
        estado_inicial.caminho[0] = povo_id;

        int max_recrutar = pesoMax / povos->povos[i].peso;
        if (max_recrutar > 0) {
            estado_inicial.quantidades[0] = max_recrutar;
            estado_inicial.habilidade = max_recrutar * povos->povos[i].habilidade;
            estado_inicial.peso_usado = max_recrutar * povos->povos[i].peso;
        }

        enfileirar(fila, estado_inicial);
    }

    while (!fila_vazia(fila)) {
        Estado estado_atual = desenfileirar(fila);

        if (estado_atual.habilidade > melhor->habilidadeTotal) {
            melhor->habilidadeTotal = estado_atual.habilidade;
            melhor->tamanho = estado_atual.tamanho_caminho;
            memcpy(melhor->visitados, estado_atual.caminho, estado_atual.tamanho_caminho * sizeof(int));
            memcpy(melhor->quantidadeRecrutada, estado_atual.quantidades, estado_atual.tamanho_caminho * sizeof(int));
        }

        for (Vizinho *viz = grafo->adj[estado_atual.povo]; viz != NULL; viz = viz->prox) {
            if (viz->distancia > estado_atual.distanciaa_restante) continue;

            Estado novo_estado = {
                .povo = viz->id,
                .distanciaa_restante = estado_atual.distanciaa_restante - viz->distancia,
                .peso_usado = estado_atual.peso_usado,
                .habilidade = estado_atual.habilidade,
                .tamanho_caminho = estado_atual.tamanho_caminho,
                .caminho = (int *)malloc((povos->numPovos + 1) * sizeof(int)),
                .quantidades = (int *)calloc(povos->numPovos + 1, sizeof(int))
            };

            memcpy(novo_estado.caminho, estado_atual.caminho, estado_atual.tamanho_caminho * sizeof(int));
            memcpy(novo_estado.quantidades, estado_atual.quantidades, estado_atual.tamanho_caminho * sizeof(int));

            Povo *povo_atual = NULL;
            for (int i = 0; i < povos->numPovos; i++) {
                if (povos->povos[i].id == viz->id) {
                    povo_atual = &povos->povos[i];
                    break;
                }
            }

            int recrutados = 0;
            if (povo_atual) {
                recrutados = (pesoMax - novo_estado.peso_usado) / povo_atual->peso;
                if (recrutados > 0) {
                    novo_estado.habilidade += recrutados * povo_atual->habilidade;
                    novo_estado.peso_usado += recrutados * povo_atual->peso;
                }
            }

            novo_estado.caminho[estado_atual.tamanho_caminho] = viz->id;
            novo_estado.quantidades[estado_atual.tamanho_caminho] = recrutados;
            novo_estado.tamanho_caminho = estado_atual.tamanho_caminho + 1;

            if (novo_estado.peso_usado <= pesoMax) {
                enfileirar(fila, novo_estado);
            } else {
                free(novo_estado.caminho);
                free(novo_estado.quantidades);
            }
        }

        free(estado_atual.caminho);
        free(estado_atual.quantidades);
    }

    liberar_fila(fila);
    liberarGrafo(grafo);
    return melhor;
}
Grafo* criarGrafo(Caminhos *caminhos, int numPovos) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numPovos = numPovos + 1;
    grafo->adj = (Vizinho **)calloc(grafo->numPovos, sizeof(Vizinho *));

    for (int i = 0; i < caminhos->numCaminhos; i++) {
        adicionarAresta(grafo, caminhos->caminhos[i].origem, caminhos->caminhos[i].destino, caminhos->caminhos[i].distancia);
        adicionarAresta(grafo, caminhos->caminhos[i].destino, caminhos->caminhos[i].origem, caminhos->caminhos[i].distancia);
    }

    return grafo;
}

void adicionarAresta(Grafo *grafo, int origem, int destino, int distancia) {
    Vizinho *novo = (Vizinho *)malloc(sizeof(Vizinho));
    novo->id = destino;
    novo->distancia = distancia;
    novo->prox = grafo->adj[origem];
    grafo->adj[origem] = novo;
}

void liberarGrafo(Grafo *grafo) {
    for (int i = 0; i < grafo->numPovos; i++) {
        Vizinho *v = grafo->adj[i];
        while (v) {
            Vizinho *tmp = v;
            v = v->prox;
            free(tmp);
        }
    }
    free(grafo->adj);
    free(grafo);
}

int comparador_habilidade(const Estado *a, const Estado *b) {
    return b->habilidade - a->habilidade;
}

FilaPrioridade* criar_fila(int (*comparador)(const Estado*, const Estado*)) {
    FilaPrioridade *fila = (FilaPrioridade*)malloc(sizeof(FilaPrioridade));
    fila->cabeca = NULL;
    fila->comparador = comparador;
    return fila;
}

void liberar_fila(FilaPrioridade *fila) {
    NoFila *atual = fila->cabeca;
    while (atual) {
        NoFila *prox = atual->prox;
        free(atual->estado.caminho);
        free(atual->estado.quantidades);
        free(atual);
        atual = prox;
    }
    free(fila);
}

void enfileirar(FilaPrioridade *fila, Estado estado) {
    NoFila *novo = (NoFila*)malloc(sizeof(NoFila));
    novo->estado = estado;
    novo->prox = NULL;

    if (!fila->cabeca || fila->comparador(&novo->estado, &fila->cabeca->estado) > 0) {
        novo->prox = fila->cabeca;
        fila->cabeca = novo;
    } else {
        NoFila *atual = fila->cabeca;
        while (atual->prox && fila->comparador(&novo->estado, &atual->prox->estado) <= 0) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

Estado desenfileirar(FilaPrioridade *fila) {
    NoFila *temp = fila->cabeca;
    Estado estado = temp->estado;
    fila->cabeca = temp->prox;
    free(temp);
    return estado;
}

int fila_vazia(FilaPrioridade *fila) {
    return fila->cabeca == NULL;
}


