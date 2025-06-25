#ifndef ALG_PARTE2_H
#define ALG_PARTE2_H

#include <stdio.h>
#include "IO.h"

#define TAMANHO_ALFABETO 256

// Estrutura para representar um nó da árvore de Huffman
typedef struct NoHuffman {
    unsigned char caractere;
    unsigned long frequencia;
    struct NoHuffman *esquerda;
    struct NoHuffman *direita;
} NoHuffman;

ResultadoParte2 BoyerMooreHorspool(char *texto, int tam_texto, char *padrao, int tam_padrao);

// Comprime os dados e retorna os dados comprimidos em `dados_comprimidos` e seu tamanho em `tamanho_comprimido`.
void ComprimirHuffman(const unsigned char *dados, size_t tamanho, unsigned char **dados_comprimidos, size_t *tamanho_comprimido);

// Busca de bytes para busca em dados comprimidos
long long buscar_comprimido(const unsigned char *texto, size_t tam_texto, const unsigned char *padrao, size_t tam_padrao);

#endif