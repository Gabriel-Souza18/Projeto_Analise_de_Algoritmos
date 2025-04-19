#include "algoritmos.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))

int direcoes[4][2] = {
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1}
};

// Converte coordenadas para índice
int coordenadaParaIndice(int linha, int coluna, int N, int M) {
    if ((linha + coluna) % 2 != 0) return -1;
    return (linha * M + coluna) / 2;
}

int getCasa(Tabuleiro *tab, int linha, int coluna) {
    if (linha < 0 || coluna < 0 || linha >= tab->N || coluna >= tab->M) return -1;
    if ((linha + coluna) % 2 != 0) return -1;
    int idx = coordenadaParaIndice(linha, coluna, tab->N, tab->M);
    return tab->estado[idx];
}

void setCasa(Tabuleiro *tab, int linha, int coluna, int valor) {
    int idx = coordenadaParaIndice(linha, coluna, tab->N, tab->M);
    if (idx >= 0) tab->estado[idx] = valor;
}

// Funções auxiliares para minMovimentos
int getCasaDoEstado(int* estado, int N, int M, int linha, int coluna) {
    if (linha < 0 || coluna < 0 || linha >= N || coluna >= M) return -1;
    if ((linha + coluna) % 2 != 0) return -1;
    int idx = (linha * M + coluna) / 2;
    return estado[idx];
}

void setCasaNoEstado(int* estado, int N, int M, int linha, int coluna, int valor) {
    if (linha < 0 || coluna < 0 || linha >= N || coluna >= M) return;
    if ((linha + coluna) % 2 != 0) return;
    int idx = (linha * M + coluna) / 2;
    estado[idx] = valor;
}

int* clonarEstado(Tabuleiro* tab) {
    int tamanho = (tab->N * tab->M + 1) / 2;
    int* copia = malloc(sizeof(int) * tamanho);
    memcpy(copia, tab->estado, sizeof(int) * tamanho);
    return copia;
}

// Força Bruta
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
                maxTotal = MAX(maxTotal, capturas);
            }
        }
    }
    char *res = malloc(16);
    sprintf(res, "%d\n", maxTotal);
    return res;
}

// Mínimos Movimentos
char* minMovimentos(Tabuleiro* tabuleiro) {
    int maxCaptura = 0;
    typedef struct {
        int i, j;
        int capturas;
        int* estadoCopia;
    } Estado;

    for (int i = 0; i < tabuleiro->N; i++) {
        for (int j = 0; j < tabuleiro->M; j++) {
            if (getCasa(tabuleiro, i, j) != 1) continue;

            int capacidade = 100;
            Estado* pilha = malloc(sizeof(Estado) * capacidade);
            int topo = 0;

            int* estadoInicial = clonarEstado(tabuleiro);
            pilha[topo++] = (Estado){i, j, 0, estadoInicial};

            while (topo > 0) {
                Estado atual = pilha[--topo];
                maxCaptura = MAX(maxCaptura, atual.capturas);

                for (int d = 0; d < 4; d++) {
                    int mi = atual.i + direcoes[d][0];
                    int mj = atual.j + direcoes[d][1];
                    int ni = atual.i + 2 * direcoes[d][0];
                    int nj = atual.j + 2 * direcoes[d][1];

                    if (getCasaDoEstado(atual.estadoCopia, tabuleiro->N, tabuleiro->M, mi, mj) == 2 && 
                        getCasaDoEstado(atual.estadoCopia, tabuleiro->N, tabuleiro->M, ni, nj) == 0) {

                        int* novoEstado = clonarEstado(tabuleiro);
                        memcpy(novoEstado, atual.estadoCopia, sizeof(int) * ((tabuleiro->N * tabuleiro->M + 1) / 2));
                        setCasaNoEstado(novoEstado, tabuleiro->N, tabuleiro->M, atual.i, atual.j, 0);
                        setCasaNoEstado(novoEstado, tabuleiro->N, tabuleiro->M, mi, mj, 0);
                        setCasaNoEstado(novoEstado, tabuleiro->N, tabuleiro->M, ni, nj, 1);

                        if (topo >= capacidade) {
                            capacidade *= 2;
                            pilha = realloc(pilha, sizeof(Estado) * capacidade);
                        }
                        pilha[topo++] = (Estado){ni, nj, atual.capturas + 1, novoEstado};
                    }
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