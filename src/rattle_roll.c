// Benjamin Eder, 210894

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
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
    Die die = create_dn(6);

    Board board = create_board(10, 10, 0, NULL);

    const size_t SIMULATION_COUNT = 1000;
    const size_t MAX_STEPS = 100;

    // print simulation args
    printf("Starting Simulations with args:\n");
    printf("- Board Size: %zu\n", board.space_count);
    printf("- Transition Count (Snakes & Ladders): %zu\n",
           board.transition_count);
    printf("- Die Sides: %zu\n", die.side_count);
    printf("- Die Outcomes: ");
    for (size_t i = 0; i < die.side_count; i++) {
        printf("v:%zu,w:%zu ", die.outcomes[i].value, die.outcomes[i].weight);
    }
    printf("\n");
    printf("- Max Steps: %zu\n", MAX_STEPS);
    printf("- Simulations: %zu\n\n", SIMULATION_COUNT);

    // run sims
    Statistics* stats = malloc(SIMULATION_COUNT * sizeof(Statistics));

    for (size_t i = 0; i < SIMULATION_COUNT; i++) {
        Engine engine = create_engine(&board, &die);
        stats[i] = simulate_game(engine, MAX_STEPS, false);
    }

    // sum up stats
    size_t* transition_totals = malloc(board.transition_count * sizeof(size_t));
    if (transition_totals == NULL) {
        fprintf(stderr, "[ERROR] Unable to allocate transition totals!\n");
        exit(EXIT_FAILURE);
    }

    size_t total_win_rolls = 0;
    size_t won_games = 0;
    size_t shortest_path_len = SIZE_MAX;
    size_t* shortest_path = NULL;
    for (size_t i = 0; i < SIMULATION_COUNT; i++) {
        Statistics stat = stats[i];
        if (stat.was_win) {
            won_games++;
            total_win_rolls += stat.roll_count;

            if (stat.roll_count < shortest_path_len) {
                // new shortest path
                shortest_path_len = stat.roll_count;
                shortest_path = stat.roll_path;
            }
        }

        for (size_t j = 0; j < board.transition_count; j++) {
            transition_totals[j] += stat.traversal_counts[j];
        }
    }

    size_t total_snake_count = 0;
    size_t total_ladder_count = 0;
    size_t snake_uses = 0;
    size_t ladder_uses = 0;
    size_t total_traversals = 0;
    for (size_t i = 0; i < board.transition_count; i++) {
        Transition transition = board.transitions[i];
        TransitionType type = get_transition_type(&transition);
        if (type == SNAKE) {
            total_snake_count++;
            snake_uses += transition_totals[i];
        } else if (type == LADDER) {
            total_ladder_count++;
            ladder_uses += transition_totals[i];
        } else {
            // sanity check
            assert(false && "Impossible Transition Type!");
        }

        total_traversals += transition_totals[i];
    }

    // print stats

    printf("=== Common/Roll Stats ===\n");
    printf("Win Rate: %.2f%%\n", won_games / (float)SIMULATION_COUNT * 100.f);
    if (shortest_path == NULL) {
        printf("No shortest path found! Board could be impossible.\n");
        assert(won_games == 0);  // sanity check
    } else {
        printf("Average rolls to win a game: %.2f\n",
               total_win_rolls / (float)SIMULATION_COUNT);
        printf("Shortest Win: %zu rolls\n", shortest_path_len);
        printf("Roll Path: \n");
        for (size_t i = 0; i < shortest_path_len; i++) {
            printf("%zu ", shortest_path[i]);
        }
        printf("\n");
    }

    printf("\n=== Transition Stats ===\n");
    printf("Number of Snakes: %zu\n", total_snake_count);
    printf("Number of Ladders: %zu\n", total_ladder_count);

    printf("\n--- Usages ---\n");

    for (size_t i = 0; i < board.transition_count; i++) {
        Transition transition = board.transitions[i];
        TransitionType type = get_transition_type(&transition);

        float rel_freq_same_group;
        float rel_freq_global = transition_totals[i] / (float)total_traversals;
        if (type == SNAKE) {
            printf("Snake");

            rel_freq_same_group = transition_totals[i] / (float)snake_uses;
        } else if (type == LADDER) {
            printf("Ladder");

            rel_freq_same_group = transition_totals[i] / (float)ladder_uses;
        } else {
            // sanity check
            assert(false && "Impossible Transition Type!");
        }

        printf(" from %zu to %zu used %zu times (%.2f%% among ",
               transition.from, transition.to, transition_totals[i],
               rel_freq_same_group);

        if (type == SNAKE) {
            printf("snakes");
        } else if (type == LADDER) {
            printf("ladders");
        } else {
            // sanity check
            assert(false && "Impossible Transition Type!");
        }

        printf(", %.2f%% of total)", rel_freq_global);
    }

    // free resources

    free(transition_totals);

    for (size_t i = 0; i < SIMULATION_COUNT; i++) {
        free_stats(stats[i]);
    }

    free(stats);
    free_board(board);
    free_die(die);
    return 0;
}