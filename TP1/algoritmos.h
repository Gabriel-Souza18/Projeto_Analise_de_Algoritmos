#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "tabuleiro.h"

// Explora recursivamente todas as possibilidades de movimento 
char* buscaForcaBruta(Tabuleiro* tabuleiro);

// Usa pilha - melhor para casos com menos peças
// Testa se a ordem dos movimentos afeta o resultado
char* buscaMovimentosMinimos(Tabuleiro* tabuleiro);

#endif