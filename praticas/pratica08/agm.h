#ifndef AGM_H
#define AGM_H

#include <limits.h>

#define INF INT_MAX

/* ------------------------------------------------------------------
 * ESTRUTURAS OBRIGATÓRIAS
 * ------------------------------------------------------------------ */
typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

/* ------------------------------------------------------------------
 * GRAFO PONDERADO (lista de adjacência) - usado pelo Prim
 * ATENÇÃO: se você já tem essa estrutura de uma prática anterior,
 * apague este bloco e use a sua (ajuste os nomes dos campos em agm.c).
 * ------------------------------------------------------------------ */
typedef struct NoAdj {
    int destino;
    int peso;
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    int V;          /* número de vértices */
    NoAdj **adj;    /* vetor de listas de adjacência */
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo(int V);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso); /* direcionada u -> v */
void liberar_grafo(GrafoListaPonderado *g);

/* Gera o vetor de arestas (cada aresta não direcionada uma única vez, com u < v).
 * O chamador deve dar free() no vetor retornado. */
Aresta *extrair_arestas(GrafoListaPonderado *g, int *m);

/* ------------------------------------------------------------------
 * UNION-FIND (DSU)
 * ------------------------------------------------------------------ */
DSU *criar_dsu(int n);
int find(DSU *d, int x);          /* com compressão de caminho */
int unir(DSU *d, int x, int y);   /* por rank; retorna 1 se uniu, 0 se já estavam juntos */
void liberar_dsu(DSU *d);

/* ------------------------------------------------------------------
 * KRUSKAL: O(E log E)
 * Grava as arestas escolhidas em agm[] (capacidade n-1) e a quantidade em *qtd.
 * Retorna o peso total. Se o grafo for desconexo, obtém uma floresta (*qtd < n-1).
 * ------------------------------------------------------------------ */
int kruskal(int n, Aresta *arestas, int m, Aresta *agm, int *qtd);

/* ------------------------------------------------------------------
 * MIN-HEAP (usado pelo Prim)
 * ------------------------------------------------------------------ */
typedef struct {
    int vertice;
    int chave;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; /* mapeia vértice -> índice no heap (-1 se não está no heap) */
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
void inserir(MinHeap *h, int vertice, int chave);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_chave);
void liberar_min_heap(MinHeap *h);

/* ------------------------------------------------------------------
 * PRIM: O((V+E) log V)
 * Preenche pai[] (-1 na raiz e nos vértices fora da componente da raiz)
 * e chave[] (peso da aresta que liga cada vértice à árvore).
 * Retorna o peso total da árvore da componente da raiz.
 * ------------------------------------------------------------------ */
int prim(GrafoListaPonderado *g, int raiz, int *pai, int *chave);

#endif
