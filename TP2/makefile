objetos = IO.o Caminhos.o Solucao.o

run: main
	./main

main: main.c $(objetos)
	gcc -o main main.c $(objetos) -lm


IO.o: IO.c IO.h
	gcc -c IO.c
	
Caminhos.o: Caminhos.c Caminhos.h
	gcc -c Caminhos.c

Solucao.o: Solucao.c Solucao.h Caminhos.h
	gcc -c Solucao.c
clean:
	rm -f main $(objetos)
	rm -f *.o
	# remove os txt menos a entrada
	find . -maxdepth 1 -type f -name '*.txt' ! -name 'entrada.txt' -delete
