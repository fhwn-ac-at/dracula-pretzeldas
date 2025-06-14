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

typedef struct SpaceNode SpaceNode;
typedef struct SpaceNode {
    size_t position;
    SpaceNode* next;
    SpaceNode* transition_next;
} SpaceNode;

typedef struct Board {
    const size_t space_count;
    const size_t transition_count;
    const Transition* const transitions;
    SpaceNode* start;
} Board;

// Creates a new Board of size `size_x` x `size_y`,
// with the specified transitions.
// Caller must guarantee that `transitions`
// contains at least `transition_count` elements.
Board create_board(size_t size_x, size_t size_y, size_t transition_count,
                   const Transition* transitions);

// Deallocates all memory associated with the Board and consumes it.
void free_board(Board board);

// Calculates the type of `transition`.
// Ladders always lead further up the board,
// while Snaked always lead towards the start square.
TransitionType get_transition_type(const Transition* const transition);