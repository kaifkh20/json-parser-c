#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

// =====================
// Private declarations
// =====================
static Object* parse_object(Token* token_container, size_t len, int* idx, struct Stack* stack, int if_sub_obj);
static Array*  parse_array (Token* token_container, size_t i,   int* idx, int len, struct Stack* stack);


// Public API functions provided via ResponseKV function pointers
static Value* lookup(const char* key, struct ResponseKV* self);

// =====================
// Lookup helpers
// =====================
// NOTE: returns a pointer INTO the tree (do not free it!).
static Value* lookup_in_object(const char* key, Object* obj) {
    if (!obj || !key) return NULL;
    for (size_t i = 0; i < obj->size; ++i) {
        if (strcmp(obj->arr[i].Key.key, key) == 0) {
            return &obj->arr[i].Value; // return address inside object
        }
        // Recurse into nested objects
        if (obj->arr[i].Value.val_type == OBJECT_TYPE) {
            Value* found = lookup_in_object(key, obj->arr[i].Value.value.obj_val);
            if (found) return found;
        }
    }
    return NULL;
}

// A NULL sentinel to avoid heap allocating on misses
static Value* null_sentinel(void) {
    static Value v = { .val_type = NULL_TYPE };
    return &v;
}

static Value* lookup(const char* key, struct ResponseKV* self) {
    if (!self || !key) return null_sentinel();
    if (self->type != OBJECT_RESPONSE || !self->value.obj) return null_sentinel();
    Value* hit = lookup_in_object(key, self->value.obj);
    return hit ? hit : null_sentinel();
}

// =====================
// Parsing: arrays
// =====================
static Array* parse_array(Token* token_container, size_t i, int* idx, int len, struct Stack* stack) {
    Array* array = (Array*)malloc(sizeof(Array));
    if (!array) return NULL;

    size_t capacity = 100;
    array->value_array = (Value*)malloc(sizeof(Value) * capacity);
    if (!array->value_array) { free(array); return NULL; }

    size_t idx_Array = 0;

    while (i < (size_t)len && token_container[i].t_type != ArrayEnd) {
        if (idx_Array >= capacity) {
            capacity *= 2; // grow capacity (dynamic array)
            Value* tmp = realloc(array->value_array, sizeof(Value) * capacity);
            if (!tmp) {
                // cleanup to avoid leaks
                free(array->value_array);
                free(array);
                return NULL;
            }
            array->value_array = tmp;
        }

        if (token_container[i].t_type == StringValue) {
            array->value_array[idx_Array].val_type = STRING;
            strcpy(array->value_array[idx_Array].value.string_val, token_container[i].ch);
            idx_Array++;
        } else if (token_container[i].t_type == Integer) {
            array->value_array[idx_Array].val_type = INTEGER;
            array->value_array[idx_Array].value.int_val = atoll(token_container[i].ch);
            idx_Array++;
        } else if (token_container[i].t_type == Float){
            array->value_array[idx_Array].val_type = FLOAT;
            array->value_array[idx_Array].value.float_val= atof(token_container[i].ch);
            idx_Array++;
        } 
        else if (token_container[i].t_type == StartObject) {
            array->value_array[idx_Array].val_type = OBJECT_TYPE;
            *idx = (int)i;
            array->value_array[idx_Array].value.obj_val = parse_object(token_container, (size_t)len, idx, stack, 1);
            i = (size_t)(*idx);
            idx_Array++;
        } else if (token_container[i].t_type == NullValue) {
            array->value_array[idx_Array].val_type = NULL_TYPE;
            array->value_array[idx_Array].value.null_val = NULL;
            idx_Array++;
        } else if (token_container[i].t_type == ArrayStart) {
            array->value_array[idx_Array].val_type = ARRAY;
            array->value_array[idx_Array].value.arr_val = parse_array(token_container, i + 1, idx, len, stack);
            i = (size_t)(*idx);
            idx_Array++;
        } else if (token_container[i].t_type == BooleanTrue) {
            array->value_array[idx_Array].val_type = BOOLEAN_TYPE;
            array->value_array[idx_Array].value.bool_val = 1;
            idx_Array++;
        } else if (token_container[i].t_type == BooleanFalse) {
            array->value_array[idx_Array].val_type = BOOLEAN_TYPE;
            array->value_array[idx_Array].value.bool_val = 0;
            idx_Array++;
        }

        i++;
    }

    array->size = idx_Array;
    *idx = (int)i;
    return array;
}

