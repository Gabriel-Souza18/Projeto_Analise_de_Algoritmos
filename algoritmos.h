#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "tabuleiro.h"

 // explora recursivamente todas as possibilidades de movimentp 
 // para conseguir capturar a maior quantidade de peças possíveis
char* buscaForcaBruta(Tabuleiro* tabuleiro);
 // utiliza pilha para explorar as possibilidades de movimento
char* buscaMovimentosMinimos(Tabuleiro* tabuleiro);

#endif