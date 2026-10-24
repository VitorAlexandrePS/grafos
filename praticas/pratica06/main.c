#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

/* Vertices: A=0, B=1, C=2, D=3, E=4, F=5 */
static const char NOME[] = {'A', 'B', 'C', 'D', 'E', 'F'};

/* adiciona aresta nos dois sentidos (grafo nao direcionado) */
static void aresta_dupla(GrafoListaPonderado *g, int u, int v, int peso) {
    adicionar_aresta(g, u, v, peso);
    adicionar_aresta(g, v, u, peso);
}

/* imprime o caminho com letras, seguindo pred[] */
static void imprimir_caminho_letras(int origem, int v, int *pred) {
    if (v == origem) {
        printf("%c", NOME[v]);
        return;
    }
    imprimir_caminho_letras(origem, pred[v], pred);
    printf(" -> %c", NOME[v]);
}

int main(void) {
    int V = 6;
    GrafoListaPonderado *g = criar_grafo(V);

    /* arestas da foto */
    aresta_dupla(g, 0, 1, 4);   /* A-B */
    aresta_dupla(g, 0, 2, 2);   /* A-C */
    aresta_dupla(g, 1, 2, 1);   /* B-C */
    aresta_dupla(g, 1, 3, 5);   /* B-D */
    aresta_dupla(g, 1, 4, 8);   /* B-E */
    aresta_dupla(g, 2, 3, 4);   /* C-D */
    aresta_dupla(g, 2, 4, 10);  /* C-E */
    aresta_dupla(g, 3, 4, 2);   /* D-E */
    aresta_dupla(g, 3, 5, 1);   /* D-F */
    aresta_dupla(g, 4, 5, 3);   /* E-F */

    int origem = 0; /* A */
    int dist[6], pred[6];
    dijkstra(g, origem, dist, pred);

    printf("Distancias a partir de %c:\n", NOME[origem]);
    for (int v = 0; v < V; v++)
        printf("  %c: %d\n", NOME[v], dist[v]);

    printf("\nCaminhos minimos:\n");
    for (int v = 0; v < V; v++) {
        printf("  %c ate %c (custo %d): ", NOME[origem], NOME[v], dist[v]);
        imprimir_caminho_letras(origem, v, pred);
        printf("\n");
        /* tambem testa a funcao obrigatoria (imprime com numeros) */
        printf("      [reconstruir_caminho] ");
        reconstruir_caminho(origem, v, pred);
    }

    liberar_grafo(g);
    return 0;
}