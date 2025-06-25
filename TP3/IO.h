#ifndef IO_H
#define IO_H

#include <stdlib.h>

#define MAX_IO_BUFFER_SIZE 4096
#define MAX_NOME_ARQUIVO 256
#define MAX_LINHA_SAIDA 8192

// Estrutura para armazenar os resultados da parte 1
typedef struct {
    double tempo;
    long long num_comparacoes;
    char *saida_ocorrencias; 
} ResultadoParte1;

// Estrutura para armazenar os resultados da parte 2
typedef struct {
    double tempo_nao_comprimido;
    long long comps_nao_comprimido;
    double tempo_comprimido;
    long long comps_comprimido;
    char *saida_ocorrencias;
} ResultadoParte2;

// Funções de leitura e escrita
char* lerArquivo(const char *nomeArquivo);
void escreverArquivo(const char *nomeArquivo, const char* conteudo);

#endif