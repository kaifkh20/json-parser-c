#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

// Print a single Value
void print_test(ResponseKV kv) {
    if (kv.type == ARRAY_RESPONSE) {
        printf("<Array ");
        for (size_t i = 0; i < kv.value.arr->size; ++i) {
            print_value(&kv.value.arr->value_array[i]);
            if (i + 1 < kv.value.arr->size) printf(", ");
        }
        printf(">\n");
    }
    else if (kv.type == OBJECT_RESPONSE) {
        for (size_t i = 0; i < kv.value.obj->size; i++) {
            KeyValue kvp = kv.value.obj->arr[i];
            printf("%s:", kvp.Key.key);
            print_value(&kvp.Value);
            printf("\n");
        }
    }
}
#endif
