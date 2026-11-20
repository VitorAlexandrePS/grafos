#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

/* ------------------------------------------------------------------
 * INSTÂNCIA DO TSP: grafo completo com matriz de distâncias simétrica.
 * ------------------------------------------------------------------ */
typedef struct {
    int n;
    int **dist;   /* dist[i][j] = custo de ir de i até j */
} InstanciaTSP;

InstanciaTSP *criar_tsp(int n);
void liberar_tsp(InstanciaTSP *t);

/* Custo de uma rota fechada: rota[0] -> rota[1] -> ... -> rota[n-1] -> rota[0]. */
int custo_rota(InstanciaTSP *t, int *rota);

/* Redução do Ciclo Hamiltoniano para o TSP:
 * peso 1 para arestas existentes e 2 para inexistentes.
 * O grafo tem ciclo hamiltoniano se, e somente se, o TSP ótimo custa exatamente n. */
InstanciaTSP *grafo_para_tsp(GrafoMatriz *g);

/* Todas as funções abaixo gravam a rota em rota[0..n-1] (volta a rota[0] implícita)
 * e retornam o custo total da rota fechada. */

/* Força bruta: origem fixa em 0, testa as (n-1)! permutações dos demais. */
int tsp_forca_bruta(InstanciaTSP *t, int *rota);

/* Heurística construtiva: parte da origem e vai sempre para o vizinho não visitado mais próximo. */
int tsp_vizinho_mais_proximo(InstanciaTSP *t, int origem, int *rota);

/* Inserção do mais próximo: parte de um ciclo pequeno e insere, a cada passo, o vértice
 * mais próximo do ciclo na posição que menos aumenta o custo. */
int tsp_insercao(InstanciaTSP *t, int *rota);

#endif
