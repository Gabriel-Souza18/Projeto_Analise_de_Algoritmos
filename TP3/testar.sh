#!/usr/bin/env bash

PATTERNS="Arquivo_Padroes.txt"
TEST_DIR="./arquivos_de_teste"
OUT1="resultados_parte1.csv"
OUT2="resultados_parte2.csv"

# Cabeçalhos dos CSVs
echo "arquivo;algoritmo;padrao;k;tempo;comparacoes" > $OUT1
echo "arquivo;padrao;tempo_nao;comp_nao;tempo_comp;comp_comp" > $OUT2

for txt in $TEST_DIR/*.txt; do
  base=$(basename "$txt")
  echo "=== Testando $base ==="

  # PARTE 1: Programação Dinâmica (1) e Shift-And (2)
  for alg in 1 2; do
    echo "  -> Parte 1, algoritmo $alg"
    out=$(./tp3_parte1 $alg "$txt" $PATTERNS)

    while IFS= read -r padrao; do
      [[ -z "$padrao" ]] && continue
      for k in {0..3}; do
        linha=$(echo "$out" \
          | awk -F'|' -v p="$padrao" -v K="$k" '$1~p && $2~K {print}')
        tempo=$(echo $linha | awk -F'|' '{gsub(/^ +| +$/,"",$3); print $3}')
        comps=$(echo $linha | awk -F'|' '{gsub(/^ +| +$/,"",$4); print $4}')
         echo "$base;$alg;$padrao;$k;$tempo;$comps;" >> $OUT1
      done
    done < $PATTERNS
  done

  # PARTE 2: BMH vs Busca Comprimida
  echo "  -> Parte 2"
  out2=$(./tp3_parte2 "$txt" $PATTERNS)
  while IFS= read -r padrao; do
    [[ -z "$padrao" ]] && continue
    linha=$(echo "$out2" | awk -F'|' -v p="$padrao" '$1~p {print}')
    t1=$(echo $linha | awk -F'|' '{gsub(/^ +| +$/,"",$2); print $2}')
    c1=$(echo $linha | awk -F'|' '{gsub(/^ +| +$/,"",$3); print $3}')
    t2=$(echo $linha | awk -F'|' '{gsub(/^ +| +$/,"",$4); print $4}')
    c2=$(echo $linha | awk -F'|' '{gsub(/^ +| +$/,"",$5); print $5}')
    echo "$base;$padrao;$t1;$c1;$t2;$c2" >> $OUT2
  done < $PATTERNS


done

echo "✅ Resultados salvos em: $OUT1 e $OUT2"