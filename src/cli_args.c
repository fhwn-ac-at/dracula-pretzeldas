// Benjamin Eder, 210894

#include "cli_args.h"

#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/board.h"
#include "engine/die.h"

void print_usage(char* program_name) {
    char* usage_str =
        "Usage: %s [-x board_x] [-y board_y] [-d weights,values;...] "
        "[-t transitions] [-m max_steps] [-n simulation_count]\n";
    fprintf(stderr, usage_str, program_name);
}

void print_die_format() {
    fprintf(stderr, "Bad value for -d: %s\n", optarg);
    fprintf(stderr,
            "Die format error, correct format: "
            "<weight>,<face_value>;<weight>,<face_value>;...\n");
}

void print_transitions_format() {
    fprintf(stderr, "Bad value for -t: %s\n", optarg);
    fprintf(stderr,
            "Transitions format error, correct format: "
            "<from>,<to>;<from>,<to>;...\n");
}

int parse_size_t_positive(const char* str, size_t* out) {
    errno = 0;
    char* endptr = NULL;
    size_t val = strtoull(str, &endptr, 10);

    // check if no conversion happened or trailing chars remain
    if (endptr == str || *endptr != '\0') {
        return -1;  // invalid input
    }
    // Check for overflow or other errors
    if (val == 0) {
        return -1;  // must be > 0
    }

    *out = (size_t)val;
    return 0;
}

Transition* parse_transitions(char* arg, char* program_name,
                              size_t* transition_count_out) {
    // Transitions are in format <from>,<to>;...
    size_t arg_len = strlen(arg);
    char* arg_copy = malloc(arg_len + 1);  // add extra for \0
    if (arg_copy == NULL) {
        fprintf(stderr, "[ERROR] Unable to copy transitions arg!\n");
        exit(EXIT_FAILURE);
    }

    memcpy(arg_copy, arg, arg_len);

    // count how many transitions (count ';' + 1)
    size_t transition_count = 1;
    for (char* c = arg_copy; *c != '\0'; c++) {
        if (*c == ';') {
            transition_count++;
        }
    }

    Transition* transitions = malloc(sizeof(Transition) * transition_count);

    if (transitions == NULL) {
        fprintf(
            stderr,
            "[ERROR] Unable to allocate transitions on transitions parse!\n");
        exit(EXIT_FAILURE);
    }

    size_t num_pairs_parsed = 0;
    char* side_str = strtok(arg_copy, ";");
    while (side_str != NULL && num_pairs_parsed < transition_count) {
        // parse weight & value, with comma seperator
        char* comma = strchr(side_str, ',');
        if (!comma) {
            // format error
            print_transitions_format();
            exit(EXIT_FAILURE);
        }
        *comma = '\0';

        size_t from_value;
        size_t to_value;
        if (parse_size_t_positive(side_str, &from_value) != 0 ||
            parse_size_t_positive(comma + 1, &to_value) != 0) {
            print_transitions_format();
            exit(EXIT_FAILURE);
        }

        if (from_value < 1 || to_value < 1) {
            fprintf(stderr,
                    "From/To values for Transitions must be above 0!\n");
            print_usage(program_name);
            exit(EXIT_FAILURE);
        }

        transitions[num_pairs_parsed] = (Transition){
            .from = from_value,
            .to = to_value,
        };

        num_pairs_parsed++;
        side_str = strtok(NULL, ";");
    }

    if (num_pairs_parsed != transition_count) {
        print_transitions_format();
        exit(EXIT_FAILURE);
    }

    *transition_count_out = transition_count;
    return transitions;
};

Die parse_die(char* arg, char* program_name) {
    // Die is in format <weight>,<face_value>;...
    size_t arg_len = strlen(arg);
    char* arg_copy = malloc(arg_len + 1);  // add extra for \0
    if (arg_copy == NULL) {
        fprintf(stderr, "[ERROR] Unable to copy die arg!\n");
        exit(EXIT_FAILURE);
    }

    memcpy(arg_copy, arg, arg_len);

    // count how many sides (count ';' + 1)
    size_t die_sides = 1;
    for (char* c = arg_copy; *c != '\0'; c++) {
        if (*c == ';') {
            die_sides++;
        }
    }

    size_t* weights = malloc(sizeof(size_t) * die_sides);
    size_t* face_values = malloc(sizeof(size_t) * die_sides);

    if (weights == NULL || face_values == NULL) {
        fprintf(
            stderr,
            "[ERROR] Unable to allocate weights/face_values on die parse!\n");
        exit(EXIT_FAILURE);
    }

    size_t num_pairs_parsed = 0;
    char* side_str = strtok(arg_copy, ";");
    while (side_str != NULL && num_pairs_parsed < die_sides) {
        // parse weight & value, with comma seperator
        char* comma = strchr(side_str, ',');
        if (!comma) {
            // format error
            print_die_format();
            print_usage(program_name);
            exit(EXIT_FAILURE);
        }
        *comma = '\0';

        size_t weight_value;
        size_t face_value_val;
        if (parse_size_t_positive(side_str, &weight_value) != 0 ||
            parse_size_t_positive(comma + 1, &face_value_val) != 0) {
            print_die_format();
            print_usage(program_name);
            exit(EXIT_FAILURE);
        }
        weights[num_pairs_parsed] = weight_value;
        face_values[num_pairs_parsed] = face_value_val;

        num_pairs_parsed++;
        side_str = strtok(NULL, ";");
    }

    if (num_pairs_parsed != die_sides) {
        print_die_format();
        print_usage(program_name);
        exit(EXIT_FAILURE);
    }

    return create_die(die_sides, weights, face_values);
}

CliArgs parse_cli_args(int argc, char* argv[]) {
    CliArgs args = {
        .board_x = 10,
        .board_y = 10,
        .die = create_dn(6),  // D6 is default
        .max_steps = 100,
        .simulation_count = 1000,
        .transitions_count = 0,
        .transitions = NULL,
    };

    char* shortopts = "x:y:d:m:n:t:";

    int opt;

    while ((opt = getopt(argc, argv, shortopts)) != -1) {
        switch (opt) {
            case 'x': {
                int err_code = parse_size_t_positive(optarg, &args.board_x);
                if (err_code != 0 || args.board_x == 0) {
                    fprintf(stderr, "Bad value for -x: %s\n", optarg);
                    print_usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'y': {
                int err_code = parse_size_t_positive(optarg, &args.board_y);
                if (err_code != 0 || args.board_y == 0) {
                    fprintf(stderr, "Bad value for -y: %s\n", optarg);
                    print_usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'd': {
                args.die = parse_die(optarg, argv[0]);
                break;
            }
            case 't': {
                args.transitions =
                    parse_transitions(optarg, argv[0], &args.transitions_count);
                break;
            }
            case 'm': {
                int err_code = parse_size_t_positive(optarg, &args.max_steps);
                if (err_code != 0 || args.max_steps == 0) {
                    fprintf(stderr, "Bad value for -m: %s\n", optarg);
                    print_usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            case 'n': {
                int err_code =
                    parse_size_t_positive(optarg, &args.simulation_count);
                if (err_code != 0 || args.simulation_count == 0) {
                    fprintf(stderr, "Bad value for -n: %s\n", optarg);
                    print_usage(argv[0]);
                    exit(EXIT_FAILURE);
                }
                break;
            }
            default: {
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    return args;
}