#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "IO.h"
#include "alg_parte1.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <algoritmo> <arquivo texto> <arquivo padroes>\n", argv[0]);
        return 0;
    }

    char *algoritmo = argv[1];
    char *arquivoTexto = argv[2];
    char *arquivoPadroes = argv[3];

    char *conteudo = lerEntrada(arquivoTexto);
    char *padroesConteudo = lerEntrada(arquivoPadroes);

    ResultadoParte1 resultados[4];

    if(strcmp(algoritmo, "1") == 0) {
        printf("Algoritmo Programação Dinamica\n");
        
        for(int i =0 ; i < 4; i++){
            resultados[i].saida = ProgramacaoDinamica(conteudo, padroesConteudo);
        }

        algoritmo = "ProgramacaoDinamica";
    } else if(strcmp(algoritmo, "2") == 0) {
        printf("Algoritmo ShiftAnd\n");
        
        for(int i =0 ; i < 4; i++){
            resultados[i].saida =  ShiftAnd(conteudo, padroesConteudo);
        }
        algoritmo = "ShiftAnd";
    }
    else {
        printf("Algoritmo desconhecido. Use 1 para Programação Dinâmica ou 2 para ShiftAnd.\n");
        return 0;
    }

    printf("Conteudo do arquivo de texto:\n%s\n", conteudo);
    printf("Conteudo do arquivo de padroes:\n%s\n", padroesConteudo);


    
    char arq_saida[256];
    snprintf(arq_saida, sizeof(arq_saida), "saida_%s.txt", algoritmo);
    
    char * resultado = (char *)malloc(1024 * sizeof(char)); 
    for (int i=0; i<4 ; i++){
        strcat(resultado, resultados[i].saida);
    }
    escreverSaida(arq_saida, resultado);

    
  
    free(padroesConteudo);
    free(conteudo);

    return 0;

}
