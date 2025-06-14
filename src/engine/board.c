// Benjamin Eder, 210894

#include "board.h"

#include <assert.h>

TransitionType get_transition_type(Transition transition) {
    // Transition can't lead to itself
    assert(transition.from != transition.to);

    int direction = transition.to - transition.from;
    if (direction > 0) {
        return LADDER;
    } else {
        return SNAKE;
    }
}

// TODO: impls