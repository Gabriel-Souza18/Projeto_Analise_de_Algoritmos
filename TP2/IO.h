#ifndef IO_H
#define IO_H

// Define um tamanho máximo para o buffer de leitura de arquivo.
#define MAX_IO_BUFFER_SIZE 4096

char* lerEntrada(char *entrada); 

void escreverSaida(char *saida, char*conteudo);

#endif