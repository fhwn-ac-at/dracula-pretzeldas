// Benjamin Eder, 210894

#include "die.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Die create_die(size_t sides, size_t* odds_arr, size_t* value_arr) {
    Roll* rolls = malloc(sides * sizeof(Roll));

    if (rolls == NULL) {
        fprintf(stderr, "[ERROR] Unable to malloc Rolls!\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < sides; i++) {
        rolls[i].weight = odds_arr[i];
        rolls[i].value = value_arr[i];
    }

    return (Die){
        .side_count = sides,
        .outcomes = rolls,
    };
}

void free_die(Die die) {
    free(die.outcomes);
}

size_t generate_outcome(const Die* die) {
    size_t weight_total = 0;
    for (size_t i = 0; i < die->side_count; i++) {
        weight_total += die->outcomes[i].weight;
    }

    if (weight_total == 0) {
        fprintf(stderr, "[ERROR] Weights for Die total to 0!\n");
        exit(EXIT_FAILURE);
    }

    size_t rand_weight = rand() % weight_total;
    // if select interval that rand_weight falls in
    for (size_t i = 0; i < die->side_count; i++) {
        if (rand_weight < die->outcomes[i].weight) {
            return die->outcomes[i].value;
        }
        rand_weight -= die->outcomes[i].weight;
    }

    fprintf(stderr, "[ERROR] Impossible Die outcome! rand_weight: %zu\n",
            rand_weight);
    exit(EXIT_FAILURE);
}

Die create_dn(size_t n) {
    size_t weights[n];
    size_t values[n];

    for (size_t i = 0; i < n; i++) {
        weights[i] = 1;
        values[i] = i + 1;
    }

    return create_die(n, weights, values);
}