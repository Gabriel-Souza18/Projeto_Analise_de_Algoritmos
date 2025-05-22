#include "Solucao.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

Resultado* resolverComHeuristica(Povos *povos, Caminhos *caminhos, int D, int W) {
    Resultado *res = criarResultado(povos->numPovos);
    float melhorRatio = 0;
    int escolhido = -1;

    for (int i = 0; i < povos->numPovos; i++) {
        float ratio = (float)povos->povos[i].habilidade / povos->povos[i].peso;
        if (ratio > melhorRatio) {
            melhorRatio = ratio;
            escolhido = i;
        }
    }

    if (escolhido != -1) {
        int qtd = W / povos->povos[escolhido].peso;
        res->visitados[res->tamanho] = povos->povos[escolhido].id;
        res->quantidadeRecrutada[res->tamanho] = qtd;
        res->habilidadeTotal = qtd * povos->povos[escolhido].habilidade;
        res->tamanho++;
    }
    return res;
}

Resultado* resolverComPD(Povos *povos, Caminhos *caminhos, int D, int W) {
    int n = povos->numPovos;
    int *dp = (int *)calloc(W + 1, sizeof(int));
    int *from = (int *)calloc(W + 1, sizeof(int));

    for (int i = 0; i <= W; i++) from[i] = -1;

    for (int i = 0; i < n; i++) {
        int peso = povos->povos[i].peso;
        int habilidade = povos->povos[i].habilidade;

        for (int w = peso; w <= W; w++) {
            if (dp[w - peso] + habilidade > dp[w]) {
                dp[w] = dp[w - peso] + habilidade;
                from[w] = i;
            }
        }
    }

    Resultado *res = criarResultado(n);
    int w = W;
    int *quantidades = (int *)calloc(n, sizeof(int));

    while (w > 0 && from[w] != -1) {
        int povoIndex = from[w];
        quantidades[povoIndex]++;
        w -= povos->povos[povoIndex].peso;
    }

    for (int i = 0; i < n; i++) {
        if (quantidades[i] > 0) {
            res->visitados[res->tamanho] = povos->povos[i].id;
            res->quantidadeRecrutada[res->tamanho] = quantidades[i];
            res->habilidadeTotal += quantidades[i] * povos->povos[i].habilidade;
            res->tamanho++;
        }
    }

    free(dp);
    free(from);
    free(quantidades);
    return res;
}