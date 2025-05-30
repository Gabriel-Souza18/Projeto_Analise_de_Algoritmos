#include "IO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *lerEntrada(char *entrada) {
    FILE *arquivo = fopen(entrada, "r");
    if(!arquivo){
        perror("Erro ao abrir o arquivo de entrada");
        return NULL;
    }

    char *conteudo = NULL;
    size_t tamanhoTotal = 0;
    char buffer[MAX_IO_BUFFER_SIZE]; // buffer para leitura

    while(fgets(buffer, sizeof(buffer), arquivo) != NULL){
        if (strcmp(buffer, "0 0\n") == 0){
            break;
        }
        if (buffer[0] == '\n' || buffer[0] == '\0'){
            continue;
        }

        size_t len = strlen(buffer);
        // realoca a memória pra nova linha
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

void escreverSaida(char *saida, char *conteudo) {
    // verifica se o conteúdo é nulo 
    if (!conteudo) {
        fprintf(stderr, "Conteúdo de saída nulo para o arquivo: %s\n", saida);
        return;
    }

    FILE *arquivo = fopen(saida, "w"); 
    if (!arquivo){
        perror("Erro ao abrir o arquivo de saída");
        return;
    }
    fprintf(arquivo, "%s", conteudo);
    fclose(arquivo);
}