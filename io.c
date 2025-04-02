#include "io.h"

#include <stdio.h>
#include <stdlib.h>


char* lerEntrada(char *nomeEntrada){
    FILE *arquivo = fopen(nomeEntrada, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return NULL;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    char *conteudo = (char *)malloc(tamanho + 1);
    if (conteudo == NULL) {
        perror("Erro ao alocar memória");
        fclose(arquivo);
        return NULL;
    }

    fread(conteudo, 1, tamanho, arquivo);
    conteudo[tamanho] = '\0';

    fclose(arquivo);
    return conteudo;
}
void escreverSaida(char *nomeSaida, char *conteudo){
    FILE *arquivo = fopen(nomeSaida, "w");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    }

    fprintf(arquivo, "%s", conteudo);
    fclose(arquivo);
}