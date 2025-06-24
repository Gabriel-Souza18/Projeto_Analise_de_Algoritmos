objetos = IO.o 

run: tp3_parte1
	./tp3_parte1 1 Arquivo_texto.txt Arquivo_Padroes.txt

tp3: tp3_parte1.c $(objetos)
	gcc -o tp3_parte1 tp3_parte1.c $(objetos) -lm


IO.o: IO.c IO.h
	gcc -c IO.c
	
