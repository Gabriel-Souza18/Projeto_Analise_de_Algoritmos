#ifndef IO_H
#define IO_H


#define MAX_IO_BUFFER_SIZE 4096

typedef struct {
    double tempo;   
    char *saida;           
} ResultadoParte1;

typedef struct {
    double tempo;    
    char *saida;       
    int num_comparaçoes;           
} ResultadoParte2;

char* lerEntrada(char *entrada); 

void escreverSaida(char *saida, char*conteudo);


#endif