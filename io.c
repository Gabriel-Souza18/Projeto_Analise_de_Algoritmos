#include "io.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* lerEntrada(char *nomeEntrada) {
    char *conteudo = NULL;
    int tamanho = 0;

    FILE *arquivo = fopen(nomeEntrada, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return NULL;
    }

    while (1) {
        char linha[256];
        if (!fgets(linha, sizeof(linha), arquivo)) break;
        if (strcmp(linha, "0 0\n") == 0) break;
        if (strlen(linha) == 0 || linha[0] == '\n') continue;

        int len = strlen(linha);
        conteudo = realloc(conteudo, tamanho + len + 1);
        strcpy(conteudo + tamanho, linha);
        tamanho += len;
    }

    fclose(arquivo);
    return conteudo;
}

void escreverSaida(char *nomeSaida, char *conteudo) {
    FILE *arquivo = fopen(nomeSaida, "w");
    fprintf(arquivo, "%s", conteudo);
    fclose(arquivo);
}