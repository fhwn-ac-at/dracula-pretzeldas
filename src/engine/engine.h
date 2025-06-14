#pragma once
// Benjamin Eder, 210894

#include <stdbool.h>

#include "board.h"
#include "die.h"

typedef struct Engine {
    const Board* board;
    const Die* die;
    const SpaceNode* player_pos;
} Engine;

typedef struct Statistics {
    size_t roll_count;
    size_t* roll_path;
    size_t* traversal_counts;
    bool was_win;
} Statistics;

// Creates a new Engine with a new Player that plays by rolling `die`.
Engine create_engine(const Board* board, const Die* die);

// Simulates one full game that lasts at most `max_rolls` rolls
// and returns it's stats.
// If `exact_tile_for_win` is true, the Player must exactly roll onto the
// end space to win.
// Caller must free returned Statistics.
// Engine is consumed after call, it can't be used again.
Statistics simulate_game(Engine engine, size_t max_rolls,
                         bool exact_tile_for_win);

// Frees all allocated memory from the Statistics and consumes it.
void free_stats(Statistics stats);