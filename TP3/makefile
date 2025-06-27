# Objetos comuns
OBJ_IO    = IO.o
OBJ_P1    = alg_parte1.o
OBJ_P2    = alg_parte2.o

# Executáveis
all: tp4_parte1 tp4_parte2

tp4_parte1: tp4_parte1.c $(OBJ_IO) $(OBJ_P1)
	gcc -o $@ tp4_parte1.c $(OBJ_IO) $(OBJ_P1) -lm

tp4_parte2: tp4_parte2.c $(OBJ_IO) $(OBJ_P2)
	gcc -o $@ tp4_parte2.c $(OBJ_IO) alg_parte2.o -lm

run: tp4_parte1 tp4_parte2
	./tp4_parte1 1 Arquivo_texto.txt Arquivo_Padroes.txt
	./tp4_parte1 2 Arquivo_texto.txt Arquivo_Padroes.txt
	./tp4_parte2 Arquivo_texto.txt Arquivo_Padroes.txt

# Compilação dos objetos
IO.o: IO.c IO.h
	gcc -c IO.c

alg_parte1.o: alg_parte1.c alg_parte1.h
	gcc -c alg_parte1.c

alg_parte2.o: alg_parte2.c alg_parte2.h
	gcc -c alg_parte2.c

clean:
	rm -f *.o tp4_parte1 tp4_parte2 *.csv saida_*.txt