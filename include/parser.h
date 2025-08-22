#ifndef PARSER_H
#define PARSER_H

#include <ctype.h>
#include <stdlib.h> // for size_t
#include "lexer.h"
#include "stack.h"

// Forward declarations to avoid unnecessary includes
typedef struct Object Object;
typedef struct Array Array;

// --- Public Data Structures ---
// These are the types users will interact with.

typedef enum ValueType {
    STRING,
    OBJECT_TYPE,
    INTEGER,
    ARRAY,
    NULL_TYPE,
    BOOLEAN_TYPE,
    FLOAT
} ValueType;

typedef union value {
    Object* obj_val;
    char string_val[100];
    long long int int_val;
    double float_val;
    Array* arr_val;
    void* null_val;
    int bool_val;
} value;

typedef struct Value {
    ValueType val_type;
    union value value;
} Value;

struct Array {
    size_t size;
    Value* value_array;
};

typedef struct Key {
    char key[100];
} Key;

typedef struct KeyValue {
    Key Key;
    Value Value;
} KeyValue;

struct Object {
    KeyValue arr[100];
    size_t size;
};

typedef enum ResponseType {
    OBJECT_RESPONSE,
    ARRAY_RESPONSE,
    ERROR_RESPONSE
} ResponseType;

typedef struct ResponseKV {
    ResponseType type;
    union {
        Object* obj;
        Array*  arr;
    } value;

    Value* (*lookup)(const char* key, struct ResponseKV*);
    void   (*freemem)(struct ResponseKV);
} ResponseKV;

ResponseKV parser(Token* token_container, size_t len);



#endif // PARSER_H
