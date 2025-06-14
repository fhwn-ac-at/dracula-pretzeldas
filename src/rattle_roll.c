// Benjamin Eder, 210894

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "engine/board.h"
#include "engine/die.h"
#include "engine/engine.h"

int main(void) {
    srand(time(NULL));

    // Default D6
#define D6_FACES 6
    size_t weights[D6_FACES] = {1, 1, 1, 1, 1, 1};
    size_t values[D6_FACES] = {1, 2, 3, 4, 5, 6};
    Die d6 = create_die(D6_FACES, weights, values);
    int occurences[D6_FACES];
    memset(occurences, 0, D6_FACES * sizeof(int));

    Board board = create_board(10, 10, 0, NULL);
    Engine engine = create_engine(&board, &d6);

    Statistics stats = simulate_game(engine, 500, false);

    printf("Stats: roll_count=%zu, win=%d\n", stats.roll_count, stats.was_win);

    printf("Roll Path: ");
    for (int i = 0; i < stats.roll_count; i++) {
        printf("%zu ", stats.roll_path[i]);
    }
    printf("\n");

    free_board(board);
    free_die(d6);
    return 0;
}