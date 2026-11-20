#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

/* nomes dos vertices: A=0, B=1, ... */
static char nome(int v) { return (char)('A' + v); }

/* adiciona aresta nos dois sentidos (grafo nao direcionado) */
static void aresta_dupla(GrafoListaPonderado *g, int u, int v, int peso) {
    adicionar_aresta(g, u, v, peso);
    adicionar_aresta(g, v, u, peso);
}

static void testar(const char *titulo, GrafoListaPonderado *g) {
    int n = g->V;
    printf("=== %s ===\n", titulo);

    /* ---------- Kruskal ---------- */
    int m;
    Aresta *arestas = extrair_arestas(g, &m);
    Aresta *agm = malloc((n > 1 ? n - 1 : 1) * sizeof(Aresta));
    int qtd;
    int peso_k = kruskal(n, arestas, m, agm, &qtd);

    printf("Kruskal (peso total = %d, %d arestas):\n", peso_k, qtd);
    for (int i = 0; i < qtd; i++)
        printf("  %c - %c (%d)\n", nome(agm[i].u), nome(agm[i].v), agm[i].peso);
    if (qtd < n - 1)
        printf("  Grafo desconexo: resultado e uma floresta geradora minima.\n");

    /* ---------- Prim (raiz A) ---------- */
    int *pai = malloc(n * sizeof(int));
    int *chave = malloc(n * sizeof(int));
    int peso_p = prim(g, 0, pai, chave);

    printf("Prim a partir de %c (peso total = %d):\n", nome(0), peso_p);
    for (int v = 0; v < n; v++)
        if (pai[v] != -1)
            printf("  %c - %c (%d)\n", nome(pai[v]), nome(v), chave[v]);
    if (peso_p != peso_k)
        printf("  (pesos diferentes: o grafo e desconexo, Prim cobre so a componente de %c)\n", nome(0));
    printf("\n");

    free(arestas);
    free(agm);
    free(pai);
    free(chave);
}

int main(void) {
    /* Teste 1: grafo conexo classico (9 vertices, A..I) */
    GrafoListaPonderado *g1 = criar_grafo(9);
    aresta_dupla(g1, 0, 1, 4);   /* A-B */
    aresta_dupla(g1, 0, 7, 8);   /* A-H */
    aresta_dupla(g1, 1, 2, 8);   /* B-C */
    aresta_dupla(g1, 1, 7, 11);  /* B-H */
    aresta_dupla(g1, 2, 3, 7);   /* C-D */
    aresta_dupla(g1, 2, 5, 4);   /* C-F */
    aresta_dupla(g1, 2, 8, 2);   /* C-I */
    aresta_dupla(g1, 3, 4, 9);   /* D-E */
    aresta_dupla(g1, 3, 5, 14);  /* D-F */
    aresta_dupla(g1, 4, 5, 10);  /* E-F */
    aresta_dupla(g1, 5, 6, 2);   /* F-G */
    aresta_dupla(g1, 6, 7, 1);   /* G-H */
    aresta_dupla(g1, 6, 8, 6);   /* G-I */
    aresta_dupla(g1, 7, 8, 7);   /* H-I */
    testar("Teste 1: grafo conexo", g1);
    liberar_grafo(g1);

    /* Teste 2: grafo desconexo (duas componentes: {A,B,C} e {D,E}) */
    GrafoListaPonderado *g2 = criar_grafo(5);
    aresta_dupla(g2, 0, 1, 1);
    aresta_dupla(g2, 1, 2, 2);
    aresta_dupla(g2, 0, 2, 5);
    aresta_dupla(g2, 3, 4, 3);
    testar("Teste 2: grafo desconexo", g2);
    liberar_grafo(g2);

    return 0;
}
