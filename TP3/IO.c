#include "IO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Lê um arquivo inteiro para uma string alocada dinamicamente.
char *lerArquivo(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de entrada");
        return NULL;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    char *conteudo = (char *)malloc(tamanho + 1);
    if (!conteudo) {
        perror("Erro ao alocar memória para o conteúdo do arquivo");
        fclose(arquivo);
        return NULL;
    }

    if (fread(conteudo, 1, tamanho, arquivo) != (size_t)tamanho) {
        fprintf(stderr, "Erro ao ler o arquivo.\n");
        free(conteudo);
        fclose(arquivo);
        return NULL;
    }
    
    conteudo[tamanho] = '\0';
    fclose(arquivo);
    return conteudo;
}

// Escreve uma string em um arquivo.
void escreverArquivo(const char *nomeArquivo, const char *conteudo) {
    if (!conteudo) {
        fprintf(stderr, "Conteúdo de saída nulo para o arquivo: %s\n", nomeArquivo);
        return;
    }

    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    }
    fprintf(arquivo, "%s", conteudo);
    fclose(arquivo);
}