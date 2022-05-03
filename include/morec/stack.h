#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct stack
{
    size_t elem_size;
    size_t size;
    size_t capacity;
    void* data;
} stack_t;

/**
 * @brief creates a stack
 * @param[in] elem_size size in byte of each element
 * @return the stack
 */
stack_t __stack_create(size_t elem_size);
#define stack(type) __stack_create(sizeof(type))

/**
 * @brief destroys a stack
 * @param[in] stack stack to destroy
 */
void stack_destroy(stack_t* stack);

/**
 * @brief checks if the stack is empty
 * @param[in] stack stack to check
 * @return true if the stack is empty, false otherwise
 */
bool stack_empty(const stack_t* stack);

/**
 * @brief returns the number of elements in a stack
 * @param[in] stack stack to get the size from
 * @return number of elements
 */
size_t stack_size(const stack_t* stack);

/**
 * @brief returns a pointer to the element on top of the stack
 * @param[in] stack stack to get the top element from
 * @return pointer to the element on top of the stack
 */
void* __stack_top(const stack_t* stack);
#define stack_top(type, stack) (*(type*)__stack_top(stack))

/**
 * @brief pushes an element onto the stack
 * @param[in] stack stack to push the element to
 * @param[in] elem element to push
 */
void __stack_push(stack_t* stack, const void* elem);
#define stack_push(stack, value) { typeof(value) __tmp = value; __stack_push(stack, &__tmp); }

/**
 * @brief pops an element from the stack
 * @param[in] stack stack to pop the element from
 */
void stack_pop(stack_t* stack);
