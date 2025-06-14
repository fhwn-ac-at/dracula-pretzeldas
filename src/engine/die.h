#pragma once
// Benjamin Eder, 210894

#include <stddef.h>

typedef struct Roll {
    // the higher the odds, the higher the chance
    // (relative to other weights)
    size_t weight;
    size_t value;
} Roll;

// Represents a die with an arbitrary distrubution
typedef struct Die {
    size_t side_count;
    Roll* outcomes;
} Die;

// Creates a new n-sided Die with the specified outcomes.
// Caller guarantees that `weights` and `values`
// have at least `sides` elements.
Die create_die(size_t sides, size_t* weights, size_t* values);

// Frees all memory associated with the Die and consumes it.
void free_die(Die die);

// Generates a random roll based on the distributions defined by `die`.
// Uses `rand` for random numbers
size_t generate_outcome(const Die* die);