// Benjamin Eder, 210894

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Edge {
    int from;
    int to;
    int weight;
} Edge;

#define YLW "\x1b[93m"
#define RST "\x1b[0m"
#define BLK "\x1b[1;30m"
#define BLU "\x1b[1;34m"
#define RED "\x1b[31m"

void print_matrix(int** matrix, size_t mtx_size) {
    for (int i = 0; i < mtx_size; i++) {
        for (int j = 0; j < mtx_size; j++) {
            int const val = matrix[i][j];

            if (val > 0) printf("%s", BLU);
            if (val < 0) printf("%s", RED);
            if (val == 0) printf("%s", BLK);

            if (i == j) printf("%s", YLW);

            printf("%4d", val);

            printf("%s", RST);
        }
        printf("\n");
    }
}

int** gen_adjacency_matrix(Edge const* edges, size_t edges_count,
                           size_t mtx_size) {
    assert(mtx_size > 1);
    assert(edges != NULL);

    int** adjadency_mtx = malloc(sizeof(int*) * mtx_size);
    if (adjadency_mtx == NULL) {
        fprintf(stderr, "[ERROR] Unable to malloc matrix!\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < mtx_size; i++) {
        adjadency_mtx[i] = calloc(sizeof(int), mtx_size);

        if (adjadency_mtx == NULL) {
            fprintf(stderr, "[ERROR] Unable to malloc matrix!\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < edges_count && edges[i].weight != -1; i++) {
        Edge const e = edges[i];

        assert(e.from != e.to);
        assert(e.weight > 0);
        assert(adjadency_mtx[e.from][e.to] == 0);
        assert(adjadency_mtx[e.to][e.from] == 0);

        adjadency_mtx[e.from][e.to] = e.weight;
        adjadency_mtx[e.to][e.from] = -e.weight;
    }

    return adjadency_mtx;
}

void free_mtx(int** mtx, size_t mtx_size) {
    for (int i = 0; i < mtx_size; i++) {
        free(mtx[i]);
    }

    free(mtx);
}

#define VERTEX_COUNT 10

int main(void) {
    Edge edges[] = {
        {0, 1, 1}, {1, 2, 3}, {2, 3, 60}, {3, 4, 5}, {5, 2, 42},
    };

    size_t edge_count = sizeof(edges) / sizeof(Edge);

    int** adjadency_mtx = gen_adjacency_matrix(edges, edge_count, VERTEX_COUNT);

    print_matrix(adjadency_mtx, VERTEX_COUNT);

    free_mtx(adjadency_mtx, VERTEX_COUNT);
}