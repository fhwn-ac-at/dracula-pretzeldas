#pragma once
// Benjamin Eder, 210894

#include <stddef.h>

typedef struct Roll {
    // the higher the odds, the higher the chance
    // (relative to other odds)
    size_t odds;
    size_t value;
} Roll;

// Represents a die with an arbitrary distrubution
typedef struct Die {
    size_t side_count;
    Roll* outcomes;
} Die;

typedef struct Player {
    Die die;
    size_t position;
} Player;

// Creates a new n-sided Die with the specified outcomes.
// Caller guarantees that `odds_arr` and `value_arr`
// have at least `sides` elements.
Die create_die(size_t sides, size_t* odds_arr, size_t* value_arr);

// Frees all memory associated with the Die and consumes it.
void free_die(Die die);

// Creates a new Player that starts off board at position 0.
Player create_player(Die die);

// Generates a random roll based on the distributions defined by `die`.
// Uses `rand` for random numbers
size_t generate_outcome(const Die* die);