#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

static void executar(const char *titulo, GrafoListaPonderado *g, int origem) {
    int V = g->V;
    int *dist = malloc(V * sizeof(int));
    int *pred = malloc(V * sizeof(int));

    printf("=== %s (origem %d) ===\n", titulo, origem);

    if (bellman_ford(g, origem, dist, pred)) {
        printf("Sem ciclo negativo. Distancias:\n");
        for (int v = 0; v < V; v++) {
            if (dist[v] == INF)
                printf("  %d: INF\n", v);
            else
                printf("  %d: %d (pred = %d)\n", v, dist[v], pred[v]);
        }
    } else {
        printf("Ciclo de custo negativo detectado!\n");
        reconstruir_ciclo_negativo(g, pred);
    }
    printf("\n");

    free(dist);
    free(pred);
}

int main(void) {
    /* Teste 1: tem arestas negativas, mas NAO tem ciclo negativo */
    GrafoListaPonderado *g1 = criar_grafo(5);
    adicionar_aresta(g1, 0, 1, 6);
    adicionar_aresta(g1, 0, 3, 7);
    adicionar_aresta(g1, 1, 2, 5);
    adicionar_aresta(g1, 1, 3, 8);
    adicionar_aresta(g1, 1, 4, -4);
    adicionar_aresta(g1, 2, 1, -2);
    adicionar_aresta(g1, 3, 2, -3);
    adicionar_aresta(g1, 3, 4, 9);
    adicionar_aresta(g1, 4, 0, 2);
    adicionar_aresta(g1, 4, 2, 7);
    executar("Teste 1: sem ciclo negativo", g1, 0);
    liberar_grafo(g1);

    /* Teste 2: ciclo 1 -> 2 -> 3 -> 1 de custo total -3 */
    GrafoListaPonderado *g2 = criar_grafo(6);
    adicionar_aresta(g2, 0, 1, 1);
    adicionar_aresta(g2, 1, 2, -1);
    adicionar_aresta(g2, 2, 3, -1);
    adicionar_aresta(g2, 3, 1, -1);
    adicionar_aresta(g2, 3, 4, 2);
    /* vertice 5 fica isolado */
    executar("Teste 2: com ciclo negativo", g2, 0);
    liberar_grafo(g2);

    return 0;
}
