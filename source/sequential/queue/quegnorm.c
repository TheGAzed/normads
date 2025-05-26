#include <sequential/queue/quegnorm.h>

#include <string.h>

#ifndef QUEGNORM_ASSERT
#   include <assert.h>
#   define QUEGNORM_ASSERT assert
#endif

#if !defined(QUEGNORM_REALLOC) && !defined(QUEGNORM_FREE)
#   include <stdlib.h>
#   ifndef QUEGNORM_REALLOC
#       define QUEGNORM_REALLOC realloc
#   endif
#   ifndef QUEGNORM_FREE
#       define QUEGNORM_FREE free
#   endif
#elif !defined(QUEGNORM_REALLOC)
#   error Reallocator macro is not defined!
#elif !defined(QUEGNORM_FREE)
#   error Free macro is not defined!
#endif

#if !defined(IS_CAPACITY_QUEGNORM) && !defined(EXPAND_CAPACITY_QUEGNORM)
#   ifndef REALLOC_QUEGNORM_CHUNK
#       define REALLOC_QUEGNORM_CHUNK (1 << 5)
#   elif REALLOC_QUEGNORM_CHUNK <= 0
#       error 'REALLOC_QUEGNORM_CHUNK' cannot be less than or equal to 0
#   endif
#   define IS_CAPACITY_QUEGNORM(size) (!((size) % REALLOC_QUEGNORM_CHUNK)) // Checks if size has reached capacity.
#   define EXPAND_CAPACITY_QUEGNORM(capacity) ((capacity) + REALLOC_QUEGNORM_CHUNK) // Calculates next queue's capacity.
#elif !defined(IS_CAPACITY_QUEGNORM)
#   error Is capacity reached check is not defined.
#elif !defined(EXPAND_CAPACITY_QUEGNORM)
#   error Expand capacity size is not defined.
#endif

quegnorm_s gnormque_create(void) {
    return (quegnorm_s) { 0 };
}

void gnormque_destroy(quegnorm_s * queue, const destroy_fn destroy, const size_t element_size) {
    QUEGNORM_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUEGNORM_ASSERT(destroy && "[ERROR] 'destroy' parameter is NULL.");
    QUEGNORM_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    // while queue isn't empty
    for (char * elements = queue->elements + (queue->current * element_size); queue->size; queue->size--) {
        destroy(elements); // destroy single element
        elements += element_size; // go to next element
    }

    // free elements array
    QUEGNORM_FREE(queue->elements);
    queue->elements = NULL;
}

quegnorm_s gnormque_copy(const quegnorm_s queue, const copy_fn copy, const size_t element_size) {
    QUEGNORM_ASSERT(copy && "[ERROR] 'copy' parameter is NULL.");
    QUEGNORM_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    quegnorm_s queue_copy = { 0 }; // create temporary copy to return

    char const * elements = queue.elements + (queue.current * element_size); // save elements array as char pointer
    for (queue_copy.size = 0; queue_copy.size < queue.size; queue_copy.size++) {
        if (IS_CAPACITY_QUEGNORM(queue_copy.size)) { // if queue copy reaches capacity expand it
            const size_t expand = EXPAND_CAPACITY_QUEGNORM(queue_copy.size);

            queue_copy.elements = QUEGNORM_REALLOC(queue_copy.elements, element_size * expand);
            QUEGNORM_ASSERT(queue_copy.elements && "[ERROR] Memory allocation failed");
        }
        // copy each queue's element into its copy
        copy(queue_copy.elements + (queue_copy.size * element_size), elements, element_size);

        elements += element_size; // go to next element
    }

    return queue_copy; // return initialized queue copy
}

bool gnormque_is_full(const quegnorm_s queue) {
    return !(~queue.size); // check if queue's size has all bits set to one
}

bool gnormque_is_empty(const quegnorm_s queue) {
    return !(queue.size); // check if queue's size is zero
}

void gnormque_enqueue(quegnorm_s * queue, const void * element, const size_t element_size) {
    QUEGNORM_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUEGNORM_ASSERT(!(~queue->size) && "[ERROR] Queue size variable will overflow.");
    QUEGNORM_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    const size_t next_index = queue->current + queue->size;
    if (IS_CAPACITY_QUEGNORM(next_index)) { // if capacity is reached expand queue's elements array
        const size_t expand = EXPAND_CAPACITY_QUEGNORM(next_index);

        queue->elements = QUEGNORM_REALLOC(queue->elements, element_size * expand);
        QUEGNORM_ASSERT(queue->elements && "[ERROR] Memory allocation failed");
    }

    // enqueue element into queue
    memcpy(queue->elements + (next_index * element_size), element, element_size);
    queue->size++;
}

void gnormque_peek(const quegnorm_s queue, void * element, const size_t element_size) {
    QUEGNORM_ASSERT(queue.size && "[ERROR] Can't peek empty queue.");
    QUEGNORM_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    QUEGNORM_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    // copy start element into parameter
    memcpy(element, queue.elements + (queue.current * element_size), element_size);
}

void gnormque_dequeue(quegnorm_s * queue, void * element, const size_t element_size) {
    QUEGNORM_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUEGNORM_ASSERT(queue->size && "[ERROR] Can't peep empty queue.");
    QUEGNORM_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    QUEGNORM_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, queue->elements + (queue->current * element_size), element_size);
    queue->size--;
    queue->current++;

    if (!(IS_CAPACITY_QUEGNORM(queue->size))) {
        return;
    }

    queue->current = 0;
    if (queue->size) {
        queue->elements = QUEGNORM_REALLOC(queue->elements, element_size * queue->size);
    } else {
        QUEGNORM_FREE(queue->elements);
        queue->elements = NULL;
    }
}

void gnormque_foreach(quegnorm_s const * queue, const operate_fn operate, const size_t element_size, void * arguments) {
    QUEGNORM_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUEGNORM_ASSERT(operate && "[ERROR] 'operate' parameter is NULL.");
    QUEGNORM_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    char * elements = queue->elements + (queue->current * element_size);
    for (size_t i = 0; i < queue->size && operate(elements, element_size, arguments); ++i) {
        elements += element_size;
    }
}

void gnormque_map(quegnorm_s const * queue, const manage_fn manage, const size_t element_size, void * arguments) {
    QUEGNORM_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUEGNORM_ASSERT(manage && "[ERROR] 'manage' parameter is NULL.");
    QUEGNORM_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    manage(queue->elements + (queue->current * element_size), queue->size, element_size, arguments);
}
