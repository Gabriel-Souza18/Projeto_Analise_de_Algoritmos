objetos = io.o teste.c

prog1: $(objetos)
	gcc $(objetos) -o teste

io.o: io.h io.c
	gcc -c io.c

clean:
	rm -rf io.o teste
