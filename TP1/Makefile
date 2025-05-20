objetos = io.o tabuleiro.o algoritmos.o main.c 

run: main
	./tp1 -i entrada.txt 

main: $(objetos)
	gcc $(objetos) -o tp1

algoritmos.o: algoritmos.h algoritmos.c
	gcc -c algoritmos.c

tabuleiro.o: tabuleiro.h tabuleiro.c
	gcc -c tabuleiro.c

io.o: io.h io.c
	gcc -c io.c

clean:
	rm -rf  io.o tabuleiro.o  algoritmos.o tp1
