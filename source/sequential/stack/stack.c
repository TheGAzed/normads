#include <sequential/stack/stack.h>

#include <string.h>

#ifndef STK_ASSERT

#include <assert.h>

#define STK_ASSERT assert

#endif

#if !defined(STK_REALLOC) && !defined(STK_FREE)
#   include <stdlib.h>
#   ifndef STK_REALLOC
#       define STK_REALLOC realloc
#   endif
#   ifndef STK_FREE
#       define STK_FREE free
#   endif
#elif !defined(STK_REALLOC)
#   error Reallocator macro is not defined!
#elif !defined(STK_FREE)
#   error Free macro is not defined!
#endif

#if !defined(IS_CAPACITY_STK) && !defined(EXPAND_CAPACITY_STK)
#   ifndef REALLOC_STK_CHUNK
#       define REALLOC_STK_CHUNK (1 << 5)
#   elif REALLOC_STK_CHUNK <= 0
#       error 'REALLOC_STK_CHUNK' cannot be less than or equal to 0
#   endif
#   define IS_CAPACITY_STK(size) (!((size) % REALLOC_STK_CHUNK)) // Checks if size has reached capacity.
#   define EXPAND_CAPACITY_STK(capacity) ((capacity) + REALLOC_STK_CHUNK) // Calculates next stack's capacity.
#elif !defined(IS_CAPACITY_STK)
#   error Is capacity reached check is not defined.
#elif !defined(EXPAND_CAPACITY_STK)
#   error Expand capacity size is not defined.
#endif

inline stack_s stkgnorm_create(void) {
    return (stack_s) { 0 };
}

void stkgnorm_destroy(stack_s * stack, const destroy_fn destroy_element, const size_t element_size) {
    STK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    STK_ASSERT(destroy_element && "[ERROR] 'destroy_element' parameter is NULL.");

    for (char * elements = stack->elements; stack->size; stack->size--) { // while stack isn't empty
        destroy_element(elements); // destroy single element
        elements += element_size; // go to next element
    }

    // free elements array
    STK_FREE(stack->elements);
    stack->elements = NULL;
}

stack_s stkgnorm_copy(const stack_s stack, const copy_fn copy_element, const size_t element_size) {
    STK_ASSERT(copy_element && "[ERROR] 'destroy_element' parameter is NULL.");

    stack_s stack_copy = { 0 }; // create temporary copy to return

    char const * elements = stack.elements; // save elements array as char pointer
    for (stack_copy.size = 0; stack_copy.size < stack.size; stack_copy.size++) {
        if (IS_CAPACITY_STK(stack_copy.size)) { // if stack copy reaches capacity expand it
            const size_t expand = EXPAND_CAPACITY_STK(stack_copy.size);

            stack_copy.elements = STK_REALLOC(stack_copy.elements, element_size * expand);
            STK_ASSERT(stack_copy.elements && "[ERROR] Memory allocation failed");
        }
        // copy each stack's element into its copy
        copy_element(stack_copy.elements + (stack_copy.size * element_size), elements, element_size);

        elements += element_size; // go to next element
    }

    return stack_copy; // return initialized stack copy
}

bool stkgnorm_is_full(const stack_s stack) {
    return !(~stack.size); // check if stack's size has all bits set to one
}

bool stkgnorm_is_empty(const stack_s stack) {
    return !(stack.size); // check if stack's size is zero
}

void stkgnorm_push(stack_s * stack, const void * element, const size_t element_size) {
    STK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    STK_ASSERT(!(~stack->size) && "[ERROR] Stack size variable will overflow.");
    STK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    if (IS_CAPACITY_STK(stack->size)) {
        const size_t expand = EXPAND_CAPACITY_STK(stack->size);

        stack->elements = STK_REALLOC(stack->elements, element_size * expand);
        STK_ASSERT(stack->elements && "[ERROR] Memory allocation failed");
    }

    memcpy(stack->elements + (stack->size * element_size), element, element_size);
    stack->size++;
}

void stkgnorm_peep(const stack_s stack, void * element, const size_t element_size) {
    STK_ASSERT(stack.size && "[ERROR] Stack is empty.");
    STK_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    STK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, stack.elements + (element_size * (stack.size - 1)), element_size);
}

void stkgnorm_pop(stack_s * stack, void * element, const size_t element_size) {
    STK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    STK_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    STK_ASSERT(stack->size && "[ERROR] Stack is empty.");
    STK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    stack->size--;
    memcpy(element, stack->elements + (element_size * stack->size), element_size);

    if (IS_CAPACITY_STK(stack->size)) {
        if (stack->size) {
            stack->elements = STK_REALLOC(stack->elements, element_size * stack->size);
        } else {
            STK_FREE(stack->elements);
            stack->elements = NULL;
        }
    }
}

void stkgnorm_foreach(stack_s const * stack, const operate_fn operate_element, const size_t element_size,
    void * arguments) {
    STK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    STK_ASSERT(operate_element && "[ERROR] 'operate_element' parameter is NULL.");
    STK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    char * elements = stack->elements;
    for (size_t i = 0; i < stack->size && operate_element(elements, element_size, arguments); ++i) {
        elements += element_size;
    }
}

void stkgnorm_map(stack_s const * stack, const manage_fn manage_element, const size_t element_size,
    void * arguments) {
    STK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    STK_ASSERT(manage_element && "[ERROR] 'manage_element' parameter is NULL.");
    STK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    manage_element(stack->elements, stack->size, element_size, arguments);
}
