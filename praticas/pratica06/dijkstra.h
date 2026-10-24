#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

#define INF INT_MAX

/* ------------------------------------------------------------------
 * GRAFO PONDERADO (lista de adjacência)
 * ATENÇÃO: se você já tem essa estrutura de uma prática anterior,
 * apague este bloco e use a sua (ajuste os nomes dos campos em dijkstra.c).
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

/* ------------------------------------------------------------------
 * MIN-HEAP (estruturas obrigatórias)
 * ------------------------------------------------------------------ */
typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; /* mapeia vértice -> índice no heap (-1 se não está no heap) */
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
void inserir(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia);
void liberar_min_heap(MinHeap *h);

/* ------------------------------------------------------------------
 * DIJKSTRA
 * ------------------------------------------------------------------ */
void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif