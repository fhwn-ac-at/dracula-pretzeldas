// Benjamin Eder, 210894

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "engine/player.h"

int main(void) {
    // TODO: Create Engine and Simualte Games
    srand(time(NULL));

    // Default D6
#define D6_FACES 6
    size_t weights[D6_FACES] = {1, 1, 0, 0, 0, 2};
    size_t values[D6_FACES] = {1, 2, 3, 4, 5, 6};
    Die d6 = create_die(D6_FACES, weights, values);
    int occurences[D6_FACES];
    memset(occurences, 0, D6_FACES * sizeof(int));

#define NUM_ROLES 10000
    for (int i = 0; i < NUM_ROLES; i++) {
        size_t outcome = generate_outcome(&d6);
        occurences[outcome - 1]++;
    }

    for (int i = 0; i < D6_FACES; i++) {
        printf("D6 [%d]: %.f%%, actual: %.f%%\n", i + 1,
               occurences[i] / (float)NUM_ROLES * 100.f, 1 / 6.f * 100.f);
    }

    free_die(d6);
    return 0;
}