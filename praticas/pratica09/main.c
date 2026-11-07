
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include "cobertura.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

#define MAX_VERTICES 15
#define MAX_ARESTAS (MAX_VERTICES * (MAX_VERTICES - 1) / 2)
#define REPETICOES 100

// Gera um grafo simples e nao direcionado.
int gerar_grafo(int n, Aresta *arestas) {
    int m = 0;

    // Adiciona as arestas de um ciclo.
    for (int i = 0; i < n; i++) {
        int proximo = (i + 1) % n;

        arestas[m].u = i;
        arestas[m].v = proximo;
        m++;
    }

    // Adiciona algumas arestas extras.
    for (int i = 0; i < n; i++) {
        for (int j = i + 2; j < n; j++) {

            // Evita adicionar novamente a aresta
            // que fecha o ciclo.
            if (i == 0 && j == n - 1) {
                continue;
            }

            if ((i * 13 + j * 7 + n * 3) % 7 < 3) {
                arestas[m].u = i;
                arestas[m].v = j;
                m++;
            }
        }
    }

    return m;
}

double obter_tempo_ms(void) {

#ifdef _WIN32

    LARGE_INTEGER contador;
    LARGE_INTEGER frequencia;

    QueryPerformanceFrequency(&frequencia);
    QueryPerformanceCounter(&contador);

    return (double)contador.QuadPart * 1000.0
           / frequencia.QuadPart;

#else

    struct timespec tempo;

    clock_gettime(CLOCK_MONOTONIC, &tempo);

    return (double)tempo.tv_sec * 1000.0
           + (double)tempo.tv_nsec / 1000000.0;

#endif
}

// Executa os experimentos para um determinado tamanho.
void executar_experimento(int n) {
    Aresta arestas[MAX_ARESTAS];

    int melhor_cobertura[MAX_VERTICES] = {0};
    int cobertura_gulosa_resultado[MAX_VERTICES] = {0};

    int m = gerar_grafo(n, arestas);

    int tamanho_bt = 0;
    int tamanho_guloso = 0;

    double inicio, fim;

// Mede o tempo do backtracking.
inicio = obter_tempo_ms();

for (int i = 0; i < REPETICOES; i++) {
    tamanho_bt = cobertura_minima_bt(
        arestas,
        n,
        m,
        melhor_cobertura
    );
}

fim = obter_tempo_ms();

double tempo_bt = (fim - inicio) / REPETICOES;


// Mede o tempo da heuristica gulosa.
inicio = obter_tempo_ms();

for (int i = 0; i < REPETICOES; i++) {
    tamanho_guloso = cobertura_gulosa(
        arestas,
        n,
        m,
        cobertura_gulosa_resultado
    );
}

fim = obter_tempo_ms();

double tempo_guloso = (fim - inicio) / REPETICOES;

    // Exibe os resultados.
    printf("\n========================================\n");
    printf("Grafo com %d vertices e %d arestas\n", n, m);
    printf("========================================\n");

    printf("Backtracking:\n");
    printf("  Tamanho da cobertura: %d\n", tamanho_bt);
    printf("  Tempo medio: %.3f us\n", tempo_bt * 1000);

    printf("\nHeuristica gulosa:\n");
    printf("  Tamanho da cobertura: %d\n", tamanho_guloso);
    printf("  Tempo medio: %.3f us\n", tempo_guloso * 1000);

    // Confere se os resultados sao coberturas validas.
    printf("\nValidacao:\n");

    printf("  Backtracking: %s\n",
        eh_cobertura(arestas, melhor_cobertura, m)
        ? "Cobertura valida"
        : "Cobertura invalida"
    );

    printf("  Guloso: %s\n",
        eh_cobertura(arestas, cobertura_gulosa_resultado, m)
        ? "Cobertura valida"
        : "Cobertura invalida"
    );
}

int main(void) {
    int tamanhos[] = {5, 8, 10, 12, 15};
    int quantidade = sizeof(tamanhos) / sizeof(tamanhos[0]);

    printf("========================================\n");
    printf("     PRATICA 09 - COBERTURA DE VERTICES\n");
    printf("========================================\n");

    for (int i = 0; i < quantidade; i++) {
        executar_experimento(tamanhos[i]);
    }

    printf("\nExperimentos finalizados!\n");

    return 0;
}
