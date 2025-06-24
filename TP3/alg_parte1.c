#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IO.h"

char * ProgramacaoDinamica(char *texto, char *padroes, int erro){
    char *resultado = (char *)malloc(1024 * sizeof(char)); // Aloca memória para o resultado
    resultado = strcpy(resultado, "Resultado da Programação Dinâmica\n");
    return resultado;
}
char * ShiftAnd(char *texto, char *padroes, int erro){
    char *resultado = (char *)malloc(1024 * sizeof(char)); // Aloca memória para o resultado
    resultado = strcpy(resultado, "Resultado do ShiftAnd\n");
    return resultado;
}