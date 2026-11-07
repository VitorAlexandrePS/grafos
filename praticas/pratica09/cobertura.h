
#ifndef COBERTURA_H
#define COBERTURA_H

typedef struct {
    int u;
    int v;
} Aresta;

// Verifica se um conjunto de vertices cobre todas as arestas.
int eh_cobertura(Aresta *arestas, int *cobertura, int m);

// Encontra uma cobertura minima utilizando backtracking.
int cobertura_minima_bt(Aresta *arestas, int n, int m, int *melhor_cobertura);

// Encontra uma cobertura utilizando a heuristica gulosa.
int cobertura_gulosa(Aresta *arestas, int n, int m, int *cobertura);

#endif
