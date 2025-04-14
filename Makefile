objetos = io.o teste.c

run: teste
	./teste

teste: $(objetos)
	gcc $(objetos) -o teste


io.o: io.h io.c
	gcc -c io.c

clean:
	rm -rf io.o teste
