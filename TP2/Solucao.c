#include "Solucao.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h> 

// Função auxiliar para criar um Resultado
Resultado* criarResultado(int tamanho) {
    Resultado *res = (Resultado *)malloc(sizeof(Resultado));
    if (!res) {
        perror("Erro ao alocar memória para Resultado");
        return NULL;
    }
    // Aloca arrays para visitados e quantidadeRecrutada
    res->visitados = (int *)malloc(tamanho * sizeof(int));
    if (!res->visitados) {
        perror("Erro ao alocar memória para visitados no Resultado");
        free(res);
        return NULL;
    }
    res->quantidadeRecrutada = (int *)malloc(tamanho * sizeof(int));
    if (!res->quantidadeRecrutada) {
        perror("Erro ao alocar memória para quantidadeRecrutada no Resultado");
        free(res->visitados);
        free(res);
        return NULL;
    }
    // Inicializa os valores
    res->tamanho = 0;
    res->habilidadeTotal = 0;
    return res;
}

// Função para liberar a memória de um Resultado
void liberarResultado(Resultado *res) {
    if (res) {
        free(res->visitados);
        free(res->quantidadeRecrutada);
        free(res);
    }
}

// Função para gerar a string de saída formatada
char* gerarSaida(Resultado *res) {
    if (!res) {
        // Retorna uma string vazia para evitar NULL em fprintf
        char* empty_str_err = malloc(sizeof(char) * 2);
        if (empty_str_err) strcpy(empty_str_err, "\n");
        return empty_str_err; 
    }
    // Retorna uma string com apenas a quebra de linha se não houver povos visitados 
    if (res->tamanho == 0) {
        char* empty_str = malloc(sizeof(char) * 2);
        if (empty_str) {
            strcpy(empty_str, "\n");
        }
        return empty_str;
    }

    char *saida = (char *)malloc(MAX_OUTPUT_STRING_SIZE * sizeof(char));
    if (!saida) {
        perror("Erro ao alocar memória para string de saída");
        return NULL;
    }
    int offset = 0; 

    // Adiciona a habilidade total ao início da string
    offset += snprintf(saida + offset, MAX_OUTPUT_STRING_SIZE - offset, "%d", res->habilidadeTotal);

    // Adiciona os povos visitados e quantidades recrutadas
    for (int i = 0; i < res->tamanho; i++) {
        // Verifica se há espaço suficiente para o próximo par (ID QUANTIDADE)
        if (MAX_OUTPUT_STRING_SIZE - offset < 10) { // Estimativa de espaço mínimo para " ID QUANT"
            fprintf(stderr, "Buffer de saída quase cheio, truncando.\n");
            break; 
        }
        offset += snprintf(saida + offset, MAX_OUTPUT_STRING_SIZE - offset, " %d %d", res->visitados[i], res->quantidadeRecrutada[i]);
    }
    // Adiciona uma nova linha ao final da string
    snprintf(saida + offset, MAX_OUTPUT_STRING_SIZE - offset, "\n");
    return saida;
}

