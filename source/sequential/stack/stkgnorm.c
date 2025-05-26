#include <sequential/stack/stkgnorm.h>

#include <string.h>

#ifndef GNORMSTK_ASSERT

#include <assert.h>

#define GNORMSTK_ASSERT assert

#endif

#if !defined(GNORMSTK_REALLOC) && !defined(GNORMSTK_FREE)
#   include <stdlib.h>
#   ifndef GNORMSTK_REALLOC
#       define GNORMSTK_REALLOC realloc
#   endif
#   ifndef GNORMSTK_FREE
#       define GNORMSTK_FREE free
#   endif
#elif !defined(GNORMSTK_REALLOC)
#   error Reallocator macro is not defined!
#elif !defined(GNORMSTK_FREE)
#   error Free macro is not defined!
#endif

#if !defined(IS_CAPACITY_GNORMSTK) && !defined(EXPAND_CAPACITY_GNORMSTK)
#   ifndef REALLOC_GNORMSTK_CHUNK
#       define REALLOC_GNORMSTK_CHUNK (1 << 5)
#   elif REALLOC_GNORMSTK_CHUNK <= 0
#       error 'REALLOC_GNORMSTK_CHUNK' cannot be less than or equal to 0
#   endif
#   define IS_CAPACITY_GNORMSTK(size) (!((size) % REALLOC_GNORMSTK_CHUNK)) // Checks if size has reached capacity.
#   define EXPAND_CAPACITY_GNORMSTK(capacity) ((capacity) + REALLOC_GNORMSTK_CHUNK) // Calculates next stack's capacity.
#elif !defined(IS_CAPACITY_GNORMSTK)
#   error Is capacity reached check is not defined.
#elif !defined(EXPAND_CAPACITY_GNORMSTK)
#   error Expand capacity size is not defined.
#endif

inline stkgnorm_s stkgnorm_create(void) {
    return (stkgnorm_s) { 0 };
}

void stkgnorm_destroy(stkgnorm_s * stack, const destroy_fn destroy_element, const size_t element_size) {
    GNORMSTK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    GNORMSTK_ASSERT(destroy_element && "[ERROR] 'destroy_element' parameter is NULL.");

    for (char * elements = stack->elements; stack->size; stack->size--) { // while stack isn't empty
        destroy_element(elements); // destroy single element
        elements += element_size; // go to next element
    }

    // free elements array
    GNORMSTK_FREE(stack->elements);
    stack->elements = NULL;
}

stkgnorm_s stkgnorm_copy(const stkgnorm_s stack, const copy_fn copy_element, const size_t element_size) {
    GNORMSTK_ASSERT(copy_element && "[ERROR] 'destroy_element' parameter is NULL.");

    stkgnorm_s stack_copy = { 0 }; // create temporary copy to return

    char const * elements = stack.elements; // save elements array as char pointer
    for (stack_copy.size = 0; stack_copy.size < stack.size; stack_copy.size++) {
        if (IS_CAPACITY_GNORMSTK(stack_copy.size)) { // if stack copy reaches capacity expand it
            const size_t expand = EXPAND_CAPACITY_GNORMSTK(stack_copy.size);

            stack_copy.elements = GNORMSTK_REALLOC(stack_copy.elements, element_size * expand);
            GNORMSTK_ASSERT(stack_copy.elements && "[ERROR] Memory allocation failed");
        }
        // copy each stack's element into its copy
        copy_element(stack_copy.elements + (stack_copy.size * element_size), elements, element_size);

        elements += element_size; // go to next element
    }

    return stack_copy; // return initialized stack copy
}

bool stkgnorm_is_full(const stkgnorm_s stack) {
    return !(~stack.size); // check if stack's size has all bits set to one
}

bool stkgnorm_is_empty(const stkgnorm_s stack) {
    return !(stack.size); // check if stack's size is zero
}

void stkgnorm_push(stkgnorm_s * stack, const void * element, const size_t element_size) {
    GNORMSTK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    GNORMSTK_ASSERT(!(~stack->size) && "[ERROR] Stack size variable will overflow.");
    GNORMSTK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    if (IS_CAPACITY_GNORMSTK(stack->size)) {
        const size_t expand = EXPAND_CAPACITY_GNORMSTK(stack->size);

        stack->elements = GNORMSTK_REALLOC(stack->elements, element_size * expand);
        GNORMSTK_ASSERT(stack->elements && "[ERROR] Memory allocation failed");
    }

    memcpy(stack->elements + (stack->size * element_size), element, element_size);
    stack->size++;
}

void stkgnorm_peep(const stkgnorm_s stack, void * element, const size_t element_size) {
    GNORMSTK_ASSERT(stack.size && "[ERROR] Stack is empty.");
    GNORMSTK_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    GNORMSTK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, stack.elements + (element_size * (stack.size - 1)), element_size);
}

void stkgnorm_pop(stkgnorm_s * stack, void * element, const size_t element_size) {
    GNORMSTK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    GNORMSTK_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    GNORMSTK_ASSERT(stack->size && "[ERROR] Stack is empty.");
    GNORMSTK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    stack->size--;
    memcpy(element, stack->elements + (element_size * stack->size), element_size);

    if (IS_CAPACITY_GNORMSTK(stack->size)) {
        if (stack->size) {
            stack->elements = GNORMSTK_REALLOC(stack->elements, element_size * stack->size);
        } else {
            GNORMSTK_FREE(stack->elements);
            stack->elements = NULL;
        }
    }
}

void stkgnorm_foreach(stkgnorm_s const * stack, const operate_fn operate_element, const size_t element_size,
    void * arguments) {
    GNORMSTK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    GNORMSTK_ASSERT(operate_element && "[ERROR] 'operate_element' parameter is NULL.");
    GNORMSTK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    char * elements = stack->elements;
    for (size_t i = 0; i < stack->size && operate_element(elements, element_size, arguments); ++i) {
        elements += element_size;
    }
}

void stkgnorm_map(stkgnorm_s const * stack, const manage_fn manage_element, const size_t element_size,
    void * arguments) {
    GNORMSTK_ASSERT(stack && "[ERROR] 'stack' parameter is NULL.");
    GNORMSTK_ASSERT(manage_element && "[ERROR] 'manage_element' parameter is NULL.");
    GNORMSTK_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    manage_element(stack->elements, stack->size, element_size, arguments);
}
