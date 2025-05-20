objetos = IO.o Caminhos.o

run: main
	./main

main: main.c $(objetos)
	gcc -o main main.c $(objetos) -lm


IO.o: IO.c IO.h
	gcc -c IO.c
	
Caminhos.o: Caminhos.c Caminhos.h
	gcc -c Caminhos.c

clean:
	rm -f main $(objetos)
	rm -f *.o
	rm -f saida.txt
