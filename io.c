#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// lê o conteúdo de um arquivo de entrada e retorna como uma string
// a leitura é interrompida quando encontra a linha "0 0\n" ou quando não há mais dados
char* lerEntrada(char *nomeEntrada){
    FILE *arquivo = fopen(nomeEntrada, "r");
    if(!arquivo){
        perror("Erro ao abrir o arquivo de entrada");
        return NULL;
    }
    char *conteudo = NULL;
    size_t tamanhoTotal = 0;
    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), arquivo) != NULL){
        if (strcmp(buffer, "0 0\n") == 0){
            break;
        }
        // pula linhas vazias
        if (buffer[0] == '\n' || buffer[0] == '\0'){
            continue;
        }
        size_t len = strlen(buffer);
        char *temp = realloc(conteudo, tamanhoTotal + len + 1);
        if (!temp){
            free(conteudo);
            fclose(arquivo);
            perror("Erro ao realocar memória");
            return NULL;
        }
        conteudo = temp;
        strcpy(conteudo + tamanhoTotal, buffer);
        tamanhoTotal += len;
    }
    fclose(arquivo);
    return conteudo;
}

// escreve o conteúdo em um arquivo de saída
void escreverSaida(char *nomeSaida, char *conteudo){
    FILE *arquivo = fopen(nomeSaida, "w");
    if (!arquivo){
        perror("Erro ao abrir o arquivo de saída");
        return;
    }
    fprintf(arquivo, "%s", conteudo);
    fclose(arquivo);
}