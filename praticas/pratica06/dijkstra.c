#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

/* ===================== GRAFO ===================== */

GrafoListaPonderado *criar_grafo(int V) {
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    g->V = V;
    g->adj = calloc(V, sizeof(NoAdj *));
    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    NoAdj *novo = malloc(sizeof(NoAdj));
    novo->destino = v;
    novo->peso = peso;
    novo->prox = g->adj[u];
    g->adj[u] = novo;
}

void liberar_grafo(GrafoListaPonderado *g) {
    for (int i = 0; i < g->V; i++) {
        NoAdj *p = g->adj[i];
        while (p) {
            NoAdj *prox = p->prox;
            free(p);
            p = prox;
        }
    }
    free(g->adj);
    free(g);
}

/* ===================== MIN-HEAP ===================== */

MinHeap *criar_min_heap(int capacidade) {
    MinHeap *h = malloc(sizeof(MinHeap));
    h->dados = malloc(capacidade * sizeof(NoHeap));
    h->posicao = malloc(capacidade * sizeof(int));
    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int i = 0; i < capacidade; i++)
        h->posicao[i] = -1; /* nenhum vértice está no heap ainda */
    return h;
}

/* troca dois nós do heap e mantém o vetor posicao[] consistente */
static void trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

/* sobe o nó i até restaurar a propriedade de min-heap: O(log V) */
static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai].distancia <= h->dados[i].distancia)
            break;
        trocar(h, i, pai);
        i = pai;
    }
}

/* desce o nó i até restaurar a propriedade de min-heap: O(log V) */
static void descer(MinHeap *h, int i) {
    for (;;) {
        int esq = 2 * i + 1, dir = 2 * i + 2, menor = i;
        if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia)
            menor = esq;
        if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia)
            menor = dir;
        if (menor == i)
            break;
        trocar(h, i, menor);
        i = menor;
    }
}

void inserir(MinHeap *h, int vertice, int distancia) {
    if (h->tamanho == h->capacidade)
        return;
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;
    subir(h, i);
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap raiz = h->dados[0];
    NoHeap ultimo = h->dados[h->tamanho - 1];
    h->tamanho--;
    h->posicao[raiz.vertice] = -1; /* saiu do heap */
    if (h->tamanho > 0) {
        h->dados[0] = ultimo;
        h->posicao[ultimo.vertice] = 0;
        descer(h, 0);
    }
    return raiz;
}

void diminuir_chave(MinHeap *h, int vertice, int nova_distancia) {
    int i = h->posicao[vertice];          /* O(1) graças ao mapeamento */
    if (i == -1 || nova_distancia >= h->dados[i].distancia)
        return;
    h->dados[i].distancia = nova_distancia;
    subir(h, i);                          /* O(log V) */
}

void liberar_min_heap(MinHeap *h) {
    free(h->dados);
    free(h->posicao);
    free(h);
}

/* ===================== DIJKSTRA ===================== */

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int V = g->V;
    MinHeap *h = criar_min_heap(V);

    for (int v = 0; v < V; v++) {
        dist[v] = INF;
        pred[v] = -1;
    }
    dist[origem] = 0;

    for (int v = 0; v < V; v++)
        inserir(h, v, dist[v]);

    while (h->tamanho > 0) {
        NoHeap u = extrair_minimo(h);
        if (u.distancia == INF)
            break; /* os vértices restantes são inalcançáveis */

        for (NoAdj *p = g->adj[u.vertice]; p != NULL; p = p->prox) {
            int v = p->destino;
            /* só relaxa se v ainda está no heap (não foi finalizado) */
            if (h->posicao[v] != -1 && dist[u.vertice] + p->peso < dist[v]) {
                dist[v] = dist[u.vertice] + p->peso;
                pred[v] = u.vertice;
                diminuir_chave(h, v, dist[v]);
            }
        }
    }
    liberar_min_heap(h);
}

/* imprime o caminho origem -> ... -> v seguindo os predecessores */
static void imprimir_caminho(int origem, int v, int *pred) {
    if (v == origem) {
        printf("%d", v);
        return;
    }
    imprimir_caminho(origem, pred[v], pred);
    printf(" -> %d", v);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (destino != origem && pred[destino] == -1) {
        printf("Nao existe caminho de %d ate %d\n", origem, destino);
        return;
    }
    imprimir_caminho(origem, destino, pred);
    printf("\n");
}