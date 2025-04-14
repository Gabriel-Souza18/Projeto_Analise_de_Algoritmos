#include "io.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char* lerEntrada(char *nomeEntrada){
    char *conteudo = NULL;
    int tamanho = 0;
    int ler = 1;

    FILE *arquivo = fopen(nomeEntrada, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada");
        return NULL;
    }
    int i =0;
    while(ler){
     
        char linha[200];
        fgets(linha, sizeof(linha), arquivo); 



        // Verifica se a linha é "0 0\n" ou vazia
        if (strcmp(linha, "0 0") == 0 || strcmp(linha, "\0") == 0) {
            break;
        }

        int tamanho_linha = strlen(linha);
        conteudo = realloc(conteudo, tamanho + tamanho_linha + 1);

        if (conteudo == NULL) {
            printf("Erro ao alocar memória para o conteúdo do arquivo");
            fclose(arquivo);
            return NULL;
        }
        strcpy(conteudo + tamanho, linha);
        tamanho += tamanho_linha;
        if( i == 10 ){
            ler = 0;
        }
        i++;
    }
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