#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "agm.h"

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

Aresta *extrair_arestas(GrafoListaPonderado *g, int *m) {
    int cont = 0;
    for (int u = 0; u < g->V; u++)
        for (NoAdj *p = g->adj[u]; p != NULL; p = p->prox)
            if (u < p->destino)
                cont++;

    Aresta *v = malloc((cont > 0 ? cont : 1) * sizeof(Aresta));
    int k = 0;
    for (int u = 0; u < g->V; u++)
        for (NoAdj *p = g->adj[u]; p != NULL; p = p->prox)
            if (u < p->destino) {
                v[k].u = u;
                v[k].v = p->destino;
                v[k].peso = p->peso;
                k++;
            }
    *m = cont;
    return v;
}

/* ===================== UNION-FIND ===================== */

DSU *criar_dsu(int n) {
    DSU *d = malloc(sizeof(DSU));
    d->n = n;
    d->pai = malloc(n * sizeof(int));
    d->rank = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
        d->pai[i] = i; /* cada vértice começa sendo seu próprio conjunto */
    return d;
}

/* compressão de caminho: todo nó visitado passa a apontar direto para a raiz */
int find(DSU *d, int x) {
    if (d->pai[x] != x)
        d->pai[x] = find(d, d->pai[x]);
    return d->pai[x];
}

/* união por rank: a árvore mais baixa vira filha da mais alta */
int unir(DSU *d, int x, int y) {
    int rx = find(d, x);
    int ry = find(d, y);
    if (rx == ry)
        return 0;
    if (d->rank[rx] < d->rank[ry]) {
        int t = rx;
        rx = ry;
        ry = t;
    }
    d->pai[ry] = rx;
    if (d->rank[rx] == d->rank[ry])
        d->rank[rx]++;
    return 1;
}

void liberar_dsu(DSU *d) {
    free(d->pai);
    free(d->rank);
    free(d);
}

/* ===================== KRUSKAL ===================== */

static int comparar_arestas(const void *a, const void *b) {
    const Aresta *x = a, *y = b;
    return (x->peso > y->peso) - (x->peso < y->peso);
}

int kruskal(int n, Aresta *arestas, int m, Aresta *agm, int *qtd) {
    /* ordena uma cópia para não alterar o vetor do chamador: O(E log E) */
    Aresta *copia = malloc((m > 0 ? m : 1) * sizeof(Aresta));
    if (m > 0)
        memcpy(copia, arestas, m * sizeof(Aresta));
    qsort(copia, m, sizeof(Aresta), comparar_arestas);

    DSU *d = criar_dsu(n);
    int total = 0;
    *qtd = 0;

    for (int i = 0; i < m && *qtd < n - 1; i++) {
        /* só aceita a aresta se ela liga dois conjuntos diferentes (não forma ciclo) */
        if (unir(d, copia[i].u, copia[i].v)) {
            agm[(*qtd)++] = copia[i];
            total += copia[i].peso;
        }
    }

    liberar_dsu(d);
    free(copia);
    return total;
}

/* ===================== MIN-HEAP ===================== */

MinHeap *criar_min_heap(int capacidade) {
    MinHeap *h = malloc(sizeof(MinHeap));
    h->dados = malloc(capacidade * sizeof(NoHeap));
    h->posicao = malloc(capacidade * sizeof(int));
    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int i = 0; i < capacidade; i++)
        h->posicao[i] = -1;
    return h;
}

static void trocar(MinHeap *h, int i, int j) {
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (h->dados[pai].chave <= h->dados[i].chave)
            break;
        trocar(h, i, pai);
        i = pai;
    }
}

static void descer(MinHeap *h, int i) {
    for (;;) {
        int esq = 2 * i + 1, dir = 2 * i + 2, menor = i;
        if (esq < h->tamanho && h->dados[esq].chave < h->dados[menor].chave)
            menor = esq;
        if (dir < h->tamanho && h->dados[dir].chave < h->dados[menor].chave)
            menor = dir;
        if (menor == i)
            break;
        trocar(h, i, menor);
        i = menor;
    }
}

void inserir(MinHeap *h, int vertice, int chave) {
    if (h->tamanho == h->capacidade)
        return;
    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].chave = chave;
    h->posicao[vertice] = i;
    subir(h, i);
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap raiz = h->dados[0];
    NoHeap ultimo = h->dados[h->tamanho - 1];
    h->tamanho--;
    h->posicao[raiz.vertice] = -1;
    if (h->tamanho > 0) {
        h->dados[0] = ultimo;
        h->posicao[ultimo.vertice] = 0;
        descer(h, 0);
    }
    return raiz;
}

void diminuir_chave(MinHeap *h, int vertice, int nova_chave) {
    int i = h->posicao[vertice];
    if (i == -1 || nova_chave >= h->dados[i].chave)
        return;
    h->dados[i].chave = nova_chave;
    subir(h, i);
}

void liberar_min_heap(MinHeap *h) {
    free(h->dados);
    free(h->posicao);
    free(h);
}

/* ===================== PRIM ===================== */

int prim(GrafoListaPonderado *g, int raiz, int *pai, int *chave) {
    int n = g->V;
    MinHeap *h = criar_min_heap(n);

    for (int v = 0; v < n; v++) {
        chave[v] = INF;
        pai[v] = -1;
    }
    chave[raiz] = 0;

    for (int v = 0; v < n; v++)
        inserir(h, v, chave[v]);

    int total = 0;
    while (h->tamanho > 0) {
        NoHeap u = extrair_minimo(h);
        if (u.chave == INF)
            break; /* o que sobrou é outra componente, inalcançável a partir da raiz */
        total += u.chave;

        for (NoAdj *p = g->adj[u.vertice]; p != NULL; p = p->prox) {
            int v = p->destino;
            /* só considera vértices que ainda estão fora da árvore (ainda no heap) */
            if (h->posicao[v] != -1 && p->peso < chave[v]) {
                chave[v] = p->peso;
                pai[v] = u.vertice;
                diminuir_chave(h, v, p->peso);
            }
        }
    }
    liberar_min_heap(h);
    return total;
}
