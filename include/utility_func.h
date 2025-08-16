#ifndef FREE_MEM_H
#define FREE_MEM_H

#include "parser.h" // For ResponseKV, Array, Object, and Value definitions

// ---------------- Memory Management ----------------

/**
 * @brief Recursively frees the memory allocated for an Object.
 * @param object A pointer to the Object to free.
 */
void free_mem_object(Object* object);

/**
 * @brief Recursively frees the memory allocated for an Array.
 * @param array A pointer to the Array to free.
 */
void free_mem_array(Array* array);

/**
 * @brief Frees all memory associated with a ResponseKV object,
 * including nested objects/arrays/values.
 * @param kv A pointer to the ResponseKV object.
 */
void free_mem(ResponseKV* kv);

/**
 * @brief Frees a Value object and its nested contents if they are dynamically allocated.
 * @param value A pointer to the Value object to free.
 */
void free_val(Value* value);

// ---------------- Debugging / Printing ----------------

/**
 * @brief Returns a string representation of the type of a Value.
 * @param value A pointer to the Value object.
 * @return A constant string indicating the type (e.g., "STRING", "OBJECT").
 */
const char* typeOf(Value* value);

/**
 * @brief Prints a Value and its contents (recursively for objects/arrays).
 * @param value A pointer to the Value object.
 */
void print_value(Value* value);

/**
 * @brief Prints all key-value pairs in an Object.
 * @param obj A pointer to the Object to print.
 */
void print_object(Object* obj);

/**
 * @brief Prints all elements in an Array.
 * @param arr A pointer to the Array to print.
 */
void print_array(Array* arr);

#endif // FREE_MEM_H