// BFS para calcular distâncias reais (pesos das arestas) usando um algoritmo similar a Dijkstra simplificado para grafos com pesos não negativos.
// Retorna as menores distâncias acumuladas a partir do 'inicio'.
static int *bfs_distancias(Grafo *grafo, int inicio) {
    int num = grafo->numPovos;
    int *dist = malloc(num * sizeof *dist);
    // Para um Dijkstra simplificado, usaremos uma fila normal e atualizaremos 'dist'
    // A complexidade será maior que um BFS puro, mas menor que um Dijkstra com Fila de Prioridade otimizada.
    int *fila_q = malloc(num * sizeof *fila_q); 
    int *in_queue = calloc(num, sizeof *in_queue); // Para marcar se um nó está na fila (evita adicionar duplicatas em excesso)

    if (!dist || !fila_q || !in_queue) {
        perror("Erro ao alocar memória para BFS/Dijkstra simplificado");
        free(dist); free(fila_q); free(in_queue);
        return NULL;
    }

    // Inicializa todas as distâncias como infinito
    for (int i = 0; i < num; i++) dist[i] = INT_MAX;

    int head = 0, tail = 0; // Ponteiros para o início e fim da fila
    dist[inicio] = 0;
    fila_q[tail++] = inicio;
    in_queue[inicio] = 1;

    while (head < tail) {
        int u = fila_q[head++]; 
        in_queue[u] = 0; // Remove da fila

        for (Vizinho *v = grafo->adj[u]; v; v = v->prox) {
            // Verifica se a distância atual pode ser melhorada através de 'u'
            if (dist[u] != INT_MAX && dist[u] + v->distancia < dist[v->id]) {
                dist[v->id] = dist[u] + v->distancia;
                // Adiciona à fila apenas se não estiver lá ou se foi re-adicionado para atualização
                if (!in_queue[v->id]) {
                    fila_q[tail++] = v->id;
                    in_queue[v->id] = 1;
                }
            }
        }
    }

    free(fila_q);
    free(in_queue);
    return dist; 
}

// Filtra povos que estão dentro de uma distância máxima D do povo de ID 1 (povo inicial)
Povos filtrarPorDistancia(Povos *original, Grafo *grafo, int D) {
    Povos f = { .povos = NULL, .numPovos = 0, .capacidade = 0 }; 
    // Assumindo que o povo inicial para BFS é o de ID 1.
    // Se o problema permite começar de qualquer lugar, esta função precisaria
    // ser chamada para cada possível ponto de partida, ou D deve ser interpretado como "distância até o povo X".
    // Para o contexto de "autonomia da nave", faz sentido ser do ponto inicial.
    // O povo 1 é apenas um exemplo. Você pode querer passar o ID do povo inicial.
    int *dist = bfs_distancias(grafo, 1); // Calcula distâncias a partir do povo ID 1

    if (!dist) {
        fprintf(stderr, "Erro ao obter distâncias BFS para filtragem.\n");
        return f; 
    }

    // Percorre todos os povos originais
    for (int i = 0; i < original->numPovos; i++) {
        int id = original->povos[i].id;
        // Verifica se o 'id' é válido para o grafo e se a distância está dentro do limite D
        // Note que o array 'dist' tem tamanho 'grafo->numPovos', que é max_id + 1.
        if (id >= 0 && id < grafo->numPovos && dist[id] != INT_MAX && dist[id] <= D) {
            // Cria um novo povo (cópia)
            Povo *p = criarPovo(id, original->povos[i].peso, original->povos[i].habilidade);
            if (p) {
                adicionarPovo(&f, p);
                free(p); // Importante: Liberar a memória do povo criado, pois 'adicionarPovo' copia o conteúdo.
            }
        }
    }
    free(dist); // Libera o array de distâncias
    return f;
}

// Função de comparação para qsort: ordena povos pela razão Habilidade/Peso (decrescente)
int comparePovosByRatio(const void *a, const void *b) {
    Povo *povoA = (Povo *)a;
    Povo *povoB = (Povo *)b;

    // Calcula a razão entre a habilidade e o peso
    // Trata o caso de peso zero para evitar divisão por zero, dando prioridade máxima
    // para povos com habilidade > 0 e peso == 0.
    float ratioA = (povoA->peso == 0) ? (povoA->habilidade > 0 ? (float)INT_MAX : 0.0f) : (float)povoA->habilidade / povoA->peso;
    float ratioB = (povoB->peso == 0) ? (povoB->habilidade > 0 ? (float)INT_MAX : 0.0f) : (float)povoB->habilidade / povoB->peso;

    // Compara as razões para ordenação decrescente (maior razão primeiro)
    if (ratioA < ratioB) return 1;
    if (ratioA > ratioB) return -1;
    return 0;
}

