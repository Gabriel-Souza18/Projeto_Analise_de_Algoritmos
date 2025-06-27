# Projeto Análise de Algoritmos – TP3

Este projeto implementa dois métodos de casamento aproximado de padrões em textos:

- **Programação Dinâmica**  
  Versão adaptada da distância de Levenshtein (até *k* erros).
- **Shift-And Aproximado**  
  Extensão bit-paralela do algoritmo Shift-And para permitir até *k* erros.

---

## Estrutura do Projeto

- **Makefile**  
  - `make all` (ou `make tp4_parte1 tp4_parte2`): compila ambos os binários  
  - `make run`: executa um exemplo com `Arquivo_texto.txt` e `Arquivo_Padroes.txt`  
  - `make clean`: remove objetos, binários e arquivos de saída  
- **Arquivo_texto.txt**  
  Texto de entrada para busca de padrões.  
- **Arquivo_Padroes.txt**  
  Lista de padrões (uma string por linha).  
- **IO.c / IO.h**  
  Leitura de arquivo em memória (`lerArquivo`) e escrita de resultados em arquivo (`escreverSaida`).  
- **alg_parte1.c / alg_parte1.h**  
  Implementação de Programação Dinâmica e Shift-And Aproximado.  
- **alg_parte2.c / alg_parte2.h**  
  Implementação de Boyer–Moore–Horspool e busca em texto Huffman-comprimido.  
- **tp4_parte1**  
  Binário: executa Parte 1 (PD vs Shift-And), imprime tabela e salva ocorrências (k=3) em  
  `saida_ProgramacaoDinamica_ocorrencias.txt` ou `saida_ShiftAnd_ocorrencias.txt`.  
- **tp4_parte2**  
  Binário: executa Parte 2 (BMH vs busca em texto comprimido), imprime comparação e salva  
  ocorrências não comprimidas em `saida_parte2_ocorrencias.txt`.

---

## Como Compilar e Executar

No diretório raiz do projeto:

1. Compile tudo:
   ```bash
   make all
   ```
2. Execute Parte 1 (Programação Dinâmica = 1 | Shift-And = 2):
   ```bash
   ./tp4_parte1 <algoritmo> <arquivo_texto> <arquivo_padroes>
   # ex.: ./tp4_parte1 1 Arquivo_texto.txt Arquivo_Padroes.txt
   ```
3. Execute Parte 2:
   ```bash
   ./tp4_parte2 <arquivo_texto> <arquivo_padroes>
   # ex.: ./tp4_parte2 Arquivo_texto.txt Arquivo_Padroes.txt
   ```
4. Limpe artefatos:
   ```bash
   make clean
   ```

---

## Exemplo de Saída

### Parte 1 (tp4_parte1)

```text
Executando Parte 1 com o algoritmo: ProgramacaoDinamica
Texto: Arquivo_texto.txt (62 caracteres)
Padrões: Arquivo_Padroes.txt

Padrão               | k | Tempo (s)    | Comparações
----------------------------------------------------------
palavra              | 0 | 0.000024     | 434
palavra              | 1 | 0.000019     | 434
palavra              | 2 | 0.000017     | 434
palavra              | 3 | 0.000023     | 434
----------------------------------------------------------
exemplo              | 0 | 0.000019     | 434
exemplo              | 1 | 0.000018     | 434
exemplo              | 2 | 0.000012     | 434
exemplo              | 3 | 0.000012     | 434
----------------------------------------------------------
```

Arquivo de ocorrências (k=3):  
```text
palavra 24 25 26 27 28 29 30 34 35 36 37 38 39 40
exemplo 5 6 7 8 9 10 11
```

### Parte 2 (tp4_parte2)

```text
Executando Parte 2: BMH (Não Comprimido) vs Busca (Comprimido)
Texto: Arquivo_texto.txt (62 caracteres)
Padrões: Arquivo_Padroes.txt

Compressão Huffman:
 - Tempo de compressão: 0.000120 s
 - Tamanho Original: 62 bytes
 - Tamanho Comprimido: 28 bytes
 - Taxa de Compressão: 54.84%

Padrão          | BMH Não-Compr.       | Busca Comprimida
                | Tempo (s) | Comp.     | Tempo (s) | Comp.
----------------------------------------------------------------
palavra         | 0.000015  |  69       | 0.000004  | 128
exemplo         | 0.000004  |  46       | 0.000002  | 126
```

Arquivo de ocorrências parte 2 (não comprimido):  
```text
palavra 24 25 26 27 28 29 30 34 35 36 37 38 39 40
exemplo 5 6 7 8 9 10
```

