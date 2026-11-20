#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

/* ------------------------------------------------------------------
 * GRAFO NÃO DIRECIONADO (matriz de adjacência: adj[u][v] = 1 se existe aresta)
 * ATENÇÃO: se você já tem uma estrutura de grafo de uma prática anterior,
 * pode adaptar este bloco e as funções de hamiltoniano.c para ela.
 * ------------------------------------------------------------------ */
typedef struct {
    int n;
    int **adj;
} GrafoMatriz;

GrafoMatriz *criar_grafo_matriz(int n);
void adicionar_aresta_matriz(GrafoMatriz *g, int u, int v); /* nos dois sentidos */
void liberar_grafo_matriz(GrafoMatriz *g);

/* ------------------------------------------------------------------
 * BACKTRACKING
 * Os vetores caminho[] / ciclo[] devem ter espaço para n inteiros.
 * Retornam 1 se encontraram, 0 caso contrário.
 * ------------------------------------------------------------------ */

/* Caminho que passa por todos os vértices exatamente uma vez (qualquer início). */
int encontrar_caminho_hamiltoniano_bt(GrafoMatriz *g, int *caminho);

/* Ciclo hamiltoniano: caminho hamiltoniano começando em 0 cujo último vértice
 * tem aresta de volta para 0. O ciclo fica em ciclo[0..n-1] (o retorno a 0 é implícito). */
int encontrar_ciclo_hamiltoniano_bt(GrafoMatriz *g, int *ciclo);

#endif
