#ifndef CAMINHOS_H
#define CAMINHOS_H
typedef struct Povo{
    int id;
    int peso;
    int habilidade;

}Povo;
typedef struct Caminho{
    int origem;
    int destino;
    int distancia;
}Caminho;

typedef struct Povos{
    Povo *povos;
    int numPovos;
}Povos;

typedef struct Caminhos{
    Caminho *caminhos;
    int numCaminhos;
}Caminhos;

Povo * criarPovo(int id, int peso, int habilidade);

void adicionarPovo(Povos *povos, Povo *povo);
void destruirPovos(Povos *povos);
void printarPovos(Povos *povos);

Caminho * criarCaminhos(int origem, int destino, int distancia);

void adicionarCaminho(Caminhos *caminhos, Caminho *caminho);
void destruirCaminhos(Caminhos *caminhos);
void printarCaminhos(Caminhos *caminhos);

#endif