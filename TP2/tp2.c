#include "IO.h"
#include "Caminhos.h"
#include "Solucao.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // Para sscanf e snprintf

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }
    
    char *entrada_filename = argv[1];
    // CORRIGIDO: Chamada de lerEntrada sem o segundo argumento '1024'
    char *conteudo_arquivo = lerEntrada(entrada_filename);
    
    char *ptr = conteudo_arquivo;

    int instancias = 0;
    int numPovos = 0;
    int pesoMaximo = 0;
    int distanciaMaxima = 0;
    int numCaminhos = 0;
    int lidos = 0;

    if (conteudo_arquivo == NULL) {
        printf("Erro ao ler o arquivo de entrada\n");
        return 1;
    }

    // Lê o número de instâncias
    sscanf(ptr, "%d%n", &instancias, &lidos);
    ptr += lidos;

    for (int i = 0; i < instancias; i++) {
        printf("--- Nova Instancia %d ---\n", i + 1);

        char instanciaStr[20];
        sprintf(instanciaStr, "%d", i + 1);

        // Inicializa as structs de Povos e Caminhos para cada instância
        // É crucial inicializar 'capacidade' como 0 para o 'adicionarPovo/Caminho' funcionar corretamente na primeira alocação.
        Povos povos = { .povos = NULL, .numPovos = 0, .capacidade = 0 }; 
        Caminhos caminhos = { .caminhos = NULL, .numCaminhos = 0, .capacidade = 0 }; 

        // Lê os parâmetros da instância
        sscanf(ptr, "%d %d %d %d%n", &numPovos, &distanciaMaxima, &pesoMaximo, &numCaminhos, &lidos);
        ptr += lidos;

        // Lê e adiciona os povos
        for (int j = 0; j < numPovos; j++) {
            int id, peso, habilidade;
            sscanf(ptr, "%d %d %d%n", &id, &peso, &habilidade, &lidos);
            ptr += lidos;
            Povo *povo = criarPovo(id, peso, habilidade);
            if (povo) { // Verifica se a criação do povo foi bem-sucedida
                adicionarPovo(&povos, povo);
                free(povo); // CORRIGIDO: Libera a memória do povo individual após o conteúdo ser copiado
            }
        }

        // Lê e adiciona os caminhos
        for (int j = 0; j < numCaminhos; j++) {
            int origem, destino, distancia;
            sscanf(ptr, "%d %d %d%n", &origem, &destino, &distancia, &lidos);
            ptr += lidos;
            // CORRIGIDO: Função criarCaminho (singular)
            Caminho *caminho = criarCaminho(origem, destino, distancia);
            if (caminho) { // Verifica se a criação do caminho foi bem-sucedida
                adicionarCaminho(&caminhos, caminho);
                free(caminho); // CORRIGIDO: Libera a memória do caminho individual após o conteúdo ser copiado
            }
        }

        // --- Execução da Heurística ---
        clock_t t0 = clock();
        Resultado *resultadoH = resolverComHeuristica(&povos, &caminhos, distanciaMaxima, pesoMaximo);
        clock_t t1 = clock();
        double tempoH = (double)(t1 - t0) / CLOCKS_PER_SEC; // Tempo em segundos

        printf("Tempo Heurística: %f ms \n", tempoH * 1e3);
        if (resultadoH) {
            printf("Habilidade Total (Heurística): %d\n", resultadoH->habilidadeTotal);
        } else {
            printf("Erro: resultadoH nulo para Heurística.\n");
        }
        
        char nomeArquivoH[MAX_FILENAME_LENGTH]; 
        sprintf(nomeArquivoH, "SaidaHeuristica%s.txt", instanciaStr);
        char *saida_H = gerarSaida(resultadoH);
        escreverSaida(nomeArquivoH, saida_H);
        free(saida_H); // Libera a string de saída gerada por gerarSaida

        // --- Execução da Programação Dinâmica ---
        clock_t t2 = clock();
        Resultado *resultadoPD = resolverComPD(&povos, &caminhos, distanciaMaxima, pesoMaximo);
        clock_t t3 = clock();
        double tempoPD = (double)(t3 - t2) / CLOCKS_PER_SEC;

        printf("Tempo PD: %f ms\n", tempoPD * 1e3);
        if (resultadoPD) {
            printf("Habilidade Total (PD): %d\n", resultadoPD->habilidadeTotal);
        } else {
            printf("Erro: resultadoPD nulo para PD.\n");
        }
        
        char nomeArquivoPD[MAX_FILENAME_LENGTH]; 
        sprintf(nomeArquivoPD, "SaidaPD%s.txt", instanciaStr);
        char *saida_PD = gerarSaida(resultadoPD);
        escreverSaida(nomeArquivoPD, saida_PD);
        free(saida_PD); // Libera a string de saída gerada por gerarSaida

        // --- Liberação de Memória para a Instância Atual ---
        liberarResultado(resultadoH); // Libera a struct Resultado e seus arrays internos
        liberarResultado(resultadoPD);
        
        destruirCaminhos(&caminhos); // Libera o array de Caminhos
        destruirPovos(&povos); // Libera o array de Povos

        printf("--- Fim da Instância %d ---\n", i + 1);
        printf("\n______________________\n\n");
    }

    free(conteudo_arquivo); // Libera o buffer de conteúdo lido do arquivo
    return 0;
}