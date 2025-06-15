#pragma once
// Benjamin Eder, 210894

#include <stdbool.h>
#include <stddef.h>

#include "engine/board.h"
#include "engine/die.h"

typedef struct CliArgs {
    size_t board_x;
    size_t board_y;
    Die die;
    size_t max_steps;
    size_t simulation_count;
    size_t transitions_count;
    Transition* transitions;
} CliArgs;

// Parses the cli args.
CliArgs parse_cli_args(int argc, char* argv[]);