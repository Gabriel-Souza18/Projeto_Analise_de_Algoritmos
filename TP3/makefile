objetos1 = IO.o alg_parte1.o
objetos2 = IO.o alg_parte2.o

run: tp4_parte1
	./tp4_parte1 1 Arquivo_texto.txt Arquivo_Padroes.txt
	./tp4_parte1 2 Arquivo_texto.txt Arquivo_Padroes.txt
	./tp4_parte2 Arquivo_texto.txt Arquivo_Padroes.txt

tp4: tp4_parte1.c $(objetos1) $(objetos2)
	gcc -o tp4_parte1 tp4_parte1.c $(objetos1) -lm
	gcc -o tp4_parte2 tp4_parte2.c $(objetos2) -lm


IO.o: IO.c IO.h
	gcc -c IO.c
	
alg_parte1.o: alg_parte1.c alg_parte1.h
	gcc -c alg_parte1.c
	
alg_parte2.o: alg_parte2.c alg_parte2.h
	gcc -c alg_parte2.c


clean:
	rm -f *.o 
	rm -f saida_*.txt
	rm -f tp4_parte1
	rm -f tp4_parte2
	rm -f *.csv 