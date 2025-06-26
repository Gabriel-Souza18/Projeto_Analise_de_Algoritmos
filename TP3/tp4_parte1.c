#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "IO.h"
#include "alg_parte1.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <algoritmo> <arquivo texto> <arquivo padroes>\n", argv[0]);
        printf("Algoritmo: 1 para Programação Dinâmica, 2 para Shift-And\n");
        return 1;
    }

    int id_algoritmo = atoi(argv[1]);
    char *arquivoTexto = argv[2];
    char *arquivoPadroes = argv[3];

    char *texto = lerArquivo(arquivoTexto);
    char *padroes_brutos = lerArquivo(arquivoPadroes);
    if (!texto || !padroes_brutos) {
        return 1;
    }
    
    int tamanho_texto = strlen(texto);
    char *nome_algoritmo = (id_algoritmo == 1) ? "ProgramacaoDinamica" : "ShiftAnd";

    printf("Executando Parte 1 com o algoritmo: %s\n", nome_algoritmo);
    printf("Texto: %s (%d caracteres)\n", arquivoTexto, tamanho_texto);
    printf("Padrões: %s\n\n", arquivoPadroes);

    // Prepara arquivos de saída
    char nome_arq_saida[MAX_NOME_ARQUIVO];
    snprintf(nome_arq_saida, sizeof(nome_arq_saida), "saida_%s_ocorrencias.txt", nome_algoritmo);
    FILE* arq_ocorrencias = fopen(nome_arq_saida, "w");

    printf("%-20s | k | Tempo (s)    | Comparações\n", "Padrão");
    printf("----------------------------------------------------------\n");

    char *copia_padroes = strdup(padroes_brutos);
    char *padrao = strtok(copia_padroes, "\n\r");

    while(padrao != NULL) {
        int tamanho_padrao = strlen(padrao);
        if (tamanho_padrao == 0) {
            padrao = strtok(NULL, "\n\r");
            continue;
        }
        
        for (int k = 0; k <= 3; k++) {
            ResultadoParte1 resultado;
            if (id_algoritmo == 1) {
                resultado = ProgramacaoDinamica(texto, tamanho_texto, padrao, tamanho_padrao, k);
            } else {
                resultado = ShiftAnd(texto, tamanho_texto, padrao, tamanho_padrao, k);
            }

            printf("%-20s | %d | %-12.6f | %-12lld\n", padrao, k, resultado.tempo, resultado.num_comparacoes);
            
            // Escreve as ocorrências uma vez para evitar duplicatas na saída
            // A busca com mais erros (k=3) conterá todas as ocorrências das buscas com menos erros
            if(k == 3) {
                 fprintf(arq_ocorrencias, "%s", resultado.saida_ocorrencias);
            }

            free(resultado.saida_ocorrencias);
        }
        printf("----------------------------------------------------------\n");
        padrao = strtok(NULL, "\n\r");
    }

    fclose(arq_ocorrencias);
    printf("\nResultados de ocorrências (para k=3) salvos em: %s\n", nome_arq_saida);
    
    free(copia_padroes);
    free(padroes_brutos);
    free(texto);

    return 0;
}