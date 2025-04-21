# Projeto_Analise_de_Algoritmos

Repositório para a disciplina de Projeto e Análise de Algoritmos (2025/1 – Prof. Leonardo Rocha). Este programa lê uma série de tabuleiros de um jogo de “captura” (semelhante a damas), executa algorítmos de força‑bruta e de mínimo de movimentos, mede tempos de usuário e sistema, e grava os resultados em um arquivo de saída.

## Estrutura do Projeto

- **Makefile**  
  Regras de compilação e execução (`make main`, `make run`, `make clean`).

- **entrada.txt**  
  Arquivo de entrada contendo vários casos de teste:
  - Cada caso começa com `N M` (linhas e colunas, 3 ≤ N,M ≤ 20; N×M ≤ 200).
  - Em seguida um vetor de `(N×M)/2` valores: `0` (casa vazia), `1` (sua peça), `2` (peça adversária).
  - O par `0 0` indica fim da entrada.

- **io.c / io.h**  
  Leitura de todo o arquivo de entrada em memória (`lerEntrada`) e escrita de saída em arquivo (`escreverSaida`).

- **tabuleiro.c / tabuleiro.h**  
  Estrutura `Tabuleiro` e rotinas de:
  - `inicializaTabuleiro` — converte a string lida num struct   
  - `liberaTabuleiro` — desaloca memória

- **algoritmos.c / algoritmos.h**  
  Implementações de:
  - `forcaBruta(Tabuleiro*)` — busca exaustiva para capturas máximas recursivas  
  - `minMovimentos(Tabuleiro*)` — pilha dinâmica para calcular o mínimo de movimentos até não haver mais capturas

- **main.c**  
  Função `main` que:
  1. Lê `argv[2]` (por exemplo, `entrada.txt`)  
  2. Processa cada caso: inicializa, executa `forcaBruta` ou `minMovimentos`, acumula resultados  
  3. Mede tempo de usuário e sistema (`gettimeofday`, `getrusage`)  
  4. Grava `saida.txt` e imprime no console os tempos em microssegundos

## Como Compilar e Executar

No diretório do projeto:

```bash
make main      # compila todos os módulos gerando o executável `main`
make run         # executa: ./main -i entrada.txt
```

Para limpar artefatos:

```bash
make clean
```

## Formato de Entrada

Exemplo de `entrada.txt`:

```
3 3
2 1 2 0 1
5 3
1 0 2 1 0 2 0 0
8 8
2 2 2 2 0 0 0 0 2 2 2 2 0 0 0 0 2 2 2 2 0 0 0 0 2 2 2 2 0 1 0 0
0 0
```

## Formato de Saída

O programa gera `saida.txt` com um inteiro por linha — o resultado do algoritmo (número máximo de capturas ou mínimo de movimentos) para cada tabuleiro.  
Exemplo de `saida.txt`:

```
1
2
7
```

Além disso, no console são exibidos:

```
Tempo de usuário: XXX us
Tempo de sistema: YYY us
```


