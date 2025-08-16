#ifndef STACK_H
#define STACK_H

#include <stdlib.h> // For size_t, malloc, and free
#include <limits.h> // For SIZE_MAX

struct Stack {
    size_t top;
    size_t capacity;
    char** array;
};

/**
 * @brief Creates a new stack with a given capacity.
 * @param capacity The maximum number of elements the stack can hold.
 * @return A pointer to the newly created stack, or NULL on failure.
 */
struct Stack* createStack(size_t capacity);

/**
 * @brief Deallocates the memory associated with the stack.
 * @param stack A pointer to the stack to be destroyed.
 */
void destroyStack(struct Stack* stack);

/**
 * @brief Checks if the stack is full.
 * @param stack A pointer to the stack.
 * @return 1 if the stack is full, 0 otherwise.
 */
int isFull(struct Stack* stack);

/**
 * @brief Checks if the stack is empty.
 * @param stack A pointer to the stack.
 * @return 1 if the stack is empty, 0 otherwise.
 */
int isEmpty(struct Stack* stack);

/**
 * @brief Pushes a character pointer onto the stack.
 * @param stack A pointer to the stack.
 * @param item A character pointer to be pushed.
 */
void push(struct Stack* stack, char* item);

/**
 * @brief Pops a character pointer from the top of the stack.
 * @param stack A pointer to the stack.
 * @return The popped character pointer, or NULL if the stack is empty.
 */
char* pop(struct Stack* stack);

/**
 * @brief Returns the top element of the stack without removing it.
 * @param stack A pointer to the stack.
 * @return The top character pointer, or NULL if the stack is empty.
 */
const char* peek(struct Stack* stack);

#endif // STACK_H
