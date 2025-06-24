#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IO.h"

void main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <algoritmo> <arquivo texto> <arquivo padroes>\n", argv[0]);
        return;
    }

    char *algoritmo = argv[1];
    char *arquivoTexto = argv[2];
    char *arquivoPadroes = argv[3];

    char *conteudo = lerEntrada(arquivoTexto);
    char *padroesConteudo = lerEntrada(arquivoPadroes);

    if(algoritmo == "1"  ){
        printf("Algoritmo Programação Dinamica\n");
    } else {
        printf("Algoritmo Shift-And\n");
    }

    printf("Conteudo do arquivo de texto:\n%s\n", conteudo);
    printf("Conteudo do arquivo de padroes:\n%s\n", padroesConteudo);


    free(padroesConteudo);
    free(conteudo);
}
