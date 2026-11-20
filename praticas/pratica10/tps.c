#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "tps.h"

/* ===================== INSTÂNCIA ===================== */

InstanciaTSP *criar_tsp(int n) {
    InstanciaTSP *t = malloc(sizeof(InstanciaTSP));
    t->n = n;
    t->dist = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        t->dist[i] = calloc(n, sizeof(int));
    return t;
}

void liberar_tsp(InstanciaTSP *t) {
    for (int i = 0; i < t->n; i++)
        free(t->dist[i]);
    free(t->dist);
    free(t);
}

int custo_rota(InstanciaTSP *t, int *rota) {
    int n = t->n;
    int custo = 0;
    for (int i = 0; i < n; i++)
        custo += t->dist[rota[i]][rota[(i + 1) % n]];
    return custo;
}

InstanciaTSP *grafo_para_tsp(GrafoMatriz *g) {
    int n = g->n;
    InstanciaTSP *t = criar_tsp(n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
                t->dist[i][j] = g->adj[i][j] ? 1 : 2;
    return t;
}

/* ===================== FORÇA BRUTA ===================== */

typedef struct {
    InstanciaTSP *t;
    int *rota;       /* rota sendo construída */
    int *usado;
    int *melhor;     /* melhor rota até agora */
    int melhor_custo;
} Contexto;

/* rota[0..k-1] já definida, com custo parcial 'custo' (sem o retorno à origem). */
static void permutar(Contexto *c, int k, int custo) {
    int n = c->t->n;
    if (k == n) {
        int total = custo + c->t->dist[c->rota[n - 1]][c->rota[0]];
        if (total < c->melhor_custo) {
            c->melhor_custo = total;
            memcpy(c->melhor, c->rota, n * sizeof(int));
        }
        return;
    }
    for (int v = 1; v < n; v++) {
        if (!c->usado[v]) {
            c->usado[v] = 1;
            c->rota[k] = v;
            permutar(c, k + 1, custo + c->t->dist[c->rota[k - 1]][v]);
            c->usado[v] = 0;
        }
    }
}

int tsp_forca_bruta(InstanciaTSP *t, int *rota) {
    int n = t->n;
    Contexto c;
    c.t = t;
    c.rota = malloc(n * sizeof(int));
    c.usado = calloc(n, sizeof(int));
    c.melhor = rota;
    c.melhor_custo = 0x7fffffff;

    c.rota[0] = 0;   /* origem fixa: gera (n-1)! permutações em vez de n! */
    c.usado[0] = 1;
    permutar(&c, 1, 0);

    int custo = c.melhor_custo;
    free(c.rota);
    free(c.usado);
    return custo;
}

/* ===================== VIZINHO MAIS PRÓXIMO ===================== */

int tsp_vizinho_mais_proximo(InstanciaTSP *t, int origem, int *rota) {
    int n = t->n;
    int *visitado = calloc(n, sizeof(int));

    rota[0] = origem;
    visitado[origem] = 1;
    for (int k = 1; k < n; k++) {
        int atual = rota[k - 1];
        int melhor = -1;
        for (int v = 0; v < n; v++)
            if (!visitado[v] && (melhor == -1 || t->dist[atual][v] < t->dist[atual][melhor]))
                melhor = v;
        rota[k] = melhor;
        visitado[melhor] = 1;
    }
    free(visitado);
    return custo_rota(t, rota);
}

/* ===================== INSERÇÃO DO MAIS PRÓXIMO ===================== */

int tsp_insercao(InstanciaTSP *t, int *rota) {
    int n = t->n;
    if (n == 1) {
        rota[0] = 0;
        return 0;
    }
    int *na_rota = calloc(n, sizeof(int));

    /* ciclo inicial: o vértice 0 e o vértice mais próximo dele */
    int j = 1;
    for (int v = 2; v < n; v++)
        if (t->dist[0][v] < t->dist[0][j])
            j = v;
    rota[0] = 0;
    rota[1] = j;
    na_rota[0] = na_rota[j] = 1;
    int tam = 2;

    while (tam < n) {
        /* 1) vértice fora da rota mais próximo de algum vértice da rota */
        int escolhido = -1, menor_dist = 0;
        for (int v = 0; v < n; v++) {
            if (na_rota[v])
                continue;
            int d = INT_MAX;
            for (int i = 0; i < tam; i++)
                if (t->dist[rota[i]][v] < d)
                    d = t->dist[rota[i]][v];
            if (escolhido == -1 || d < menor_dist) {
                escolhido = v;
                menor_dist = d;
            }
        }

        /* 2) posição de inserção (entre rota[i] e rota[i+1]) de menor aumento de custo */
        int melhor_pos = 0, menor_aumento = 0;
        for (int i = 0; i < tam; i++) {
            int a = rota[i], b = rota[(i + 1) % tam];
            int aumento = t->dist[a][escolhido] + t->dist[escolhido][b] - t->dist[a][b];
            if (i == 0 || aumento < menor_aumento) {
                menor_aumento = aumento;
                melhor_pos = i;
            }
        }

        /* 3) insere escolhido logo após rota[melhor_pos] */
        for (int i = tam; i > melhor_pos + 1; i--)
            rota[i] = rota[i - 1];
        rota[melhor_pos + 1] = escolhido;
        na_rota[escolhido] = 1;
        tam++;
    }
    free(na_rota);
    return custo_rota(t, rota);
}
