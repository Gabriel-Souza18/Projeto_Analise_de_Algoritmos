#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "IO.h"
#include "alg_parte2.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <arquivo texto> <arquivo padroes>\n", argv[0]);
        return 1;
    }

    char *arquivoTexto = argv[1];
    char *arquivoPadroes = argv[2];

    // Lê os arquivos de texto e padrões
    char *texto = lerArquivo(arquivoTexto);
    char *padroes_brutos = lerArquivo(arquivoPadroes);
    if (!texto || !padroes_brutos) {
        return 1;
    }
    
    int tam_texto = strlen(texto);
    
    printf("Executando Parte 2: BMH (Não Comprimido) vs Busca (Comprimido)\n");
    printf("Texto: %s (%d caracteres)\n", arquivoTexto, tam_texto);
    printf("Padrões: %s\n\n", arquivoPadroes);

    // Compressão de Huffman do texto inteiro 
    unsigned char *texto_comprimido = NULL;
    size_t tamanho_texto_comprimido = 0;
    clock_t inicio_huffman = clock();
    ComprimirHuffman((unsigned char*)texto, tam_texto, &texto_comprimido, &tamanho_texto_comprimido);
    double tempo_huffman = ((double)(clock() - inicio_huffman)) / CLOCKS_PER_SEC;
    
    printf("Compressão Huffman:\n");
    printf(" - Tempo de compressão: %.6f s\n", tempo_huffman);
    if (tamanho_texto_comprimido > 0) {
        printf(" - Tamanho Original: %d bytes\n", tam_texto);
        printf(" - Tamanho Comprimido: %zu bytes\n", tamanho_texto_comprimido);
        printf(" - Taxa de Compressão: %.2f%%\n\n", 100.0 * (1.0 - (double)tamanho_texto_comprimido / tam_texto));
    } else {
        printf(" - Compressão falhou ou texto vazio.\n\n");
    }

    // Abre o arquivo de saída
    char nome_arq_saida[MAX_NOME_ARQUIVO];
    snprintf(nome_arq_saida, sizeof(nome_arq_saida), "saida_parte2_ocorrencias.txt");
    FILE* arq_ocorrencias = fopen(nome_arq_saida, "w");

    printf("%-15s | %-25s | %-25s\n", "Padrão", "Busca Não-Comprimida (BMH)", "Busca Comprimida (Byte-Search)");
    printf("%-15s | %-12s | %-12s | %-12s | %-12s\n", "", "Tempo (s)", "Comparações", "Tempo (s)", "Comparações");
    printf("-----------------------------------------------------------------------------------------\n");

    char *copia_padroes = strdup(padroes_brutos);
    char *padrao = strtok(copia_padroes, "\n\r");

    while(padrao != NULL) {
        int tam_padrao = strlen(padrao);
        if (tam_padrao == 0) {
            padrao = strtok(NULL, "\n\r");
            continue;
        }

        // Busca com BMH no texto não comprimido
        ResultadoParte2 resultado = BoyerMooreHorspool(texto, tam_texto, padrao, tam_padrao);
        fprintf(arq_ocorrencias, "%s", resultado.saida_ocorrencias);

        unsigned char *padrao_comprimido = NULL;
        size_t tamanho_padrao_comprimido = 0;
        ComprimirHuffman((unsigned char*)padrao, tam_padrao, &padrao_comprimido, &tamanho_padrao_comprimido);
        
        // Busca direta por bytes no texto comprimido 
        clock_t inicio_busca_comprimida = clock();
        long long comparacoes_comprimido = 0;
        if (texto_comprimido && padrao_comprimido) {
            comparacoes_comprimido = buscar_comprimido(texto_comprimido, tamanho_texto_comprimido, padrao_comprimido, tamanho_padrao_comprimido);
        }
        double tempo_busca_comprimida = ((double)(clock() - inicio_busca_comprimida)) / CLOCKS_PER_SEC;

        printf("%-15s | %-12.6f | %-12lld | %-12.6f | %-12lld\n", 
            padrao, 
            resultado.tempo_nao_comprimido, resultado.comps_nao_comprimido,
            tempo_busca_comprimida, comparacoes_comprimido);
        
        // Liberação de memória da iteração atual
        free(resultado.saida_ocorrencias);
        free(padrao_comprimido);
        
        padrao = strtok(NULL, "\n\r");
    }
    
    printf("-----------------------------------------------------------------------------------------\n");

    fclose(arq_ocorrencias);
    printf("\nResultados de ocorrências salvos em: %s\n", nome_arq_saida);

    // Liberação final
    free(copia_padroes);
    free(padroes_brutos);
    free(texto);
    free(texto_comprimido);

    return 0;
}
