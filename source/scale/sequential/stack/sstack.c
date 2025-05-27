#include <scale/sequential/stack/sstack.h>

#include <string.h>

#ifndef ASSERT_SSTK

#include <assert.h>

#define ASSERT_SSTK assert

#endif

#if !defined(REALLOC_SSTK) && !defined(FREE_SSTK)
#   include <stdlib.h>
#   ifndef REALLOC_SSTK
#       define REALLOC_SSTK realloc
#   endif
#   ifndef FREE_SSTK
#       define FREE_SSTK free
#   endif
#elif !defined(REALLOC_SSTK)
#   error Reallocator macro is not defined!
#elif !defined(FREE_SSTK)
#   error Free macro is not defined!
#endif

#if !defined(IS_CAPACITY_SSTK) && !defined(EXPAND_CAPACITY_SSTK)
#   ifndef REALLOC_CHUNK_SSTK
#       define REALLOC_CHUNK_SSTK (1 << 5)
#   elif REALLOC_CHUNK_SSTK <= 0
#       error 'REALLOC_CHUNK_SSTK' cannot be less than or equal to 0
#   endif
#   define IS_CAPACITY_SSTK(size) (!((size) % REALLOC_CHUNK_SSTK)) // Checks if size has reached capacity.
#   define EXPAND_CAPACITY_SSTK(capacity) ((capacity) + REALLOC_CHUNK_SSTK) // Calculates next stack's capacity.
#elif !defined(IS_CAPACITY_SSTK)
#   error Is capacity reached check is not defined.
#elif !defined(EXPAND_CAPACITY_SSTK)
#   error Expand capacity size is not defined.
#endif

sstack_s sstk_create(void) {
    return (sstack_s) { 0 };
}

void sstk_destroy(sstack_s * stack, const destroy_fn destroy_element, const size_t element_size) {
    ASSERT_SSTK(stack && "[ERROR] 'stack' parameter is NULL.");
    ASSERT_SSTK(destroy_element && "[ERROR] 'destroy_element' parameter is NULL.");

    for (char * elements = stack->elements; stack->size; stack->size--) { // while stack isn't empty
        destroy_element(elements); // destroy single element
        elements += element_size; // go to next element
    }

    // free elements array
    FREE_SSTK(stack->elements);
    stack->elements = NULL;
}

sstack_s sstk_copy(const sstack_s stack, const copy_fn copy_element, const size_t element_size) {
    ASSERT_SSTK(copy_element && "[ERROR] 'destroy_element' parameter is NULL.");

    sstack_s stack_copy = { 0 }; // create temporary copy to return

    char const * elements = stack.elements; // save elements array as char pointer
    for (stack_copy.size = 0; stack_copy.size < stack.size; stack_copy.size++) {
        if (IS_CAPACITY_SSTK(stack_copy.size)) { // if stack copy reaches capacity expand it
            const size_t expand = EXPAND_CAPACITY_SSTK(stack_copy.size);

            stack_copy.elements = REALLOC_SSTK(stack_copy.elements, element_size * expand);
            ASSERT_SSTK(stack_copy.elements && "[ERROR] Memory allocation failed");
        }
        // copy each stack's element into its copy
        copy_element((char*)(stack_copy.elements) + (stack_copy.size * element_size), elements, element_size);

        elements += element_size; // go to next element
    }

    return stack_copy; // return initialized stack copy
}

bool sstk_is_full(const sstack_s stack) {
    return !(~stack.size); // check if stack's size has all bits set to one
}

bool sstk_is_empty(const sstack_s stack) {
    return !(stack.size); // check if stack's size is zero
}

void sstk_push(sstack_s * stack, const void * element, const size_t element_size) {
    ASSERT_SSTK(stack && "[ERROR] 'stack' parameter is NULL.");
    ASSERT_SSTK((~stack->size) && "[ERROR] Stack size variable will overflow.");
    ASSERT_SSTK(element_size && "[ERROR] Element's size can't be zero.");

    if (IS_CAPACITY_SSTK(stack->size)) {
        const size_t expand = EXPAND_CAPACITY_SSTK(stack->size);

        stack->elements = REALLOC_SSTK(stack->elements, element_size * expand);
        ASSERT_SSTK(stack->elements && "[ERROR] Memory allocation failed");
    }

    memcpy((char*)(stack->elements) + (stack->size * element_size), element, element_size);
    stack->size++;
}

void sstk_peep(const sstack_s stack, void * element, const size_t element_size) {
    ASSERT_SSTK(stack.size && "[ERROR] Stack is empty.");
    ASSERT_SSTK(element && "[ERROR] 'element' parameter is NULL.");
    ASSERT_SSTK(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, (char*)(stack.elements) + (element_size * (stack.size - 1)), element_size);
}

void sstk_pop(sstack_s * stack, void * element, const size_t element_size) {
    ASSERT_SSTK(stack && "[ERROR] 'stack' parameter is NULL.");
    ASSERT_SSTK(element && "[ERROR] 'element' parameter is NULL.");
    ASSERT_SSTK(stack->size && "[ERROR] Stack is empty.");
    ASSERT_SSTK(element_size && "[ERROR] Element's size can't be zero.");

    stack->size--;
    memcpy(element, ((char*)stack->elements) + (element_size * stack->size), element_size);

    if (IS_CAPACITY_SSTK(stack->size)) {
        if (stack->size) {
            stack->elements = REALLOC_SSTK(stack->elements, element_size * stack->size);
        } else {
            FREE_SSTK(stack->elements);
            stack->elements = NULL;
        }
    }
}

void sstk_foreach(sstack_s const * stack, const operate_fn operate, const size_t element_size,
    void * arguments) {
    ASSERT_SSTK(stack && "[ERROR] 'stack' parameter is NULL.");
    ASSERT_SSTK(operate && "[ERROR] 'operate_element' parameter is NULL.");
    ASSERT_SSTK(element_size && "[ERROR] Element's size can't be zero.");

    char * elements = stack->elements;
    for (size_t i = 0; i < stack->size && operate(elements, element_size, arguments); ++i) {
        elements += element_size;
    }
}

void sstk_map(sstack_s const * stack, const manage_fn manage, const size_t element_size,
    void * arguments) {
    ASSERT_SSTK(stack && "[ERROR] 'stack' parameter is NULL.");
    ASSERT_SSTK(manage && "[ERROR] 'manage' parameter is NULL.");
    ASSERT_SSTK(element_size && "[ERROR] Element's size can't be zero.");

    manage(stack->elements, stack->size, element_size, arguments);
}
