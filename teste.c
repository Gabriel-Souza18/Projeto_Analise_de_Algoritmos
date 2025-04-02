#include "io.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
    // Teste de leitura
    char *entrada = lerEntrada("entrada.txt");
    if (entrada != NULL) {
        printf("Conteudo do arquivo de entrada:\n%s\n", entrada);
        free(entrada);
    } else {
        printf("Erro ao ler o arquivo de entrada.\n");
    }
    
    // Teste de escrita
    char* saida= "1\n2\n7";
    escreverSaida("saida.txt", saida);
    printf("Arquivo de saída escrito com sucesso.\n");
    free(entrada);

return 0;
}