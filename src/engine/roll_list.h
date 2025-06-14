#pragma once
// Benjamin Eder, 210894

#include <stddef.h>

typedef size_t roll_list_item_t;

typedef struct RollList {
    roll_list_item_t* items;
    size_t size;
    size_t capacity;
} RollList;

// Creates an empty RollList that can hold at least `init_capacity` items.
RollList create_list(size_t init_capacity);

// Frees the memory allocated by list and consumes it.
void free_list(RollList list);

// Appends a new item to the list.
void list_append(RollList* list, roll_list_item_t item);

// Resizes `items` to be exactly `size` items long.
void list_make_exact_fit(RollList* list);