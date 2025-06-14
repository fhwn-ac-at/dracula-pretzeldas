// Benjamin Eder, 210894

#include "player.h"

#include <stdio.h>
#include <stdlib.h>

Die create_die(size_t sides, size_t* odds_arr, size_t* value_arr) {
    Roll* rolls = malloc(sides * sizeof(Roll));

    if (rolls == NULL) {
        fprintf(stderr, "[ERROR] Unable to malloc Rolls!\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < sides; i++) {
        rolls[i].odds = odds_arr[i];
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

Player create_player(Die die) {
    // player starts off board
    return (Player){
        .die = die,
        .position = 0,
    };
}

size_t generate_outcome(const Die* die) {
    // TODO: impl
    return 1;
}