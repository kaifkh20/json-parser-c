#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Stack* createStack(size_t capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    if (!stack) {
        perror("Failed to allocate stack memory");
        return NULL;
    }
    stack->capacity = capacity;
    stack->top = (size_t)-1; // Using SIZE_MAX to represent an empty stack
    stack->array = (char**)malloc(stack->capacity * sizeof(char*));
    if (!stack->array) {
        perror("Failed to allocate array memory");
        free(stack);
        return NULL;
    }
    return stack;
}

void destroyStack(struct Stack* stack) {
    if (stack) {
        // Free the individual strings in the array
        for (size_t i = 0; i <= stack->top; ++i) {
            free(stack->array[i]);
        }
        free(stack->array);
        free(stack);
    }
}

int isFull(struct Stack* stack) {
    return stack->top == stack->capacity - 1;
}

int isEmpty(struct Stack* stack) {
    return stack->top == (size_t)-1;
}

void push(struct Stack* stack, char* item) {
    if (isFull(stack)) {
        printf("Stack overflow!\n");
        return;
    }
    stack->top++;
    stack->array[stack->top] = item;
}

char* pop(struct Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    char* item = stack->array[stack->top];
    stack->top--;
    return item;
}

const char* peek(struct Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->array[stack->top];
}
