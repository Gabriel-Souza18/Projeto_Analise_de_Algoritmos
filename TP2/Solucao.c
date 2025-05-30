#include "Solucao.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h> 

Resultado* criarResultado(int tamanho){
    Resultado *res = (Resultado *)malloc(sizeof(Resultado));
    if (!res){
        perror("Erro ao alocar memória para Resultado");
        return NULL;
    }
    res->visitados = (int *)malloc(tamanho * sizeof(int));
    if (!res->visitados){
        perror("Erro ao alocar memória para visitados no Resultado");
        free(res);
        return NULL;
    }
    res->quantidadeRecrutada = (int *)malloc(tamanho * sizeof(int));
    if (!res->quantidadeRecrutada){
        perror("Erro ao alocar memória para quantidadeRecrutada no Resultado");
        free(res->visitados);
        free(res);
        return NULL;
    }
    // inicializa os valores
    res->tamanho = 0;
    res->habilidadeTotal = 0;
    return res;
}

void liberarResultado(Resultado *res){
    if (res){
        free(res->visitados);
        free(res->quantidadeRecrutada);
        free(res);
    }
}

char* gerarSaida(Resultado *res){
    if (!res){
        return NULL; 
    }
    // retorna uma string vazia se não houver povos visitados 
    if (res->tamanho == 0){
        char* empty_str = malloc(2 * sizeof(char));
        if (empty_str){
            strcpy(empty_str, "\n");
        }
        return empty_str;
    }

    char *saida = (char *)malloc(MAX_OUTPUT_STRING_SIZE * sizeof(char));
    if (!saida){
        perror("Erro ao alocar memória para string de saída");
        return NULL;
    }
    char buffer[64]; 
    int offset = 0; 

    offset += snprintf(saida + offset, MAX_OUTPUT_STRING_SIZE - offset, "%d", res->habilidadeTotal);

    for (int i = 0; i < res->tamanho; i++) {
        offset += snprintf(saida + offset, MAX_OUTPUT_STRING_SIZE - offset, " %d %d", res->visitados[i], res->quantidadeRecrutada[i]);
        if (offset >= MAX_OUTPUT_STRING_SIZE) {
            fprintf(stderr, "Buffer de saída excedido.\n");
            break;
        }
    }
    snprintf(saida + offset, MAX_OUTPUT_STRING_SIZE - offset, "\n");
    return saida;
}

static int *bfs_distancias(Grafo *grafo, int inicio) {
    int num = grafo->numPovos;
    int *dist = malloc(num * sizeof *dist);
    int *visitados  = calloc(num, sizeof *visitados); 
    int *fila = malloc(num * sizeof *fila);
    int ini = 0, fim = 0;

    if (!dist || !visitados || !fila) {
        perror("Erro ao alocar memória para BFS");
        free(dist); free(visitados); free(fila);
        return NULL;
    }

    for (int i = 0; i < num; i++) dist[i] = INT_MAX;

    dist[inicio] = 0;
    visitados[inicio]  = 1;
    fila[fim++] = inicio;

    while (ini < fim) {
        int u = fila[ini++]; 
        for (Vizinho *v = grafo->adj[u]; v; v = v->prox) {
            if (!visitados[v->id]) {
                visitados[v->id] = 1;
                dist[v->id] = dist[u] + 1; 
                fila[fim++] = v->id; 
            }
        }
    }

    free(visitados);
    free(fila);
    return dist; 
}

Povos filtrarPorDistancia(Povos *original, Grafo *grafo, int D) {
    Povos f = { .povos = NULL, .numPovos = 0, .capacidade = 0 }; 
    int *dist = bfs_distancias(grafo, 1); 

    if (!dist) {
        fprintf(stderr, "Erro ao obter distâncias BFS.\n");
        return f; 
    }

    for (int i = 0; i < original->numPovos; i++) {
        int id = original->povos[i].id;
        // verifica se o 'id' é válido e se a distância está dentro do limite D
        if (id < grafo->numPovos && dist[id] != INT_MAX && dist[id] <= D) {
            // cria um novo povo 
            Povo *p = criarPovo(id, original->povos[i].peso, original->povos[i].habilidade);
            if (p) {
                adicionarPovo(&f, p);
                free(p); 
            }
        }
    }
    free(dist); 
    return f;
}

int comparePovosByRatio(const void *a, const void *b) {
    Povo *povoA = (Povo *)a;
    Povo *povoB = (Povo *)b;

    // faz a razão entre a habilidade e o peso
    float ratioA = (povoA->peso == 0) ? (float)INT_MAX : (float)povoA->habilidade / povoA->peso;
    float ratioB = (povoB->peso == 0) ? (float)INT_MAX : (float)povoB->habilidade / povoB->peso;

    // compara as razões
    if (ratioA < ratioB) return 1;
    if (ratioA > ratioB) return -1;
    return 0;
}

