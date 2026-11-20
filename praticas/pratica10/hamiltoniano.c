#include <stdlib.h>
#include "hamiltoniano.h"

/* ===================== GRAFO ===================== */

GrafoMatriz *criar_grafo_matriz(int n) {
    GrafoMatriz *g = malloc(sizeof(GrafoMatriz));
    g->n = n;
    g->adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        g->adj[i] = calloc(n, sizeof(int));
    return g;
}

void adicionar_aresta_matriz(GrafoMatriz *g, int u, int v) {
    if (u == v)
        return; /* ignora laços */
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void liberar_grafo_matriz(GrafoMatriz *g) {
    for (int i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

/* ===================== BACKTRACKING ===================== */

/* Já existem k vértices em caminho[0..k-1]. Tenta estender com cada vértice
 * ainda não usado que seja vizinho do último; se não der, desfaz e tenta o próximo. */
static int estender_caminho(GrafoMatriz *g, int *caminho, int *usado, int k) {
    int n = g->n;
    if (k == n)
        return 1;
    int ultimo = caminho[k - 1];
    for (int v = 0; v < n; v++) {
        if (!usado[v] && g->adj[ultimo][v]) {
            usado[v] = 1;
            caminho[k] = v;
            if (estender_caminho(g, caminho, usado, k + 1))
                return 1;
            usado[v] = 0; /* backtrack */
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(GrafoMatriz *g, int *caminho) {
    int n = g->n;
    if (n == 0)
        return 0;
    int *usado = calloc(n, sizeof(int));
    for (int inicio = 0; inicio < n; inicio++) {
        usado[inicio] = 1;
        caminho[0] = inicio;
        if (estender_caminho(g, caminho, usado, 1)) {
            free(usado);
            return 1;
        }
        usado[inicio] = 0;
    }
    free(usado);
    return 0;
}

static int estender_ciclo(GrafoMatriz *g, int *ciclo, int *usado, int k) {
    int n = g->n;
    if (k == n)
        return g->adj[ciclo[n - 1]][ciclo[0]]; /* precisa fechar o ciclo */
    int ultimo = ciclo[k - 1];
    for (int v = 1; v < n; v++) {
        if (!usado[v] && g->adj[ultimo][v]) {
            usado[v] = 1;
            ciclo[k] = v;
            if (estender_ciclo(g, ciclo, usado, k + 1))
                return 1;
            usado[v] = 0;
        }
    }
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(GrafoMatriz *g, int *ciclo) {
    int n = g->n;
    if (n < 3)
        return 0; /* ciclo simples precisa de pelo menos 3 vértices */
    int *usado = calloc(n, sizeof(int));
    usado[0] = 1;
    ciclo[0] = 0; /* o ciclo pode começar em qualquer vértice: fixamos o 0 */
    int ok = estender_ciclo(g, ciclo, usado, 1);
    free(usado);
    return ok;
}