// Resolve o problema usando uma heurística gulosa
Resultado* resolverComHeuristica(Povos *povos, Caminhos *caminhos, int D, int W) {
    // Cria o grafo a partir dos caminhos e dos povos para determinar o numPovos correto
    Grafo *grafo = criarGrafo(caminhos, povos);
    if (!grafo) return NULL;

    // Filtra os povos que estão dentro da distância máxima D do povo inicial (ID 1)
    // Se a regra for "pode começar de qualquer lugar", essa heurística precisaria
    // ser adaptada para cada povo inicial, ou a filtragem ser mais abrangente.
    Povos filtrados = filtrarPorDistancia(povos, grafo, D);
    liberarGrafo(grafo); // Libera o grafo após o uso

    int n = filtrados.numPovos;
    int cap = W; // Capacidade máxima de peso
    Resultado *melhorRes = criarResultado(n); 
    if (!melhorRes) {
        destruirPovos(&filtrados); // Libera os povos filtrados em caso de erro
        return NULL;
    }

    // Ordena os povos filtrados pela razão Habilidade/Peso em ordem decrescente
    qsort(filtrados.povos, n, sizeof(Povo), comparePovosByRatio);

    // Itera sobre os povos ordenados e recruta o máximo possível até a capacidade
    for (int i = 0; i < n && cap > 0; i++) {
        Povo *povo = &filtrados.povos[i];
        // Calcula a quantidade de soldados a recrutar neste povo
        // Se o peso do povo for 0, mas a habilidade for > 0, pode recrutar o máximo.
        // Se ambos forem 0, não recruta.
        int quantidade = 0;
        if (povo->peso > 0) {
            quantidade = cap / povo->peso;
        } else if (povo->habilidade > 0) { // Peso 0, mas habilidade > 0: pode recrutar "infinito" se não houver restrição
            quantidade = 1; // Recruta pelo menos 1 para ganhar a habilidade, se o peso é 0. Ou um valor alto como INT_MAX / 2.
                            // Para este problema, vamos assumir que o peso 0 não significa infinito e que 'quantidade' deve ser controlada.
                            // Se peso 0 e habilidade > 0: recruta 1 (ou o que for definido como "uma unidade de peso 0").
        }

        if (quantidade > 0) {
            // Adiciona o povo ao resultado
            melhorRes->visitados[melhorRes->tamanho] = povo->id;
            melhorRes->quantidadeRecrutada[melhorRes->tamanho] = quantidade;
            melhorRes->habilidadeTotal += quantidade * povo->habilidade;
            melhorRes->tamanho++;
            cap -= quantidade * povo->peso; // Reduz a capacidade restante
        }
    }
    
    destruirPovos(&filtrados); // Libera a memória dos povos filtrados
    return melhorRes;
}

