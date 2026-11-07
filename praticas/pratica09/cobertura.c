
#include <stdlib.h>
#include <string.h>
#include "cobertura.h"

// Verifica se todas as arestas possuem pelo menos
// uma extremidade selecionada na cobertura.
int eh_cobertura(Aresta *arestas, int *cobertura, int m) {
    for (int i = 0; i < m; i++) {
        int u = arestas[i].u;
        int v = arestas[i].v;

        if (!cobertura[u] && !cobertura[v]) {
            return 0;
        }
    }

    return 1;
}

// Estrutura utilizada pelo backtracking.
typedef struct {
    Aresta *arestas;
    int n;
    int m;
    int *atual;
    int *melhor;
    int melhor_tamanho;
} DadosBT;

// Busca recursivamente a cobertura minima.
static void buscar_cobertura(DadosBT *dados, int vertice, int quantidade) {

    // Poda: se ja selecionamos vertices demais,
    // nao existe possibilidade de melhorar a solucao.
    if (quantidade >= dados->melhor_tamanho) {
        return;
    }

    // Todos os vertices foram analisados.
    if (vertice == dados->n) {

        // Verifica se encontramos uma cobertura valida.
        if (eh_cobertura(dados->arestas, dados->atual, dados->m)) {
            dados->melhor_tamanho = quantidade;

            memcpy(
                dados->melhor,
                dados->atual,
                dados->n * sizeof(int)
            );
        }

        return;
    }

    // Primeira possibilidade: nao selecionar o vertice.
    dados->atual[vertice] = 0;

    buscar_cobertura(dados, vertice + 1, quantidade);

    // Segunda possibilidade: selecionar o vertice.
    dados->atual[vertice] = 1;

    buscar_cobertura(dados, vertice + 1, quantidade + 1);

    // Restaura o estado para a proxima chamada.
    dados->atual[vertice] = 0;
}

// Funcao principal do backtracking.
int cobertura_minima_bt(
    Aresta *arestas,
    int n,
    int m,
    int *melhor_cobertura
) {
    if (n < 0 || m < 0) {
        return -1;
    }

    int *atual = calloc(n > 0 ? n : 1, sizeof(int));

    if (atual == NULL) {
        return -1;
    }

    // Inicialmente, o melhor tamanho e n + 1.
    // Isso permite que qualquer cobertura valida
    // seja considerada uma melhoria.
    DadosBT dados = {
        arestas,
        n,
        m,
        atual,
        melhor_cobertura,
        n + 1
    };

    buscar_cobertura(&dados, 0, 0);

    free(atual);

    return dados.melhor_tamanho <= n
        ? dados.melhor_tamanho
        : -1;
}

// Heuristica gulosa: seleciona o vertice
// com maior grau entre as arestas ainda descobertas.
int cobertura_gulosa(
    Aresta *arestas,
    int n,
    int m,
    int *cobertura
) {
    if (n < 0 || m < 0) {
        return -1;
    }

    int *graus = calloc(n > 0 ? n : 1, sizeof(int));

    if (graus == NULL) {
        return -1;
    }

    memset(cobertura, 0, n * sizeof(int));

    // Enquanto houver arestas descobertas.
    while (!eh_cobertura(arestas, cobertura, m)) {

        memset(graus, 0, n * sizeof(int));

        // Calcula os graus considerando apenas
        // as arestas que ainda nao foram cobertas.
        for (int i = 0; i < m; i++) {
            int u = arestas[i].u;
            int v = arestas[i].v;

            if (!cobertura[u] && !cobertura[v]) {
                graus[u]++;
                graus[v]++;
            }
        }

        // Procura o vertice de maior grau.
        int maior_grau = 0;
        int escolhido = -1;

        for (int i = 0; i < n; i++) {
            if (!cobertura[i] && graus[i] > maior_grau) {
                maior_grau = graus[i];
                escolhido = i;
            }
        }

        // Seguranca: evita um loop infinito
        // caso nao seja possivel encontrar um vertice.
        if (escolhido == -1) {
            free(graus);
            return -1;
        }

        // Seleciona o vertice escolhido.
        cobertura[escolhido] = 1;
    }

    free(graus);

    int quantidade = 0;

    for (int i = 0; i < n; i++) {
        quantidade += cobertura[i];
    }

    return quantidade;
}