Resultado* resolverComHeuristica(Povos *povos, Caminhos *caminhos, int D, int W) {
    Grafo *grafo = criarGrafo(caminhos, povos->numPovos);
    if (!grafo) return NULL;

    Povos filtrados = filtrarPorDistancia(povos, grafo, D);
    liberarGrafo(grafo); 

    int n = filtrados.numPovos;
    int cap = W;
    Resultado *melhorRes = criarResultado(n); 
    if (!melhorRes) {
        destruirPovos(&filtrados);
        return NULL;
    }

    qsort(filtrados.povos, n, sizeof(Povo), comparePovosByRatio);

    for (int i = 0; i < n && cap > 0; i++) {
        Povo *povo = &filtrados.povos[i];
        int quantidade = (povo->peso > 0) ? (cap / povo->peso) : 0; 

        if (quantidade > 0) {
            melhorRes->visitados[melhorRes->tamanho] = povo->id;
            melhorRes->quantidadeRecrutada[melhorRes->tamanho] = quantidade;
            melhorRes->habilidadeTotal += quantidade * povo->habilidade;
            melhorRes->tamanho++;
            cap -= quantidade * povo->peso;
        }
    }
    
    destruirPovos(&filtrados); 
    return melhorRes;
}

Resultado *resolverComPD(Povos* povos, Caminhos *caminhos, int D, int pesoMax) {
    Grafo *grafo = criarGrafo(caminhos, povos->numPovos);
    if (!grafo){    
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

    for (int i = 0; i < povos->numPovos; i++) {
        int povo_id = povos->povos[i].id;
        Estado estado_inicial = {
            .povo = povo_id, .distancia_restante = D, .peso_usado = 0, .habilidade = 0, .tamanho_caminho = 1;
            .caminho = (int *)malloc((povos->numPovos + 1) * sizeof(int));
            .quantidades = (int *)malloc(grafo->numPovos * sizeof(int)); 
        }

        if (!estado_inicial.caminho || !estado_inicial.quantidades) {
            perror("Erro ao alocar memória para estado inicial");
            free(estado_inicial.caminho); free(estado_inicial.quantidades); continue;
        }

        estado_inicial.caminho[0] = povo_id;
        memset(estado_inicial.quantidades, 0, grafo->numPovos * sizeof(int)); // Zera quantidades

        // Recruta o máximo possível no povo inicial
        int max_recrutar = (povos->povos[i].peso > 0) ? (pesoMax / povos->povos[i].peso) : 0;
        if (max_recrutar > 0) {
            estado_inicial.quantidades[povo_id - 1] = max_recrutar; // Usa ID-1 como índice
            estado_inicial.habilidade = max_recrutar * povos->povos[i].habilidade;
            estado_inicial.peso_usado = max_recrutar * povos->povos[i].peso;
        }

        enfileirar(fila, estado_inicial); // Enfileira o estado inicial
    }

    // Loop principal da busca (enquanto a fila não estiver vazia)
    while (!fila_vazia(fila)) {
        Estado estado_atual = desenfileirar(fila); // Pega o estado com maior habilidade

        // Atualiza o melhor resultado global se o atual for melhor
        if (estado_atual.habilidade > melhor->habilidadeTotal) {
            melhor->habilidadeTotal = estado_atual.habilidade;
            melhor->tamanho = estado_atual.tamanho_caminho;
            // Copia o caminho e quantidades para o resultado
            memcpy(melhor->visitados, estado_atual.caminho, estado_atual.tamanho_caminho * sizeof(int));
            for (int i = 0; i < estado_atual.tamanho_caminho; i++) {
                 int id = estado_atual.caminho[i];
                 melhor->quantidadeRecrutada[i] = estado_atual.quantidades[id - 1];
            }
        }

        // Explora os vizinhos do povo atual
        for (Vizinho *viz = grafo->adj[estado_atual.povo]; viz != NULL; viz = viz->prox) {
            // Verifica se há distância suficiente para ir ao vizinho
            if (viz->distancia > estado_atual.distancia_restante) continue;

            // Cria um novo estado para o vizinho
            Estado novo_estado = {
                .povo = viz->id,
                .distancia_restante = estado_atual.distancia_restante - viz->distancia,
                .peso_usado = estado_atual.peso_usado, // Começa com os valores do pai
                .habilidade = estado_atual.habilidade,
                .tamanho_caminho = estado_atual.tamanho_caminho,
                .caminho = (int *)malloc((povos->numPovos + 1) * sizeof(int)),
                .quantidades = (int *)malloc(grafo->numPovos * sizeof(int))
            };

            if (!novo_estado.caminho || !novo_estado.quantidades) {
                perror("Erro ao alocar memória para novo estado");
                free(novo_estado.caminho); free(novo_estado.quantidades); continue;
            }

            // Copia o caminho e quantidades do estado pai
            memcpy(novo_estado.caminho, estado_atual.caminho, estado_atual.tamanho_caminho * sizeof(int));
            memcpy(novo_estado.quantidades, estado_atual.quantidades, grafo->numPovos * sizeof(int));

            // Encontra o Povo correspondente ao vizinho
            Povo *povo_atual = NULL;
            for (int i = 0; i < povos->numPovos; i++) {
                if (povos->povos[i].id == viz->id) { povo_atual = &povos->povos[i]; break; }
            }

            // Tenta recrutar no novo povo (vizinho)
            int recrutados = 0;
            if (povo_atual && povo_atual->peso > 0) {
                // Recruta o máximo possível com o peso restante
                recrutados = (pesoMax - novo_estado.peso_usado) / povo_atual->peso;
                if (recrutados > 0) {
                    // Atualiza o estado com os novos recrutados
                    novo_estado.quantidades[viz->id - 1] += recrutados; // Adiciona aos existentes
                    novo_estado.habilidade += recrutados * povo_atual->habilidade;
                    novo_estado.peso_usado += recrutados * povo_atual->peso;
                }
            }

            // Atualiza o caminho e tamanho
            novo_estado.caminho[estado_atual.tamanho_caminho] = viz->id;
            novo_estado.tamanho_caminho++;

            // Se o novo estado for válido (peso <= pesoMax), enfileira
            if (novo_estado.peso_usado <= pesoMax) {
                enfileirar(fila, novo_estado);
            } else {
                // Se não for válido, libera a memória alocada para ele (evita leak)
                free(novo_estado.caminho);
                free(novo_estado.quantidades);
            }
        }
        // Libera a memória do estado atual (que foi desenfileirado)
        // A função liberar_fila fará isso ao final, mas o ideal seria
        // liberar aqui, após explorar todos os vizinhos. Contudo,
        // liberar_fila já cuida disso.
        free(estado_atual.caminho);
        free(estado_atual.quantidades);
    }

    liberar_fila(fila); // Libera toda a memória da fila e seus nós
    liberarGrafo(grafo); // Libera o grafo
    return melhor; // Retorna o melhor resultado encontrado
}

Grafo* criarGrafo(Caminhos *caminhos, int numPovos) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    if (!grafo) { perror("Erro ao alocar memória para o grafo"); return NULL; }

    grafo->numPovos = numPovos + 1; // +1 para indexação 1-based
    grafo->adj = (Vizinho **)calloc(grafo->numPovos, sizeof(Vizinho *));
    if (!grafo->adj) { perror("Erro ao alocar memória para lista de adjacência"); free(grafo); return NULL; }

    // Adiciona cada caminho como duas arestas (ida e volta)
    for (int i = 0; i < caminhos->numCaminhos; i++) {
        adicionarAresta(grafo, caminhos->caminhos[i].origem, caminhos->caminhos[i].destino, caminhos->caminhos[i].distancia);
        adicionarAresta(grafo, caminhos->caminhos[i].destino, caminhos->caminhos[i].origem, caminhos->caminhos[i].distancia);
    }
    return grafo;
}

