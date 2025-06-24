#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IO.h"
#include "alg_parte2.h"

int main(int argc, char *argv[]){
    if (argc < 3){
        printf("Uso: %s <arquivo texto> <arquivo padroes>\n", argv[0]);
        return 0;
    }   

    char *arquivoTexto = argv[1];
    char *arquivoPadroes = argv[2];

    char *conteudo = lerEntrada(arquivoTexto);
    char *padroesConteudo = lerEntrada(arquivoPadroes);

    char *Comprimido = Huffman(conteudo, padroesConteudo);

    ResultadoParte2 resultado;
    resultado.saida = BoyerMoore(conteudo, padroesConteudo);

    printf("Conteudo do arquivo de texto:\n%s\n", conteudo);
    printf("Conteudo do arquivo de padroes:\n%s\n", padroesConteudo);
    printf("Resultado do Huffman:\n%s\n", Comprimido);
    printf("Resultado do Boyer-Moore:\n%s\n", resultado.saida);   
    
    printf("Tempo de execução : %.2f segundos\n", resultado.tempo);
    escreverSaida("saida_Parte2.txt", resultado.saida);


}