// Resolve o problema usando uma busca com fila de prioridade (similar a Branch and Bound ou A*)
Resultado *resolverComPD(Povos* povos, Caminhos *caminhos, int D, int pesoMax) {
    Grafo *grafo = criarGrafo(caminhos, povos); // Passa povos para correto dimensionamento do grafo
    if (!grafo) {    
        return NULL;
    }

    Resultado *melhor = criarResultado(povos->numPovos);
    if (!melhor) { 
        liberarGrafo(grafo); 
        return NULL; 
    }

    FilaPrioridade *fila = criar_fila(comparador_habilidade);
    if (!fila) { 
        liberarResultado(melhor); 
        liberarGrafo(grafo); 
        return NULL; 
    }

    // Inicializa estados para cada povo como possível ponto de partida de Zorc
    for (int i = 0; i < povos->numPovos; i++) {
        int povo_id = povos->povos[i].id;
        
        Estado estado_inicial = {
            .povo = povo_id, 
            .distancia_restante = D, 
            .peso_usado = 0, 
            .habilidade = 0, 
            .tamanho_caminho = 0, // Inicia com 0, o primeiro povo será adicionado no caminho
            .caminho = (int *)malloc((povos->numPovos + 1) * sizeof(int)), // Capacidade suficiente para o caminho
            .quantidades = (int *)calloc(grafo->numPovos, sizeof(int)) // Usar calloc para zerar, dimensionado pelo max_id
        };

        if (!estado_inicial.caminho || !estado_inicial.quantidades) {
            perror("Erro ao alocar memória para estado inicial");
            free(estado_inicial.caminho); // Libera se um falhar
            free(estado_inicial.quantidades); // Libera se um falhar
            continue; // Tenta com o próximo povo se a alocação falhar
        }
        
        // Adiciona o povo inicial ao caminho
        estado_inicial.caminho[estado_inicial.tamanho_caminho++] = povo_id;

        // Recruta o máximo possível no povo inicial (assumindo que pode recrutar ao chegar)
        Povo *povo_atual_ref = NULL;
        for(int k = 0; k < povos->numPovos; k++) {
            if (povos->povos[k].id == povo_id) {
                povo_atual_ref = &povos->povos[k];
                break;
            }
        }

        if (povo_atual_ref) {
            int max_recrutar = (povo_atual_ref->peso > 0) ? (pesoMax / povo_atual_ref->peso) : 0;
            // Se peso é 0 e habilidade > 0, ainda podemos recrutar uma unidade "virtual"
            if (povo_atual_ref->peso == 0 && povo_atual_ref->habilidade > 0) {
                 if (povo_atual_ref->id < grafo->numPovos && estado_inicial.quantidades[povo_atual_ref->id] == 0) { // Garante que só recruta uma vez se peso for 0
                    estado_inicial.quantidades[povo_atual_ref->id] = 1; // Recruta 1 unidade se peso 0
                    estado_inicial.habilidade += povo_atual_ref->habilidade;
                    // peso_usado não aumenta
                 }
            } else if (max_recrutar > 0) {
                if (povo_id < grafo->numPovos) { // Verifica se o ID é válido para o índice do array 'quantidades'
                    estado_inicial.quantidades[povo_id] = max_recrutar; 
                    estado_inicial.habilidade = max_recrutar * povo_atual_ref->habilidade;
                    estado_inicial.peso_usado = max_recrutar * povo_atual_ref->peso;
                }
            }
        }
        enfileirar(fila, estado_inicial); // Enfileira o estado inicial
    }

    // Loop principal da busca (enquanto a fila não estiver vazia)
    while (!fila_vazia(fila)) {
        Estado estado_atual = desenfileirar(fila); // Pega o estado com maior habilidade
        
        // Verifica se o estado_atual é um estado inválido (retornado por erro de desenfileirar)
        if (estado_atual.caminho == NULL && estado_atual.quantidades == NULL) {
            continue;
        }

        // Atualiza o melhor resultado global se o atual for melhor
        if (estado_atual.habilidade > melhor->habilidadeTotal) {
            melhor->habilidadeTotal = estado_atual.habilidade;
            melhor->tamanho = estado_atual.tamanho_caminho;
            // Copia o caminho e quantidades para o resultado
            memcpy(melhor->visitados, estado_atual.caminho, estado_atual.tamanho_caminho * sizeof(int));
            
            // Copia as quantidades recrutadas para os povos visitados no resultado
            for (int i = 0; i < estado_atual.tamanho_caminho; i++) {
                int id_povo_no_caminho = estado_atual.caminho[i];
                if (id_povo_no_caminho < grafo->numPovos) { // Verifica índice
                    melhor->quantidadeRecrutada[i] = estado_atual.quantidades[id_povo_no_caminho];
                } else {
                    melhor->quantidadeRecrutada[i] = 0; // Fallback
                }
            }
        }

        // Explora os vizinhos do povo atual
        for (Vizinho *viz = grafo->adj[estado_atual.povo]; viz != NULL; viz = viz->prox) {
            // Verifica se há distância suficiente para ir ao vizinho
            if (viz->distancia > estado_atual.distancia_restante) continue;

            // Aloca e inicializa um novo estado para o vizinho
            Estado novo_estado = {
                .povo = viz->id,
                .distancia_restante = estado_atual.distancia_restante - viz->distancia,
                .peso_usado = estado_atual.peso_usado, 
                .habilidade = estado_atual.habilidade,
                .tamanho_caminho = estado_atual.tamanho_caminho,
                // Aloca nova memória para o caminho e quantidades do novo estado
                .caminho = (int *)malloc((povos->numPovos + 1) * sizeof(int)), 
                .quantidades = (int *)malloc(grafo->numPovos * sizeof(int))
            };

            if (!novo_estado.caminho || !novo_estado.quantidades) {
                perror("Erro ao alocar memória para novo estado na exploração");
                free(novo_estado.caminho); 
                free(novo_estado.quantidades); 
                continue;
            }

            // Copia o caminho e quantidades do estado pai para o novo estado
            memcpy(novo_estado.caminho, estado_atual.caminho, estado_atual.tamanho_caminho * sizeof(int));
            memcpy(novo_estado.quantidades, estado_atual.quantidades, grafo->numPovos * sizeof(int));

            // Encontra o Povo correspondente ao vizinho pelo ID
            Povo *povo_alvo = NULL;
            for (int k = 0; k < povos->numPovos; k++) {
                if (povos->povos[k].id == viz->id) { 
                    povo_alvo = &povos->povos[k]; 
                    break; 
                }
            }

            // Tenta recrutar no novo povo (vizinho) se ele não foi recrutado antes
            // ou se for uma mochila ilimitada por povo.
            // A lógica "pega ou não pega o povo" no sentido de já ter visitado.
            // Para mochila ilimitada em cada povo (pode recrutar mais se tiver peso),
            // a lógica abaixo é mais adequada.
            
            // Verifica se o povo já está no caminho (visita única)
            int povo_ja_visitado = 0;
            for(int k = 0; k < estado_atual.tamanho_caminho; k++) {
                if (estado_atual.caminho[k] == viz->id) {
                    povo_ja_visitado = 1;
                    break;
                }
            }

            if (povo_alvo && !povo_ja_visitado) { // Recruta apenas se não visitou ainda
                int peso_disponivel = pesoMax - novo_estado.peso_usado;
                
                if (povo_alvo->peso > 0) {
                    if (peso_disponivel >= povo_alvo->peso) {
                        int recrutados = peso_disponivel / povo_alvo->peso;
                        // Certifique-se de que o ID é válido para o índice
                        if (viz->id < grafo->numPovos) { 
                            novo_estado.quantidades[viz->id] = recrutados; // Define a quantidade recrutada neste povo
                            novo_estado.habilidade += recrutados * povo_alvo->habilidade;
                            novo_estado.peso_usado += recrutados * povo_alvo->peso;
                        }
                    }
                } else if (povo_alvo->habilidade > 0) { // Peso 0, mas habilidade > 0
                     if (viz->id < grafo->numPovos && novo_estado.quantidades[viz->id] == 0) { // Só recruta se ainda não recrutou
                        novo_estado.quantidades[viz->id] = 1;
                        novo_estado.habilidade += povo_alvo->habilidade;
                        // peso_usado não aumenta
                     }
                }
            }
            
            // Adiciona o vizinho ao caminho
            // Verifica se há espaço no array caminho antes de adicionar
            if (novo_estado.tamanho_caminho < (povos->numPovos + 1)) { 
                novo_estado.caminho[novo_estado.tamanho_caminho++] = viz->id;
            } else {
                fprintf(stderr, "Aviso: Caminho excedeu a capacidade alocada para o estado. ID: %d\n", viz->id);
                // Pode ser necessário realocar 'caminho' ou parar a exploração se for crítico.
            }
            
            // Se o novo estado for válido (peso <= pesoMax), enfileira
            if (novo_estado.peso_usado <= pesoMax) {
                enfileirar(fila, novo_estado);
            } else {
                // Se não for válido (excedeu peso), libera a memória alocada para este novo estado
                free(novo_estado.caminho);
                free(novo_estado.quantidades);
            }
        }
        // Libera a memória do estado_atual que foi desenfileirado e processado
        free(estado_atual.caminho);
        free(estado_atual.quantidades);
    }

    liberar_fila(fila); // Libera toda a memória da fila e seus nós restantes
    liberarGrafo(grafo); // Libera o grafo
    return melhor; // Retorna o melhor resultado encontrado
}

