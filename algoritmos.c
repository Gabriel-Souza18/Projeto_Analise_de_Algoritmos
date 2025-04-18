#include "algoritmos.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

// Direções: cima esquerda, cima direita, baixo esquerda, baixo direita
int direcoes[4][2] = {
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1}
};

// Converte coordenadas (linha, coluna) para índice no vetor de estado
int coordenadaParaIndice(int linha, int coluna, int N, int M) {
    if ((linha + coluna) % 2 != 0) return -1; // casa não jogável
    return (linha * M + coluna) / 2;
}

int getCasa(Tabuleiro *tab, int i, int j) {
    if (i < 0 || j < 0 || i >= tab->N || j >= tab->M) return -1;
    if ((i + j) % 2 != 0) return -1;
    int idx = coordenadaParaIndice(i, j, tab->N, tab->M);
    return tab->estado[idx];
}

void setCasa(Tabuleiro *tab, int i, int j, int valor) {
    int idx = coordenadaParaIndice(i, j, tab->N, tab->M);
    if (idx >= 0) tab->estado[idx] = valor;
}

// FORÇA BRUTA - com recursão
int maxCapturas(Tabuleiro *tab, int i, int j) {
    int maxCaptura = 0;

    for (int d = 0; d < 4; d++) {
        int mi = i + direcoes[d][0];
        int mj = j + direcoes[d][1];
        int ni = i + 2 * direcoes[d][0];
        int nj = j + 2 * direcoes[d][1];

        if (getCasa(tab, mi, mj) == 2 && getCasa(tab, ni, nj) == 0) {
            setCasa(tab, i, j, 0);
            setCasa(tab, mi, mj, 0);
            setCasa(tab, ni, nj, 1);

            int capturas = 1 + maxCapturas(tab, ni, nj);
            maxCaptura = MAX(maxCaptura, capturas);

            setCasa(tab, i, j, 1);
            setCasa(tab, mi, mj, 2);
            setCasa(tab, ni, nj, 0);
        }
    }

    return maxCaptura;
}

char* forcaBruta(Tabuleiro* tabuleiro) {
    int maxTotal = 0;

    for (int i = 0; i < tabuleiro->N; i++) {
        for (int j = 0; j < tabuleiro->M; j++) {
            if (getCasa(tabuleiro, i, j) == 1) {
                int capturas = maxCapturas(tabuleiro, i, j);
                if (capturas > maxTotal) maxTotal = capturas;
            }
        }
    }

    char *res = malloc(16);
    sprintf(res, "%d\n", maxTotal);
    return res;
}

// MÍNIMOS MOVIMENTOS - com pilha dinâmica e otimizado
char* minMovimentos(Tabuleiro* tabuleiro) {
    int maxCaptura = 0;

    typedef struct {
        int i, j;
        int capturas;
        int* estadoCopia;
    } Estado;

    int tamanho = (tabuleiro->N * tabuleiro->M + 1) / 2;

    for (int i = 0; i < tabuleiro->N; i++) {
        for (int j = 0; j < tabuleiro->M; j++) {
            if (getCasa(tabuleiro, i, j) != 1) continue;

            Estado* pilha = malloc(sizeof(Estado) * 100);
            int capacidade = 100;
            int topo = 0;

            int* estadoInicial = malloc(sizeof(int) * tamanho);
            memcpy(estadoInicial, tabuleiro->estado, sizeof(int) * tamanho);
            pilha[topo++] = (Estado){i, j, 0, estadoInicial};

            while (topo > 0) {
                Estado atual = pilha[--topo];
                int capturou = 0;

                // Restaura o estado do tabuleiro
                Tabuleiro copia;
                copia.N = tabuleiro->N;
                copia.M = tabuleiro->M;
                copia.estado = atual.estadoCopia;

                for (int d = 0; d < 4; d++) {
                    int mi = atual.i + direcoes[d][0];
                    int mj = atual.j + direcoes[d][1];
                    int ni = atual.i + 2 * direcoes[d][0];
                    int nj = atual.j + 2 * direcoes[d][1];

                    if (getCasa(&copia, mi, mj) == 2 && getCasa(&copia, ni, nj) == 0) {
                        // Cria novo estado modificado
                        int* novoEstado = malloc(sizeof(int) * tamanho);
                        memcpy(novoEstado, copia.estado, sizeof(int) * tamanho);
                        Tabuleiro novo;
                        novo.N = tabuleiro->N;
                        novo.M = tabuleiro->M;
                        novo.estado = novoEstado;

                        setCasa(&novo, atual.i, atual.j, 0);
                        setCasa(&novo, mi, mj, 0);
                        setCasa(&novo, ni, nj, 1);

                        // Redimensiona pilha se necessário
                        if (topo >= capacidade) {
                            capacidade *= 2;
                            pilha = realloc(pilha, sizeof(Estado) * capacidade);
                        }
                        pilha[topo++] = (Estado){ni, nj, atual.capturas + 1, novoEstado};
                        capturou = 1;
                    }
                }

                if (!capturou && atual.capturas > maxCaptura) {
                    maxCaptura = atual.capturas;
                }

                free(atual.estadoCopia);
            }

            free(pilha);
        }
    }

    char *res = malloc(16);
    sprintf(res, "%d\n", maxCaptura);
    return res;
}