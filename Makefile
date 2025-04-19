objetos = io.o tabuleiro.o algoritmos.o main.c 

run: main
	./main -i entrada.txt 

main: $(objetos)
	gcc $(objetos) -o main

algoritmos.o: algoritmos.h algoritmos.c
	gcc -c algoritmos.c

tabuleiro.o: tabuleiro.h tabuleiro.c
	gcc -c tabuleiro.c

io.o: io.h io.c
	gcc -c io.c

clean:
	rm -rf  saida.txt io.o tabuleiro.o  algoritmos.o main 