// Cria um grafo a partir dos caminhos. Agora recebe 'povos' para calcular o numPovos correto.
Grafo* criarGrafo(Caminhos *caminhos, Povos *povos_list) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    if (!grafo) { perror("Erro ao alocar memória para o grafo"); return NULL; }

    // Determinar o maior ID de povo para dimensionar o array de adjacência
    // Assumindo que os IDs dos povos são 1-based, mas podem não ser sequenciais.
    int max_id = 0;
    for(int i = 0; i < caminhos->numCaminhos; i++) {
        if(caminhos->caminhos[i].origem > max_id) max_id = caminhos->caminhos[i].origem;
        if(caminhos->caminhos[i].destino > max_id) max_id = caminhos->caminhos[i].destino;
    }
    // Também verificar os IDs dos povos que não têm caminhos, se existirem
    for(int i = 0; i < povos_list->numPovos; i++) {
        if(povos_list->povos[i].id > max_id) max_id = povos_list->povos[i].id;
    }

    // O tamanho do array de adjacência deve ser o maior ID + 1
    grafo->numPovos = max_id + 1; 

    grafo->adj = (Vizinho **)calloc(grafo->numPovos, sizeof(Vizinho *));
    if (!grafo->adj) { perror("Erro ao alocar memória para lista de adjacência"); free(grafo); return NULL; }

    // Adiciona cada caminho como duas arestas (ida e volta) para um grafo não direcionado
    for (int i = 0; i < caminhos->numCaminhos; i++) {
        adicionarAresta(grafo, caminhos->caminhos[i].origem, caminhos->caminhos[i].destino, caminhos->caminhos[i].distancia);
        adicionarAresta(grafo, caminhos->caminhos[i].destino, caminhos->caminhos[i].origem, caminhos->caminhos[i].distancia);
    }
    return grafo;
}

