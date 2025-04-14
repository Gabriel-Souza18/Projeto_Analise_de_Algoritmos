typedef struct  tabuleiro {
    int N;
    int M;
    int * estado; // Tabuleiro 8x8
} Tabuleiro;

Tabuleiro* inicializaTabuleiro(char* entrada);
void liberaTabuleiro(Tabuleiro *tabuleiro);
void imprimeTabuleiro(Tabuleiro *tabuleiro);
