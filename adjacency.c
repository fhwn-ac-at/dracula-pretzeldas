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

        printf("Adding edge #%d with from=%d, to=%d, weight=%d\n", i, e.from,
               e.to, e.weight);

        assert(e.from != e.to);
        assert(e.weight > 0);
        assert(adjadency_mtx[e.from][e.to] == 0);
        assert(adjadency_mtx[e.to][e.from] == 0);

        adjadency_mtx[e.from][e.to] = e.weight;
        adjadency_mtx[e.to][e.from] = -e.weight;
    }

    printf("\n");

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
    Edge edges[] = {{0, 1, 4},  {1, 3, 7},  {2, 5, 2},  {3, 6, 9},  {4, 7, 3},
                    {5, 8, 6},  {6, 9, 1},  {7, 0, 5},  {8, 2, 8},  {9, 4, 10},
                    {4, 1, 11}, {6, 2, 14}, {0, 2, 13}, {3, 7, 4},  {4, 5, 9},
                    {5, 0, 7},  {6, 1, 15}, {7, 8, 3},  {8, 9, 12}, {9, 3, 2},
                    {0, 6, 8},  {1, 7, 10}, {2, 9, 11}, {4, 8, 14}, {5, 3, 5}};

    size_t edge_count = sizeof(edges) / sizeof(Edge);

    int** adjadency_mtx = gen_adjacency_matrix(edges, edge_count, VERTEX_COUNT);

    print_matrix(adjadency_mtx, VERTEX_COUNT);

    free_mtx(adjadency_mtx, VERTEX_COUNT);
}