// Adiciona uma aresta ao grafo
void adicionarAresta(Grafo *grafo, int origem, int destino, int distancia) {
    // Verifica se os IDs de origem e destino são válidos para o grafo
    if (origem < 0 || origem >= grafo->numPovos || destino < 0 || destino >= grafo->numPovos) {
        fprintf(stderr, "Erro: Tentativa de adicionar aresta com IDs de povo inválidos (%d ou %d) para um grafo de tamanho %d.\n", origem, destino, grafo->numPovos);
        return;
    }

    Vizinho *novo = (Vizinho *)malloc(sizeof(Vizinho));
    if (!novo) { perror("Erro ao alocar memória para vizinho"); return; }
    novo->id = destino;
    novo->distancia = distancia;
    novo->prox = grafo->adj[origem]; 
    grafo->adj[origem] = novo; 
}

// Libera a memória alocada para o grafo
void liberarGrafo(Grafo *grafo) {
    if (!grafo) return;
    for (int i = 0; i < grafo->numPovos; i++) {
        Vizinho *v = grafo->adj[i];
        while (v) {
            Vizinho *tmp = v;
            v = v->prox;
            free(tmp);
        }
    }
    free(grafo->adj);
    free(grafo);
}

// Comparador para a fila de prioridade (maior habilidade primeiro)
int comparador_habilidade(const Estado *a, const Estado *b) {
    // Retorna um valor positivo se 'a' tiver menor habilidade que 'b' (b está à frente)
    // Retorna um valor negativo se 'a' tiver maior habilidade que 'b' (a está à frente)
    // Retorna 0 se forem iguais.
    // Isso garante ordem decrescente de habilidade.
    return b->habilidade - a->habilidade; 
}

