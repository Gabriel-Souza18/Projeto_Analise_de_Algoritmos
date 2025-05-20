#include "IO.h"
#include  "Caminhos.h"

#include <stdio.h>
#include <stdlib.h>

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
         
            printf("NumPovos: %d\t", numPovos);
            printf("DistanciaMaxima: %d\t", distanciaMaxima);
            printf("PesoMaximo: %d \t", pesoMaximo);
            printf("NumCaminhos: %d\n", numCaminhos);

            printarPovos(&povos);
            printarCaminhos(&caminhos);


            destruirCaminhos(&caminhos);
            destruirPovos(&povos);
            printf("Fim da Instancia\n");
        }

    free(conteudo);
    }
}