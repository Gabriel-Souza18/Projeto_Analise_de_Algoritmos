objetos = io.o tabuleiro.o teste.c 

run: teste
	./teste

teste: $(objetos)
	gcc $(objetos) -o teste

tabuleiro.o: tabuleiro.h tabuleiro.c
	gcc -c tabuleiro.c

io.o: io.h io.c
	gcc -c io.c

clean:
	rm -rf io.o teste