// Cria uma nova fila de prioridade
FilaPrioridade* criar_fila(int (*comparador)(const Estado*, const Estado*)) {
    FilaPrioridade *fila = (FilaPrioridade*)malloc(sizeof(FilaPrioridade));
    if (!fila) { perror("Erro ao alocar memória para FilaPrioridade"); return NULL; }
    fila->cabeca = NULL;
    fila->comparador = comparador;
    return fila;
}

// Libera a memória de toda a fila de prioridade e seus conteúdos
void liberar_fila(FilaPrioridade *fila) {
    if (!fila) return;
    NoFila *atual = fila->cabeca;
    while (atual) {
        NoFila *prox = atual->prox;
        // Libera os componentes dinâmicos do Estado ANTES de liberar o nó
        free(atual->estado.caminho);
        free(atual->estado.quantidades);
        free(atual); // Libera o nó
        atual = prox;
    }
    free(fila); // Libera a estrutura da fila
}

// Adiciona um estado à fila de prioridade, mantendo a ordem
void enfileirar(FilaPrioridade *fila, Estado estado) {
    NoFila *novo = (NoFila*)malloc(sizeof(NoFila));
    if (!novo) {
        perror("Erro ao alocar memória para NoFila");
        // Se não puder alocar o nó, libera os componentes do estado
        // pois a posse da memória não foi transferida para o nó da fila.
        free(estado.caminho); 
        free(estado.quantidades);
        return;
    }
    novo->estado = estado; // A struct estado (e seus ponteiros) é copiada
    novo->prox = NULL;

    // Se a fila está vazia ou o novo estado tem maior prioridade que a cabeça
    if (!fila->cabeca || fila->comparador(&novo->estado, &fila->cabeca->estado) > 0) {
        novo->prox = fila->cabeca;
        fila->cabeca = novo;
    } else {
        // Percorre a fila para encontrar a posição correta para inserção
        NoFila *atual = fila->cabeca;
        while (atual->prox && fila->comparador(&novo->estado, &atual->prox->estado) <= 0) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

// Remove e retorna o estado de maior prioridade da fila
// IMPORTANTE: Esta função transfere a POSSE dos arrays 'caminho' e 'quantidades'
// para a struct Estado que é retornada. O chamador é responsável por liberar esses arrays.
Estado desenfileirar(FilaPrioridade *fila) {
    // Retorna um estado inválido se a fila estiver vazia
    if (fila_vazia(fila)) {
        fprintf(stderr, "Erro: Tentativa de desenfileirar de uma fila vazia.\n");
        Estado empty_state = {0}; // Inicializa todos os membros com 0
        empty_state.caminho = NULL; // Assegura que os ponteiros são NULL
        empty_state.quantidades = NULL;
        return empty_state;
    }

    NoFila *temp = fila->cabeca;
    Estado estado_retornado = temp->estado; // COPIA a struct Estado (incluindo os ponteiros)

    fila->cabeca = temp->prox;
    // NÃO LIBERAMOS temp->estado.caminho e temp->estado.quantidades AQUI!
    // A posse desses ponteiros foi transferida para 'estado_retornado'.
    free(temp); // Libera APENAS o nó da fila (NoFila)

    return estado_retornado; // Retorna a CÓPIA do estado, com seus ponteiros alocados
}

// Verifica se a fila de prioridade está vazia
int fila_vazia(FilaPrioridade *fila) {
    return fila->cabeca == NULL;
}