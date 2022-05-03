#include <morec/stack.h>

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 16

stack_t __stack_create(size_t elem_size)
{
    stack_t stack = {
        .elem_size = elem_size,
        .size = 0,
        .capacity = DEFAULT_SIZE,
        .data = malloc(elem_size * DEFAULT_SIZE)
    };
    return stack;
}

void stack_destroy(stack_t* stack)
{
    free(stack->data);
}

bool stack_empty(const stack_t* stack)
{
    return stack->size == 0;
}

size_t stack_size(const stack_t* stack)
{
    return stack->size;
}

void* __stack_top(const stack_t* stack)
{
    return (uint8_t*)stack->data + (stack->size - 1) * stack->elem_size;
}

void __stack_push(stack_t* stack, const void* elem)
{
    if (stack->size == stack->capacity)
    {
        stack->capacity *= 2;
        stack->data = realloc(stack->data, stack->elem_size * stack->capacity);
    }
    memcpy((uint8_t*)stack->data + stack->size * stack->elem_size, elem, stack->elem_size);
    stack->size++;
}

void stack_pop(stack_t* stack)
{
    assert(stack->size > 0);
    stack->size--;
}
