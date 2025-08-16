#include <stdlib.h>
#include "parser.h"
#include "utility_func.h"

void free_val(Value* value) {
    if (!value) return;

    switch (value->val_type) {
        case STRING:
            // string_val is fixed-size inside struct → no malloc → nothing to free
            break;

        case INTEGER:
        case BOOLEAN_TYPE:
        case NULL_TYPE:
            // nothing dynamically allocated here
            break;

        case OBJECT_TYPE:
            if (value->value.obj_val) {
                Object* obj = value->value.obj_val;
                for (size_t i = 0; i < obj->size; i++) {
                    free_val(&obj->arr[i].Value);
                }
                free(obj);
                value->value.obj_val = NULL;
            }
            break;

        case ARRAY:
            if (value->value.arr_val) {
                Array* arr = value->value.arr_val;
                for (size_t i = 0; i < arr->size; i++) {
                    free_val(&arr->value_array[i]);
                }
                free(arr->value_array);
                free(arr);
                value->value.arr_val = NULL;
            }
            break;
    }
}

void free_mem_object(Object* object) {
    if (!object) return;
    for (size_t i = 0; i < object->size; i++) {
        free_val(&object->arr[i].Value);
    }
    free(object);
}

void free_mem_array(Array* array) {
    if (!array) return;
    for (size_t i = 0; i < array->size; i++) {
        free_val(&array->value_array[i]);
    }
    free(array->value_array);
    free(array);
}

void free_mem(ResponseKV kv) {
    
    if (kv.type == OBJECT_RESPONSE && kv.value.obj) {
        free_mem_object(kv.value.obj);
    } else if (kv.type == ARRAY_RESPONSE && kv.value.arr) {
        free_mem_array(kv.value.arr);
    }

}

// Helper to print a Value recursively
void print_value(Value* val) {
    if (!val) return;

    switch (val->val_type) {
        case STRING:
            printf("%s", val->value.string_val);
            break;

        case INTEGER:
            printf("%lli", val->value.int_val);
            break;

        case BOOLEAN_TYPE:
            printf("%s", val->value.bool_val ? "true" : "false");
            break;

        case NULL_TYPE:
            printf("null");
            break;

        case OBJECT_TYPE:
            printf("<Object>");
            break;

        case ARRAY: {
            Array* arr = val->value.arr_val;
            printf("<Array ");
            for (size_t i = 0; i < arr->size; i++) {
                print_value(&arr->value_array[i]);
                if (i != arr->size - 1) printf(", ");
            }
            printf(">");
            break;
        }
    }
}
/**
 * @brief Prints all key-value pairs in an Object.
 * @param obj A pointer to the Object to print.
 */
void print_object(Object* obj) {
    if (!obj) {
        printf("null");
        return;
    }

    printf("{ ");
    for (size_t i = 0; i < obj->size; i++) {
        KeyValue kv = obj->arr[i];
        printf("%s: ", kv.Key.key);
        print_value(&kv.Value);

        if (i != obj->size - 1) {
            printf(", ");
        }
    }
    printf(" }");
}

/**
 * @brief Prints all elements in an Array.
 * @param arr A pointer to the Array to print.
 */
void print_array(Array* arr) {
    if (!arr) {
        printf("null");
        return;
    }

    printf("[ ");
    for (size_t i = 0; i < arr->size; i++) {
        print_value(&arr->value_array[i]);

        if (i != arr->size - 1) {
            printf(", ");
        }
    }
    printf(" ]");
}
const char* typeOf(Value* value) {
    if (!value) {
        return "\e[1;31m<NULL VALUE>"; // red for error/null
    }

    switch (value->val_type) {
        case STRING:
            return "\e[1;33mSTRING";   // yellow
        case INTEGER:
            return "\e[1;33mINTEGER";  // yellow
        case OBJECT_TYPE:
            return "\e[1;32mOBJECT";   // green
        case ARRAY:
            return "\e[1;32mARRAY";    // green
        case NULL_TYPE:
            return "\e[1;31mNULL";     // red
        case BOOLEAN_TYPE:
            return "\e[1;34mBOOLEAN";  // blue
        default:
            return "\e[1;35mUNKNOWN";  // magenta fallback
    }
}