void adicionarAresta(Grafo *grafo, int origem, int destino, int distancia) {
    Vizinho *novo = (Vizinho *)malloc(sizeof(Vizinho));
    if (!novo) { perror("Erro ao alocar memória para vizinho"); return; }
    novo->id = destino;
    novo->distancia = distancia;
    novo->prox = grafo->adj[origem]; // O novo aponta para o antigo início
    grafo->adj[origem] = novo;       // O início agora é o novo
}

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

int comparador_habilidade(const Estado *a, const Estado *b) {
    return b->habilidade - a->habilidade;
}

FilaPrioridade* criar_fila(int (*comparador)(const Estado*, const Estado*)) {
    FilaPrioridade *fila = (FilaPrioridade*)malloc(sizeof(FilaPrioridade));
    if (!fila) { perror("Erro ao alocar memória para FilaPrioridade"); return NULL; }
    fila->cabeca = NULL;
    fila->comparador = comparador;
    return fila;
}

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

void enfileirar(FilaPrioridade *fila, Estado estado) {
    NoFila *novo = (NoFila*)malloc(sizeof(NoFila));
    if (!novo) {
        perror("Erro ao alocar memória para NoFila");
        // Se não puder alocar o nó, libera os componentes do estado
        free(estado.caminho); free(estado.quantidades);
        return;
    }
    novo->estado = estado;
    novo->prox = NULL;

    // Se a fila está vazia ou o novo é o de maior prioridade
    if (!fila->cabeca || fila->comparador(&novo->estado, &fila->cabeca->estado) > 0) {
        novo->prox = fila->cabeca;
        fila->cabeca = novo;
    } else {
        // Procura a posição correta para inserção
        NoFila *atual = fila->cabeca;
        while (atual->prox && fila->comparador(&novo->estado, &atual->prox->estado) <= 0) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

Estado desenfileirar(FilaPrioridade *fila) {
    NoFila *temp = fila->cabeca;
    Estado estado = temp->estado; // Faz uma cópia do estado
    fila->cabeca = temp->prox;
    free(temp); // Libera o nó, mas NÃO seus componentes (caminho, quantidades)
    return estado;
}

int fila_vazia(FilaPrioridade *fila) {
    return fila->cabeca == NULL;
}