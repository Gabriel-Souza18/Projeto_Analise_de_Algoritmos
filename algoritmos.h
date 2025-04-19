#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "tabuleiro.h"

/*
 * Força Bruta: Testa todas as possibilidades recursivamente.
 * Complexidade: O(4^n), onde n é o número máximo de capturas.
 */
char* forcaBruta(Tabuleiro* tabuleiro);

/*
 * Mínimos Movimentos: Usa pilha para otimizar a busca.
 * Complexidade: O(f^p), onde f é o fator de ramificação e p é a profundidade máxima de busca.
 */
char* minMovimentos(Tabuleiro* tabuleiro);

#endif