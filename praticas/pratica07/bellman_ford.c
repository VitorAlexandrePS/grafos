#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

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

/* ===================== BELLMAN-FORD ===================== */

/* Uma rodada: tenta relaxar TODAS as arestas. Retorna 1 se alguma distância melhorou. */
static int relaxar_todas(GrafoListaPonderado *g, int *dist, int *pred) {
    int mudou = 0;
    for (int u = 0; u < g->V; u++) {
        if (dist[u] == INF)
            continue; /* u ainda não foi alcançado: evita overflow de INF + peso */
        for (NoAdj *p = g->adj[u]; p != NULL; p = p->prox) {
            int v = p->destino;
            if (dist[u] + p->peso < dist[v]) {
                dist[v] = dist[u] + p->peso;
                pred[v] = u;
                mudou = 1;
            }
        }
    }
    return mudou;
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    int n = g->V;

    for (int v = 0; v < n; v++) {
        dist[v] = INF;
        pred[v] = -1;
    }
    dist[origem] = 0;

    /* Rodadas 1 .. n-1: suficientes se NÃO houver ciclo negativo.
     * Rodada n: se ainda melhorar alguma distância, há ciclo negativo. */
    for (int i = 1; i <= n; i++) {
        if (!relaxar_todas(g, dist, pred))
            return 1;          /* nada mudou: terminou (pode parar antes) */
    }
    return 0;                  /* a n-ésima rodada ainda melhorou: ciclo negativo */
}

/* ===================== CICLO NEGATIVO ===================== */

int extrair_ciclo_negativo(GrafoListaPonderado *g, int *pred, int *ciclo) {
    int n = g->V;

    for (int s = 0; s < n; s++) {
        /* retrocede n vezes pelo pred[]: se não bater em -1, caiu dentro de um ciclo */
        int x = s;
        for (int i = 0; i < n && x != -1; i++)
            x = pred[x];
        if (x == -1)
            continue;

        /* x está num ciclo: percorre pred[] até voltar a x */
        int tam = 0;
        int atual = x;
        do {
            ciclo[tam++] = atual;
            atual = pred[atual];
        } while (atual != x);

        /* pred[] anda para trás (de v para u, onde u -> v); inverte para a ordem das arestas */
        for (int i = 0, j = tam - 1; i < j; i++, j--) {
            int t = ciclo[i];
            ciclo[i] = ciclo[j];
            ciclo[j] = t;
        }
        return tam;
    }
    return 0;
}

/* menor peso entre as arestas u -> v (assume que existe) */
static int peso_aresta(GrafoListaPonderado *g, int u, int v) {
    int menor = INF;
    for (NoAdj *p = g->adj[u]; p != NULL; p = p->prox)
        if (p->destino == v && p->peso < menor)
            menor = p->peso;
    return menor;
}

int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *pred) {
    int *ciclo = malloc(g->V * sizeof(int));
    int tam = extrair_ciclo_negativo(g, pred, ciclo);

    if (tam == 0) {
        printf("Nao foi possivel reconstruir o ciclo negativo.\n");
        free(ciclo);
        return 0;
    }

    int custo = 0;
    printf("Ciclo negativo: ");
    for (int i = 0; i < tam; i++) {
        int u = ciclo[i];
        int v = ciclo[(i + 1) % tam];
        custo += peso_aresta(g, u, v);
        printf("%d -> ", u);
    }
    printf("%d\n", ciclo[0]);
    printf("Custo total do ciclo: %d\n", custo);

    free(ciclo);
    return tam;
}
