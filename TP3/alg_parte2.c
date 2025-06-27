#include "alg_parte2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Preenche o vetor de deslocamentos para o algoritmo BMH
static void pre_calcular_deslocamentos(const char *padrao, int tam_padrao, int deslocamentos[TAMANHO_ALFABETO]) {
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        deslocamentos[i] = tam_padrao; 
    }
    for (int i = 0; i < tam_padrao - 1; i++) {
        deslocamentos[(unsigned char)padrao[i]] = tam_padrao - 1 - i;
    }
}

// Algoritmo BMH para busca exata
ResultadoParte2 BoyerMooreHorspool(char *texto, int tam_texto, char *padrao, int tam_padrao) {
    ResultadoParte2 resultado = {0};
    clock_t inicio = clock();

    // Verifica se o padrão é válido
    if (tam_padrao > tam_texto || tam_padrao == 0) {
        resultado.saida_ocorrencias = strdup("");
        return resultado;
    }

    int deslocamentos[TAMANHO_ALFABETO];
    pre_calcular_deslocamentos(padrao, tam_padrao, deslocamentos);

    char *string_ocorrencias = (char *)malloc(MAX_LINHA_SAIDA);
    sprintf(string_ocorrencias, "%s", padrao); 

    int i = 0;
    while (i <= tam_texto - tam_padrao) {
        int j = tam_padrao - 1;
        // Verifica se o padrão casa com a posição atual
        while (j >= 0) {
            resultado.comps_nao_comprimido++;
            if (padrao[j] != texto[i + j]) {
                break;
            }
            j--;
        }
        // Se casou, registra a ocorrência
        if (j < 0) {
            char temp[32];
            sprintf(temp, " %d", i + 1); 
            strcat(string_ocorrencias, temp);
        }
        i += deslocamentos[(unsigned char)texto[i + tam_padrao - 1]];
    }

    strcat(string_ocorrencias, "\n");
    resultado.saida_ocorrencias = string_ocorrencias;
    resultado.tempo_nao_comprimido = ((double)(clock() - inicio)) / CLOCKS_PER_SEC;
    return resultado;
}

typedef struct NoFilaPrioridade {
    NoHuffman *no_huffman;
    struct NoFilaPrioridade *proximo;
} NoFilaPrioridade;

static NoHuffman* criar_no_huffman(unsigned char caractere, unsigned long frequencia) {
    NoHuffman* no = (NoHuffman*)malloc(sizeof(NoHuffman));
    no->caractere = caractere;
    no->frequencia = frequencia;
    no->esquerda = no->direita = NULL;
    return no;
}

// Insere um nó na fila de prioridade ordenada por frequência
static void fila_inserir(NoFilaPrioridade **cabeca, NoHuffman *no_huffman) {
    NoFilaPrioridade *novo_no = (NoFilaPrioridade*)malloc(sizeof(NoFilaPrioridade));
    novo_no->no_huffman = no_huffman;
    if (*cabeca == NULL || (*cabeca)->no_huffman->frequencia >= no_huffman->frequencia) {
        novo_no->proximo = *cabeca;
        *cabeca = novo_no;
    } else {
        NoFilaPrioridade *atual = *cabeca;
        while (atual->proximo != NULL && atual->proximo->no_huffman->frequencia < no_huffman->frequencia) {
            atual = atual->proximo;
        }
        novo_no->proximo = atual->proximo;
        atual->proximo = novo_no;
    }
}

// Remove e retorna o nó com menor frequência da fila
static NoHuffman* fila_remover(NoFilaPrioridade **cabeca) {
    if (*cabeca == NULL) return NULL;
    NoFilaPrioridade *temp = *cabeca;
    NoHuffman *no_huffman = temp->no_huffman;
    *cabeca = (*cabeca)->proximo;
    free(temp);
    return no_huffman;
}

// Gera códigos binários recursivamente a partir da árvore de Huffman
static void gerar_codigos_recursivo(NoHuffman* raiz, char *codigos[TAMANHO_ALFABETO], char *codigo, int topo) {
    if (raiz->esquerda) {
        codigo[topo] = '0';
        gerar_codigos_recursivo(raiz->esquerda, codigos, codigo, topo + 1);
    }
    if (raiz->direita) {
        codigo[topo] = '1';
        gerar_codigos_recursivo(raiz->direita, codigos, codigo, topo + 1);
    }
    if (!raiz->esquerda && !raiz->direita) {
        codigo[topo] = '\0';
        codigos[raiz->caractere] = strdup(codigo);
    }
}
void ComprimirHuffman(const unsigned char *dados, size_t tamanho, unsigned char **dados_comprimidos, size_t *tamanho_comprimido) {
    unsigned long frequencias[TAMANHO_ALFABETO] = {0};
    for(size_t i = 0; i < tamanho; i++) frequencias[dados[i]]++;

    NoFilaPrioridade *fila_p = NULL;
    for(int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (frequencias[i] > 0) {
            fila_inserir(&fila_p, criar_no_huffman((unsigned char)i, frequencias[i]));
        }
    }
    
    if (fila_p == NULL) { *dados_comprimidos = NULL; *tamanho_comprimido = 0; return; }
    
    while(fila_p->proximo != NULL) {
        NoHuffman *esquerda = fila_remover(&fila_p);
        NoHuffman *direita = fila_remover(&fila_p);
        NoHuffman *no_interno = criar_no_huffman('$', esquerda->frequencia + direita->frequencia);
        no_interno->esquerda = esquerda;
        no_interno->direita = direita;
        fila_inserir(&fila_p, no_interno);
    }
    
    NoHuffman *raiz = fila_remover(&fila_p);

    char *codigos[TAMANHO_ALFABETO] = {0};
    char codigo[256];
    gerar_codigos_recursivo(raiz, codigos, codigo, 0);

    size_t tamanho_saida_bits = 0;
    for (size_t i = 0; i < tamanho; i++) tamanho_saida_bits += strlen(codigos[dados[i]]);
    
    *tamanho_comprimido = (tamanho_saida_bits + 7) / 8;
    *dados_comprimidos = (unsigned char*)calloc(*tamanho_comprimido, sizeof(unsigned char));

    size_t indice_byte = 0;
    int indice_bit = 0;
    for (size_t i = 0; i < tamanho; i++) {
        char *c = codigos[dados[i]];
        for (int j = 0; c[j] != '\0'; j++) {
            if (c[j] == '1') (*dados_comprimidos)[indice_byte] |= (1 << (7 - indice_bit));
            indice_bit++;
            if (indice_bit == 8) { indice_bit = 0; indice_byte++; }
        }
    }
}

// Busca de bytes
long long buscar_comprimido(const unsigned char *texto, size_t tam_texto, const unsigned char *padrao, size_t tam_padrao) {
    long long comparacoes = 0;
    if (tam_padrao == 0 || tam_padrao > tam_texto) return 0;

    for(size_t i = 0; i <= tam_texto - tam_padrao; i++) {
        int encontrou = 1;
        for(size_t j = 0; j < tam_padrao; j++) {
            comparacoes++;
            if (texto[i+j] != padrao[j]) {
                encontrou = 0;
                break;
            }
        }
        if (encontrou) {
        
        }
    }
    return comparacoes;
}