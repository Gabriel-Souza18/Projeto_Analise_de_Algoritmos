#include "IO.h"
#include "Caminhos.h"
#include "Solucao.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void main(){
    char *entrada = "entrada.txt";
    char *saida = "saida.txt";
    char *conteudo = lerEntrada(entrada, 1024);
    char *ptr = conteudo;


    int instacias = 0;
    int numPovos = 0;
    int pesoMaximo = 0;
    int distanciaMaxima = 0;
    int numCaminhos = 0;

    int lidos =0;

    if (conteudo == NULL){
        printf( "Erro ao ler o arquivo de entrada\n");
        return;
    }




    while(*ptr != '\0'){
        sscanf(ptr, "%d%n", &instacias, &lidos);
        ptr += lidos;
    
        for (int i =0 ;i<instacias; i++){
            printf("Nova Instancia\n");

            char instanciaStr[20];
            sprintf(instanciaStr, "%d", i+1);
            
            Povos    povos    = { 
                .povos = NULL,    
                .numPovos    = 0 };

            Caminhos caminhos = { 
                .caminhos = NULL,
                 .numCaminhos = 0 };
            

            sscanf(ptr, "%d %d %d %d%n",
                &numPovos, &distanciaMaxima, &pesoMaximo, &numCaminhos, &lidos);
                ptr += lidos;
            for (int j=0;j<numPovos;j++){
                int id, peso, habilidade;
                sscanf(ptr, "%d %d %d%n", &id, &peso, &habilidade, &lidos);
                ptr += lidos;
                Povo *povo = criarPovo(id, peso, habilidade);
                if (povo == NULL){
                    printf("Erro ao criar o povo\n");
                    return;
                }
                adicionarPovo(&povos, povo);
            }

            for(int j=0;j<numCaminhos;j++){
                int origem, destino, distancia;
                sscanf(ptr, "%d %d %d%n", &origem, &destino, &distancia, &lidos);
                ptr += lidos;
                Caminho *caminho = criarCaminhos(origem, destino, distancia);
                if (caminho == NULL){
                    printf("Erro ao criar o caminho\n");
                    return;
                }
                adicionarCaminho(&caminhos, caminho);
           
            }
 /*        
            printf("NumPovos: %d\t", numPovos);
            printf("DistanciaMaxima: %d\t", distanciaMaxima);
            printf("PesoMaximo: %d \t", pesoMaximo);
            printf("NumCaminhos: %d\n", numCaminhos);

            printarPovos(&povos);
            printarCaminhos(&caminhos);
*/
            printf("Resolvendo com Heuristica\n");
            clock_t t0 = clock();
            Resultado *resultadoH = resolverComHeuristica(&povos, &caminhos, distanciaMaxima, pesoMaximo);
            clock_t t1 = clock();
            double tempoH = (double)(t1-t0)/CLOCKS_PER_SEC;
            printf("Tempo Heuristica: %f\n", tempoH);
            printf("Habilidade Total: %d\n", resultadoH->habilidadeTotal);
/*          
            printf("Habilidade Total: %d\n, ", resultadoH->habilidadeTotal);
            printf("\nPovos Visitados: ");
            for (int k = 0; k < resultadoH->tamanho; k++){
                printf("%d ", resultadoH->visitados[k]);
            }
            printf("\nQuantidade Recrutada: ");
            for (int k = 0; k < resultadoH->tamanho; k++){
                printf("%d ", resultadoH->quantidadeRecrutada[k]);
            }
*/             
            char nomeArquivoH[50];
            sprintf(nomeArquivoH, "SaidaH%s.txt", instanciaStr);
            escreverSaida(nomeArquivoH, gerarSaida(resultadoH));


            printf("resolvendo com PD\n");
            clock_t t2 = clock();
            Resultado *resultadoPD = resolverComPD(&povos, &caminhos, distanciaMaxima, pesoMaximo);
            clock_t t3 = clock();
            double tempoPD = (double)(t3-t2)/CLOCKS_PER_SEC;
            printf("Tempo PD: %f\n", tempoPD);
            printf("Habilidade Total: %d\n", resultadoPD->habilidadeTotal);
/*
            printf("Habilidade Total: %d\n", resultadoPD->habilidadeTotal);
            printf("\nPovos Visitados: ");
            for (int k = 0; k < resultadoPD->tamanho; k++){
                printf("%d ", resultadoPD->visitados[k]);
            }
            printf("\nQuantidade Recrutada: ");  
            for (int k = 0; k < resultadoPD->tamanho; k++){
                printf("%d ", resultadoPD->quantidadeRecrutada[k]);
            }
*/
            char nomeArquivoPD[50];
            sprintf(nomeArquivoPD, "SaidaPD%s.txt", instanciaStr);
            printf("TEste\n");
            escreverSaida(nomeArquivoPD, gerarSaida(resultadoH));
            
            printf("Fim da Instancia\n");
            printf("\n_____________________________\n\n");
        }

    free(conteudo);
    }


}