// =====================
// Parsing: objects
// =====================
static Object* parse_object(Token* token_container, size_t len, int* idx, struct Stack* stack, int if_sub_obj) {
    KeyValue temp_arr[100];
    size_t idx_arr = 0;

    Object* obj = (Object*)malloc(sizeof(Object));
    if (!obj) return NULL;

    size_t i = (size_t)(*idx);
    int expecting_key = 1;

    for (i = (size_t)(*idx); i < len; i++) {
        char* token = token_container[i].ch;
        enum TokenType type_token = token_container[i].t_type;

        if (type_token == StartObject) {
            push(stack, token_container[i].ch);
            if (i > 0 && token_container[i - 1].t_type == KeyValueSeperator) {
                *idx = (int)(i + 1);
                Object* nested_obj = parse_object(token_container, len, idx, stack, 1);
                temp_arr[idx_arr].Value.val_type = OBJECT_TYPE;
                temp_arr[idx_arr].Value.value.obj_val = nested_obj;
                idx_arr++;
                i = (size_t)(*idx);
                expecting_key = 1;
            }
        } else if (type_token == EndObject) {
            if (isEmpty(stack) == 1 || strncmp("{", peek(stack), 1) != 0) {
                free(obj);
                fprintf(stderr,"Parser Error : Invalid Syntax\nOperation Aborted\n");
                return NULL; 
            }
            pop(stack);

            if (if_sub_obj) {
                memcpy(obj->arr, temp_arr, 100 * sizeof(KeyValue));
                obj->size = idx_arr;
                *idx = (int)i;
                return obj;
            }
        } else if (type_token == StringKey && !isEmpty(stack) && expecting_key) {
            Key key;
            strcpy(key.key, token);
            temp_arr[idx_arr].Key = key;
            expecting_key = 0;
        } else if (type_token == StringValue && !expecting_key) {
            temp_arr[idx_arr].Value.val_type = STRING;
            strcpy(temp_arr[idx_arr].Value.value.string_val, token);
            idx_arr++;
            expecting_key = 1;
        } else if (type_token == Integer && !expecting_key) {
            temp_arr[idx_arr].Value.val_type = INTEGER;
            temp_arr[idx_arr].Value.value.int_val = atoll(token);
            idx_arr++;
            expecting_key = 1;
        } 
        else if (token_container[i].t_type == Float){
            temp_arr[idx_arr].Value.val_type = FLOAT;
            temp_arr[idx_arr].Value.value.float_val = atof(token);
            idx_arr++;
            expecting_key = 1;
        } 

        else if (type_token == ArrayStart && !expecting_key) {
            Array* array = parse_array(token_container, i + 1, idx, len, stack);
            temp_arr[idx_arr].Value.val_type = ARRAY;
            temp_arr[idx_arr].Value.value.arr_val = array;
            idx_arr++;
            i = (size_t)(*idx);
            expecting_key = 1;
        } else if (type_token == NullValue && !expecting_key) {
            temp_arr[idx_arr].Value.val_type = NULL_TYPE;
            temp_arr[idx_arr].Value.value.null_val = NULL;
            idx_arr++;
            expecting_key = 1;
        } else if (type_token == BooleanTrue && !expecting_key) {
            temp_arr[idx_arr].Value.val_type = BOOLEAN_TYPE;
            temp_arr[idx_arr].Value.value.bool_val = 1;
            idx_arr++;
            expecting_key = 1;
        } else if (type_token == BooleanFalse && !expecting_key) {
            temp_arr[idx_arr].Value.val_type = BOOLEAN_TYPE;
            temp_arr[idx_arr].Value.value.bool_val = 0;
            idx_arr++;
            expecting_key = 1;
        } else if (type_token == KeyValueSeperator) {
            // no-op
        } else if (type_token == Comma) {
            expecting_key = 1;
        }
    }

    if (i == len && !isEmpty(stack)) {
        printf("%s\n", peek(stack));
        fprintf(stderr,"Parser Error : Invalid Syntax\nOperation Aborted\n");
        free(obj);
        return NULL;
    }

    memcpy(obj->arr, temp_arr, 100 * sizeof(KeyValue));
    obj->size = idx_arr;
    *idx = (int)i;
    return obj;
}

// =====================
// Parser entrypoint
// =====================
ResponseKV parser(Token* token_container, size_t len) {
    struct Stack* stack = createStack(len);
    int idx = 0;
    ResponseKV res;

    if (token_container[0].t_type == ArrayStart) {
        Array* arr = parse_array(token_container, 1, &idx, (int)len, stack);
        if(arr==NULL){
            arr = NULL;
            res.type = ERROR_RESPONSE;
        }// store pointer, do NOT copy
        else res.type = ARRAY_RESPONSE;
        res.value.arr = arr;      
        
    } else if (token_container[0].t_type == StartObject) {
        Object* obj = parse_object(token_container, len, &idx, stack, 0);
        if(obj==NULL){obj = NULL;res.type = ERROR_RESPONSE;}
        else res.type = OBJECT_RESPONSE;
        res.value.obj = obj;                 // store pointer, do NOT copy
    } else {
        fprintf(stderr,"Parser Error: Invalid Opening Tags\nOperation Aborted\n");
        res.type = ERROR_RESPONSE;
    }

    res.lookup  = lookup;
    free(stack->array);
    free(stack);
    // Safe to destroy the parse-time stack now (keys/strings are copied into fixed buffers)
    return res;
}

