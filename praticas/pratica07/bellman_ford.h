#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

#define INF INT_MAX

/* ------------------------------------------------------------------
 * GRAFO PONDERADO (lista de adjacência)
 * ATENÇÃO: se você já tem essa estrutura de uma prática anterior,
 * apague este bloco e use a sua (ajuste os nomes dos campos em bellman_ford.c).
 * ------------------------------------------------------------------ */
typedef struct NoAdj {
    int destino;
    int peso;               /* pode ser negativo */
    struct NoAdj *prox;
} NoAdj;

typedef struct {
    int V;                  /* número de vértices */
    NoAdj **adj;            /* vetor de listas de adjacência */
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo(int V);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso); /* direcionada u -> v */
void liberar_grafo(GrafoListaPonderado *g);

/* ------------------------------------------------------------------
 * BELLMAN-FORD
 * ------------------------------------------------------------------ */

/* Retorna 1 se terminou normalmente, 0 se existe ciclo negativo alcançável da origem. */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Encontra um vértice que está em um ciclo do vetor pred[] (retrocedendo n vezes)
 * e grava o ciclo em ciclo[] na ordem das arestas (sem repetir o primeiro vértice).
 * Retorna o tamanho do ciclo, ou 0 se não achou nenhum. */
int extrair_ciclo_negativo(GrafoListaPonderado *g, int *pred, int *ciclo);

/* Imprime o ciclo negativo (ex.: 1 -> 2 -> 3 -> 1) e o custo total.
 * Retorna o tamanho do ciclo (0 se não achou). */
int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *pred);

#endif
