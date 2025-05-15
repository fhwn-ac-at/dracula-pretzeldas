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

void print_matrix(size_t size, int matrix[size][size]) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
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

#define VERTEX_COUNT 10

void gen_adjacency_matrix(size_t mtx_size, size_t edges_count,
                          Edge const edges[edges_count],
                          int adjadency_mtx[mtx_size][mtx_size]) {
    assert(mtx_size > 1);
    assert(mtx_size >= edges_count);
    assert(edges != NULL);
    assert(adjadency_mtx != NULL);

    for (int i = 0; i < edges_count && edges[i].weight != -1; i++) {
        Edge const e = edges[i];

        assert(e.from != e.to);
        assert(e.weight > 0);
        assert(adjadency_mtx[e.from][e.to] == 0);
        assert(adjadency_mtx[e.to][e.from] == 0);

        adjadency_mtx[e.from][e.to] = e.weight;
        adjadency_mtx[e.to][e.from] = -e.weight;
    }
}

int main(void) {
    int adjadency_mtx[VERTEX_COUNT][VERTEX_COUNT] = {0};

    Edge edges[] = {
        {0, 1, 1}, {1, 2, 3}, {2, 3, 60}, {3, 4, 5}, {5, 2, 42},
    };

    size_t edge_count = sizeof(edges) / sizeof(Edge);

    gen_adjacency_matrix(VERTEX_COUNT, edge_count, edges, adjadency_mtx);

    print_matrix(VERTEX_COUNT, adjadency_mtx);
}