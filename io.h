#ifndef IO_H
#define IO_H

// lê o conteúdo do arquivo de entrada especificado por `nomeEntrada` e retorna como uma string
// a leitura ignora linhas vazias e para se encontrar uma linha com "0 0"
char* lerEntrada(char *nomeEntrada);

// escreve o conteúdo passado por `conteudo` no arquivo de saída (nomeSaida)
void escreverSaida(char *nomeSaida, char *conteudo);

#endif