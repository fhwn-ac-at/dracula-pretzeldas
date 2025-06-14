// Benjamin Eder, 210894

#include "roll_list.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

roll_list_item_t* resize_list(roll_list_item_t* old_buffer,
                              size_t new_capacity) {
    roll_list_item_t* new_buffer =
        realloc(old_buffer, new_capacity * sizeof(roll_list_item_t));
    if (new_buffer == NULL) {
        fprintf(stderr, "[ERROR] Unable to allocate RollList!\n");
        exit(EXIT_FAILURE);
    }

    return new_buffer;
}

RollList create_list(size_t init_capacity) {
    roll_list_item_t* buffer = resize_list(NULL, init_capacity);

    return (RollList){
        .items = buffer,
        .capacity = init_capacity,
        .size = 0,
    };
}

void free_list(RollList list) {
    free(list.items);
}

void list_append(RollList* list, roll_list_item_t item) {
    assert(list->size <= list->capacity);  // sanity check

    if (list->size == list->capacity) {
        // no space left

        if (list->capacity == 0) {
            list->capacity = 1;  // sanity check to edge case
        }

        // double capacity
        size_t new_capacity = list->capacity * 2;
        resize_list(list->items, new_capacity);
        list->capacity = new_capacity;
    }

    // insert new item
    list->items[list->size] = item;
    list->size++;
}

void list_make_exact_fit(RollList* list) {
    resize_list(list->items, list->size);
    list->capacity = list->size;
}