// Benajmin Eder, 210894

#include "engine.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "die.h"
#include "roll_list.h"

Engine create_engine(const Board* board, const Die* die) {
    return (Engine){
        .board = board,
        .die = die,
        .player_pos = NULL,  // Player starts off grid
    };
}

size_t simulate_step(Engine* engine, size_t* traversal_counts,
                     bool exact_tile_for_win) {
    size_t roll_result = generate_outcome(engine->die);

    // on first move of game, player_pos is NULL because player starts off board
    bool is_first_move = engine->player_pos == NULL;
    if (is_first_move) {
        // now move player to actual start of board,
        // and skip one "movement" later
        engine->player_pos = engine->board->start;
    }

    const SpaceNode* node_before_move = engine->player_pos;
    for (size_t i = roll_result; i > 0; i--) {
        if (engine->player_pos->next == NULL) {
            // reached end tile, but overshot it
            // reset move if exact die roll is needed
            engine->player_pos =
                exact_tile_for_win ? node_before_move : engine->player_pos;
            break;
        }

        if (is_first_move) {
            // skip movement
            // it's not first turn anymore
            is_first_move = false;
        } else {
            // normal board movement
            engine->player_pos = engine->player_pos->next;
        }
    }

    // traverse snake/ladder
    if (engine->player_pos->transition_next != NULL) {
        // find which transition it exactly is
        bool found_transition = false;
        size_t search_transition_idx;

        for (size_t i = 0; i < engine->board->transition_count; i++) {
            Transition transition = engine->board->transitions[i];
            if (transition.from == engine->player_pos->position) {
                search_transition_idx = i;
                found_transition = true;
            }
        }

        assert(found_transition);  // sanity check

        traversal_counts[search_transition_idx]++;

        // actually traverse it
        engine->player_pos = engine->player_pos->transition_next;
    }

    return roll_result;
}

Statistics simulate_game(Engine engine, size_t max_rolls,
                         bool exact_tile_for_win) {
    size_t* traversal_counts =
        malloc(engine.board->transition_count * sizeof(size_t));
    if (traversal_counts == NULL) {
        fprintf(stderr, "[ERROR] Unable to allocate traversal counts!\n");
    }

    memset(traversal_counts, 0,
           engine.board->transition_count * sizeof(size_t));

    // arbitrary size of 8, resizes anyways
    RollList roll_path = create_list(8);

    bool reached_goal = false;
    size_t roll_count;
    for (roll_count = 1; roll_count <= max_rolls; roll_count++) {
        size_t roll_result =
            simulate_step(&engine, traversal_counts, exact_tile_for_win);

        list_append(&roll_path, roll_result);

        if (engine.player_pos->position == engine.board->space_count) {
            // reached last space
            reached_goal = true;
            break;
        }
    }

    // resize path to fit exactly
    list_make_exact_fit(&roll_path);

    return (Statistics){
        .roll_count = roll_count,
        .roll_path = roll_path.items,  // no leak, is freed by caller
        .traversal_counts = traversal_counts,
        .was_win = reached_goal,
    };
}

void free_stats(Statistics stats) {
    free(stats.roll_path);
    free(stats.traversal_counts);
}