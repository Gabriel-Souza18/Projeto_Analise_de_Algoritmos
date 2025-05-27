objetos = IO.o Caminhos.o Solucao.o

run: tp2
	./tp2 entrada.txt

tp2: tp2.c $(objetos)
	gcc -o tp2 tp2.c $(objetos) -lm


IO.o: IO.c IO.h
	gcc -c IO.c
	
Caminhos.o: Caminhos.c Caminhos.h
	gcc -c Caminhos.c

Solucao.o: Solucao.c Solucao.h Caminhos.h
	gcc -c Solucao.c
clean:
	rm -f tp2 $(objetos)
	rm -f *.o
	# remove os txt menos a entrada
	find . -maxdepth 1 -type f -name '*.txt' ! -name 'entrada.txt' -delete
