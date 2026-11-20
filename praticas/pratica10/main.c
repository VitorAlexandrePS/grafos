#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

/* ------------------------------------------------------------------
 * Gerador pseudoaleatorio proprio: assim as instancias (e os custos) sao
 * iguais em qualquer sistema (rand() muda entre Windows e Linux).
 * ------------------------------------------------------------------ */
static uint32_t estado = 12345u;

static int aleatorio(int minimo, int maximo) {
    estado = estado * 1103515245u + 12345u;
    int r = (int)((estado >> 16) & 0x7fff);
    return minimo + r % (maximo - minimo + 1);
}

static void imprimir_rota(const int *v, int n, int fechar) {
    for (int i = 0; i < n; i++)
        printf("%d%s", v[i], i + 1 < n ? " -> " : "");
    if (fechar)
        printf(" -> %d", v[0]);
    printf("\n");
}

/* ===================== PARTE 1: HAMILTONIANO + REDUCAO ===================== */

static void testar_hamiltoniano(const char *titulo, GrafoMatriz *g) {
    int n = g->n;
    int *v = malloc(n * sizeof(int));
    int *rota = malloc(n * sizeof(int));

    printf("=== %s ===\n", titulo);

    printf("Caminho hamiltoniano: ");
    if (encontrar_caminho_hamiltoniano_bt(g, v))
        imprimir_rota(v, n, 0);
    else
        printf("nao existe\n");

    int tem_ciclo = encontrar_ciclo_hamiltoniano_bt(g, v);
    printf("Ciclo hamiltoniano:   ");
    if (tem_ciclo)
        imprimir_rota(v, n, 1);
    else
        printf("nao existe\n");

    /* reducao: peso 1 (aresta existe) / 2 (nao existe); ciclo existe <=> otimo == n */
    InstanciaTSP *t = grafo_para_tsp(g);
    int otimo = tsp_forca_bruta(t, rota);
    int via_tsp = (otimo == n);
    printf("Reducao para TSP: custo otimo = %d (n = %d) -> %s [%s]\n", otimo, n,
           via_tsp ? "existe ciclo hamiltoniano" : "nao existe ciclo hamiltoniano",
           via_tsp == tem_ciclo ? "coerente com o backtracking" : "INCOERENTE");
    printf("\n");

    liberar_tsp(t);
    free(v);
    free(rota);
}

/* ===================== PARTE 2: EXPERIMENTO TSP ===================== */

/* instancia simetrica com pesos aleatorios entre 1 e 100 */
static InstanciaTSP *instancia_aleatoria(int n) {
    InstanciaTSP *t = criar_tsp(n);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            int w = aleatorio(1, 100);
            t->dist[i][j] = w;
            t->dist[j][i] = w;
        }
    return t;
}

/* tempo medio, em ms, de 'reps' execucoes de uma funcao de TSP */
typedef int (*FuncTSP)(InstanciaTSP *, int *);

static int envolver_vmp(InstanciaTSP *t, int *rota) { return tsp_vizinho_mais_proximo(t, 0, rota); }

static double medir(FuncTSP f, InstanciaTSP *t, int *rota, int reps, int *custo) {
    clock_t ini = clock();
    for (int r = 0; r < reps; r++)
        *custo = f(t, rota);
    clock_t fim = clock();
    return 1000.0 * (double)(fim - ini) / CLOCKS_PER_SEC / reps;
}

static void experimento(void) {
    int tamanhos[] = {5, 8, 10, 12};
    int reps_fb[] = {200000, 2000, 20, 1};
    const int reps_heur = 200000;

    printf("=== Experimento TSP (pesos aleatorios 1..100, tempos em ms) ===\n");
    for (int k = 0; k < 4; k++) {
        int n = tamanhos[k];
        InstanciaTSP *t = instancia_aleatoria(n);
        int *rota = malloc(n * sizeof(int));
        int c_fb = 0, c_vmp = 0, c_ins = 0;

        double t_fb = medir(tsp_forca_bruta, t, rota, reps_fb[k], &c_fb);
        double t_vmp = medir(envolver_vmp, t, rota, reps_heur, &c_vmp);
        double t_ins = medir(tsp_insercao, t, rota, reps_heur, &c_ins);

        printf("n = %d\n", n);
        printf("  forca bruta       : custo = %3d (otimo)   tempo = %.4f ms\n", c_fb, t_fb);
        printf("  vizinho + proximo : custo = %3d (+%.1f%%)   tempo = %.4f ms\n", c_vmp,
               100.0 * (c_vmp - c_fb) / c_fb, t_vmp);
        printf("  insercao          : custo = %3d (+%.1f%%)   tempo = %.4f ms\n", c_ins,
               100.0 * (c_ins - c_fb) / c_fb, t_ins);

        free(rota);
        liberar_tsp(t);
    }
}

int main(void) {
    /* Grafo 1: pentagono 0-1-2-3-4-0 com a corda 0-2 (tem ciclo hamiltoniano) */
    GrafoMatriz *g1 = criar_grafo_matriz(5);
    for (int i = 0; i < 5; i++)
        adicionar_aresta_matriz(g1, i, (i + 1) % 5);
    adicionar_aresta_matriz(g1, 0, 2);
    testar_hamiltoniano("Grafo 1: pentagono com corda", g1);
    liberar_grafo_matriz(g1);

    /* Grafo 2: caminho 0-1-2-3-4 (tem caminho, nao tem ciclo) */
    GrafoMatriz *g2 = criar_grafo_matriz(5);
    for (int i = 0; i < 4; i++)
        adicionar_aresta_matriz(g2, i, i + 1);
    testar_hamiltoniano("Grafo 2: caminho simples", g2);
    liberar_grafo_matriz(g2);

    /* Grafo 3: estrela com centro 0 (nao tem caminho nem ciclo) */
    GrafoMatriz *g3 = criar_grafo_matriz(5);
    for (int i = 1; i < 5; i++)
        adicionar_aresta_matriz(g3, 0, i);
    testar_hamiltoniano("Grafo 3: estrela", g3);
    liberar_grafo_matriz(g3);

    experimento();
    return 0;
}
