// Benjamin Eder, 210894

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cli_args.h"
#include "engine/board.h"
#include "engine/die.h"
#include "engine/engine.h"

int main(int argc, char* argv[]) {
    CliArgs args = parse_cli_args(argc, argv);

    srand(time(NULL));

    Board board = create_board(args.board_x, args.board_y,
                               args.transitions_count, args.transitions);

    // print simulation args
    printf("Starting Simulations with args:\n");
    printf("- Board Size: %zu\n", board.space_count);
    printf("- Transition Count (Snakes & Ladders): %zu\n",
           board.transition_count);
    printf("- Die Sides: %zu\n", args.die.side_count);
    printf("- Die Outcomes: ");
    for (size_t i = 0; i < args.die.side_count; i++) {
        printf("v:%zu,w:%zu ", args.die.outcomes[i].value,
               args.die.outcomes[i].weight);
    }
    printf("\n");
    printf("- Max Steps: %zu\n", args.max_steps);
    printf("- Simulations: %zu\n\n", args.simulation_count);

    // run sims
    Statistics* stats = malloc(args.simulation_count * sizeof(Statistics));

    for (size_t i = 0; i < args.simulation_count; i++) {
        Engine engine = create_engine(&board, &args.die);
        stats[i] =
            simulate_game(engine, args.max_steps, args.exact_tile_to_win);
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
    for (size_t i = 0; i < args.simulation_count; i++) {
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
    printf("Win Rate: %.2f%%\n",
           won_games / (float)args.simulation_count * 100.f);
    if (shortest_path == NULL) {
        printf("No shortest path found! Board is likely impossible.\n");
        assert(won_games == 0);  // sanity check
    } else {
        printf("Average rolls to win a game: %.2f\n",
               total_win_rolls / (float)args.simulation_count);
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
        float rel_freq_global =
            transition_totals[i] / (float)total_traversals * 100.f;
        if (type == SNAKE) {
            printf("Snake ");

            rel_freq_same_group =
                transition_totals[i] / (float)snake_uses * 100.f;
        } else if (type == LADDER) {
            printf("Ladder");

            rel_freq_same_group =
                transition_totals[i] / (float)ladder_uses * 100.f;
        } else {
            // sanity check
            assert(false && "Impossible Transition Type!");
        }

        printf(" from %3zu to %3zu used %zu times (%.2f%% among ",
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

        printf(", %.2f%% of total)\n", rel_freq_global);
    }

    // free resources

    free(transition_totals);

    for (size_t i = 0; i < args.simulation_count; i++) {
        free_stats(stats[i]);
    }
    free(stats);

    free_board(board);

    free(args.transitions);
    free_die(args.die);
    return 0;
}