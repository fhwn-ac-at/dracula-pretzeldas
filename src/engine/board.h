#pragma once
// Benjamin Eder, 210894

#include <stddef.h>

typedef enum TransitionType {
    SNAKE,
    LADDER,
} TransitionType;

// Represents either a snake or a ladder,
typedef struct Transition {
    size_t from;
    size_t to;
} Transition;

typedef struct Board {
    size_t grid_size_x;
    size_t grid_size_y;
    size_t transition_count;
    Transition* transitions;
} Board;

// TODO: Create/Delete board functions

// Calculates the type of `transition`.
// Ladders always lead further up the board,
// while Snaked always lead towards the start square.
TransitionType get_transition_type(Transition transition);