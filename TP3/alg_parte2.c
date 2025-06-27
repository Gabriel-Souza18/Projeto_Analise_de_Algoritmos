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
