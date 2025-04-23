#include "algoritmos.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAIOR(a, b) ((a) > (b) ? (a) : (b))

// matriz de deslocamento para movimentos diagonais
int movimentos[4][2] = {
    {-1, -1},
    {-1,  1},
    { 1, -1},
    { 1,  1}
};

// converte as coordenadas em índice do vetor de casas
// retorna -1 se não for uma casa válida
int obterIndiceCasa(int linha, int coluna, int totalLinhas, int totalColunas) {
    if ((linha + coluna) % 2 != 0)
        return -1;
    return (linha * totalColunas + coluna) / 2;
}

// retorna o estado da casa atual
int lerCasa(Tabuleiro *tab, int linha, int coluna) {
    if (linha < 0 || coluna < 0 || linha >= tab->N || coluna >= tab->M)
        return -1;
    if ((linha + coluna) % 2 != 0)
        return -1;
    int indice = obterIndiceCasa(linha, coluna, tab->N, tab->M);
    return tab->estado[indice];
}

// atribui um novo valor a casa selecionada no tabuleiro
void gravarCasa(Tabuleiro *tab, int linha, int coluna, int valor) {
    int indice = obterIndiceCasa(linha, coluna, tab->N, tab->M);
    if (indice >= 0)
        tab->estado[indice] = valor;
}

// lê o valor de uma casa a partir de um vetor de estado
int lerValorCasa(int* estado, int totalLinhas, int totalColunas, int linha, int coluna) {
    if (linha < 0 || coluna < 0 || linha >= totalLinhas || coluna >= totalColunas)
        return -1;
    if ((linha + coluna) % 2 != 0)
        return -1;
    int indice = (linha * totalColunas + coluna) / 2;
    return estado[indice];
}

// grava um valor em uma casa do vetor de estado
void gravarValorCasa(int* estado, int totalLinhas, int totalColunas, int linha, int coluna, int valor) {
    if (linha < 0 || coluna < 0 || linha >= totalLinhas || coluna >= totalColunas)
        return;
    if ((linha + coluna) % 2 != 0)
        return;
    int indice = (linha * totalColunas + coluna) / 2;
    estado[indice] = valor;
}

// cria uma cópia do estado atual do tabuleiro
int* duplicarEstado(Tabuleiro* tab) {
    int total = (tab->N * tab->M + 1) / 2;
    int *copia = malloc(sizeof(int) * total);
    memcpy(copia, tab->estado, sizeof(int) * total);
    return copia;
}

// calcula o máximo de capturas a partir de uma posição
int calcularMaxCapturas(Tabuleiro *tab, int linha, int coluna) {
    int melhorResultado = 0;
    for (int d = 0; d < 4; d++) {
        int midLinha = linha + movimentos[d][0];
        int midColuna = coluna + movimentos[d][1];
        int destLinha = linha + 2 * movimentos[d][0];
        int destColuna = coluna + 2 * movimentos[d][1];

        if (lerCasa(tab, midLinha, midColuna) == 2 && lerCasa(tab, destLinha, destColuna) == 0) {
            // realiza a captura
            gravarCasa(tab, linha, coluna, 0);
            gravarCasa(tab, midLinha, midColuna, 0);
            gravarCasa(tab, destLinha, destColuna, 1);

            int capturas = 1 + calcularMaxCapturas(tab, destLinha, destColuna);
            melhorResultado = MAIOR(melhorResultado, capturas);

            // restaura o estado original 
            gravarCasa(tab, linha, coluna, 1);
            gravarCasa(tab, midLinha, midColuna, 2);
            gravarCasa(tab, destLinha, destColuna, 0);
        }
    }
    return melhorResultado;
}

// usa força bruta para determinar o máximo de capturas possíveis
char* buscaForcaBruta(Tabuleiro* tabuleiro) {
    int capturasMaximas = 0;
    for (int i = 0; i < tabuleiro->N; i++) {
        for (int j = 0; j < tabuleiro->M; j++) {
            if (lerCasa(tabuleiro, i, j) == 1) {
                int capturas = calcularMaxCapturas(tabuleiro, i, j);
                capturasMaximas = MAIOR(capturasMaximas, capturas);
            }
        }
    }
    char *resultado = malloc(16);
    sprintf(resultado, "%d\n", capturasMaximas);
    return resultado;
}

// utiliza uma busca em profundidade com pilha para calcular o mínimo de movimentos a partir de cada peça.
char* buscaMovimentosMinimos(Tabuleiro* tabuleiro) {
    int capturasMaxEncontradas = 0;
    
    typedef struct {
        int linha, coluna;
        int capturas;
        int* copiaEstado;
    } Estado;
    
    for (int i = 0; i < tabuleiro->N; i++) {
        for (int j = 0; j < tabuleiro->M; j++) {
            if (lerCasa(tabuleiro, i, j) != 1)
                continue;
            
            int capacidade = 100;
            Estado *pilha = malloc(sizeof(Estado) * capacidade);
            int topo = 0;
            
            int* estadoInicial = duplicarEstado(tabuleiro);
            pilha[topo++] = (Estado){ i, j, 0, estadoInicial };
            
            while (topo > 0) {
                Estado atual = pilha[--topo];
                capturasMaxEncontradas = MAIOR(capturasMaxEncontradas, atual.capturas);
                
                for (int d = 0; d < 4; d++) {
                    int saltoLinha = atual.linha + movimentos[d][0];
                    int saltoColuna = atual.coluna + movimentos[d][1];
                    int novoLinha  = atual.linha + 2 * movimentos[d][0];
                    int novoColuna = atual.coluna + 2 * movimentos[d][1];
                    
                    if (lerValorCasa(atual.copiaEstado, tabuleiro->N, tabuleiro->M, saltoLinha, saltoColuna) == 2 &&
                        lerValorCasa(atual.copiaEstado, tabuleiro->N, tabuleiro->M, novoLinha, novoColuna) == 0) {
                        
                        int* novoEstado = duplicarEstado(tabuleiro);
                        memcpy(novoEstado, atual.copiaEstado, sizeof(int) * ((tabuleiro->N * tabuleiro->M + 1) / 2));
                        gravarValorCasa(novoEstado, tabuleiro->N, tabuleiro->M, atual.linha, atual.coluna, 0);
                        gravarValorCasa(novoEstado, tabuleiro->N, tabuleiro->M, saltoLinha, saltoColuna, 0);
                        gravarValorCasa(novoEstado, tabuleiro->N, tabuleiro->M, novoLinha, novoColuna, 1);
                        
                        if (topo >= capacidade) {
                            capacidade *= 2;
                            pilha = realloc(pilha, sizeof(Estado) * capacidade);
                        }
                        pilha[topo++] = (Estado){ novoLinha, novoColuna, atual.capturas + 1, novoEstado };
                    }
                }
                free(atual.copiaEstado);
            }
            free(pilha);
        }
    }
    
    char* resultado = malloc(16);
    sprintf(resultado, "%d\n", capturasMaxEncontradas);
    return resultado;
}