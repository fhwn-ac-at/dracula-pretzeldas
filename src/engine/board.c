// Benjamin Eder, 210894

#include "board.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SpaceNode* create_node(size_t position) {
    SpaceNode* node = malloc(sizeof(SpaceNode));
    if (node == NULL) {
        fprintf(stderr, "[ERROR] Unable to malloc Node %zu!\n", position);
        exit(EXIT_FAILURE);
    }

    node->next = NULL;
    node->transition_next = NULL;
    node->position = position;

    return node;
}

void free_nodes(SpaceNode* node) {
    SpaceNode* next_node = node;
    while (next_node != NULL) {
        SpaceNode* current_node = next_node;
        next_node = next_node->next;
        free(current_node);
    }
}

void validate_transitions(const Transition* transitions,
                          size_t transition_count, size_t end_pos) {
    for (size_t i = 0; i < transition_count; i++) {
        // debugging info
        size_t transition_pos = i + 1;
        Transition transition = transitions[i];

        if (transition.from > end_pos) {
            fprintf(stderr,
                    "[ERROR] Transition #%zu: start point"
                    " is outside grid of size %zu!\n",
                    transition_pos, end_pos);
            exit(EXIT_FAILURE);
        }

        if (transition.to > end_pos) {
            fprintf(stderr,
                    "[ERROR] Transition #%zu: end point"
                    " is outside grid of size %zu!\n",
                    transition_pos, end_pos);
            exit(EXIT_FAILURE);
        }

        if (transition.from == end_pos) {
            fprintf(stderr,
                    "[ERROR] Transition #%zu: start point can't be on goal!\n",
                    transition_pos);
            exit(EXIT_FAILURE);
        }

        if (transition.from == transition.to) {
            fprintf(stderr, "[ERROR] Transition #%zu: start == end!\n",
                    transition_pos);
            exit(EXIT_FAILURE);
        }

        // check for duplicates
        for (size_t j = 0; j < i; j++) {
            Transition cmp_transition = transitions[j];
            if (cmp_transition.from == transition.from) {
                fprintf(stderr,
                        "[ERROR] Transition #%zu: duplicate start with "
                        "Transition #%zu!\n",
                        transition_pos, j + 1);
                exit(EXIT_FAILURE);
            }

            if (cmp_transition.to == transition.to) {
                fprintf(stderr,
                        "[ERROR] Transition #%zu: duplicate end with "
                        "Transition #%zu!\n",
                        transition_pos, j + 1);
                exit(EXIT_FAILURE);
            }
        }

        // all checks passed for this transition
    }
}

void apply_transition(SpaceNode* head, size_t end_pos, Transition transition) {
    // iterate to start space
    SpaceNode* from_node = head;
    for (size_t from_node_pos = 1; from_node_pos < end_pos; from_node_pos++) {
        if (from_node->position != transition.from) {
            from_node = from_node->next;
        }
    }

    // validating checks from validate_transitions
    assert(from_node != NULL);
    assert(from_node->position != end_pos);
    assert(from_node->transition_next == NULL);

    // iterate to end space
    SpaceNode* to_node = head;
    for (size_t to_node_pos = 1; to_node_pos < end_pos; to_node_pos++) {
        if (to_node->position != transition.to) {
            to_node = to_node->next;
        }
    }

    // validatig checks from validate_transitions
    assert(to_node != NULL);
    assert(from_node != to_node);

    // establish edge
    from_node->transition_next = to_node;
}

Board create_board(size_t size_x, size_t size_y, size_t transition_count,
                   const Transition* transitions) {
    size_t end_pos = size_x * size_y;

    validate_transitions(transitions, transition_count, end_pos);

    // create nodes
    SpaceNode* head = create_node(1);
    SpaceNode* curr_node = head;
    for (size_t i = 2; i <= end_pos; i++) {
        curr_node->next = create_node(i);
        curr_node = curr_node->next;
    }

    // apply transitions
    for (size_t transition_idx = 0; transition_idx < transition_count;
         transition_idx++) {
        Transition transition = transitions[transition_idx];

        apply_transition(head, end_pos, transition);
    }

    return (Board){
        .space_count = end_pos,
        .transition_count = transition_count,
        .start = head,
        .transitions = transitions,
    };
}

void free_board(Board board) {
    free_nodes(board.start);
}

TransitionType get_transition_type(const Transition* const transition) {
    // Transition can't lead to itself
    assert(transition->from != transition->to);

    int direction = transition->to - transition->from;
    if (direction > 0) {
        return LADDER;
    } else {
        return SNAKE;
    }
}