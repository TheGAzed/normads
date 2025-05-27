#include <sequential/queue/queue.h>

#include <string.h>

#ifndef QUE_ASSERT
#   include <assert.h>
#   define QUE_ASSERT assert
#endif

#if !defined(QUE_REALLOC) && !defined(QUE_FREE)
#   include <stdlib.h>
#   ifndef QUE_REALLOC
#       define QUE_REALLOC realloc
#   endif
#   ifndef QUE_FREE
#       define QUE_FREE free
#   endif
#elif !defined(QUE_REALLOC)
#   error Reallocator macro is not defined!
#elif !defined(QUE_FREE)
#   error Free macro is not defined!
#endif

#if !defined(IS_CAPACITY_QUE) && !defined(EXPAND_CAPACITY_QUE)
#   ifndef REALLOC_QUE_CHUNK
#       define REALLOC_QUE_CHUNK (1 << 5)
#   elif REALLOC_QUE_CHUNK <= 0
#       error 'REALLOC_QUE_CHUNK' cannot be less than or equal to 0
#   endif
#   define IS_CAPACITY_QUE(size) (!((size) % REALLOC_QUE_CHUNK)) // Checks if size has reached capacity.
#   define EXPAND_CAPACITY_QUE(capacity) ((capacity) + REALLOC_QUE_CHUNK) // Calculates next queue's capacity.
#elif !defined(IS_CAPACITY_QUE)
#   error Is capacity reached check is not defined.
#elif !defined(EXPAND_CAPACITY_QUE)
#   error Expand capacity size is not defined.
#endif

queue_s que_create(void) {
    return (queue_s) { 0 };
}

void que_destroy(queue_s * queue, const destroy_fn destroy, const size_t element_size) {
    QUE_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUE_ASSERT(destroy && "[ERROR] 'destroy' parameter is NULL.");
    QUE_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    // while queue isn't empty
    for (char * elements = queue->elements + (queue->current * element_size); queue->size; queue->size--) {
        destroy(elements); // destroy single element
        elements += element_size; // go to next element
    }

    // free elements array
    QUE_FREE(queue->elements);
    queue->elements = NULL;
}

queue_s que_copy(const queue_s queue, const copy_fn copy, const size_t element_size) {
    QUE_ASSERT(copy && "[ERROR] 'copy' parameter is NULL.");
    QUE_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    queue_s queue_copy = { 0 }; // create temporary copy to return

    char const * elements = queue.elements + (queue.current * element_size); // save elements array as char pointer
    for (queue_copy.size = 0; queue_copy.size < queue.size; queue_copy.size++) {
        if (IS_CAPACITY_QUE(queue_copy.size)) { // if queue copy reaches capacity expand it
            const size_t expand = EXPAND_CAPACITY_QUE(queue_copy.size);

            queue_copy.elements = QUE_REALLOC(queue_copy.elements, element_size * expand);
            QUE_ASSERT(queue_copy.elements && "[ERROR] Memory allocation failed");
        }
        // copy each queue's element into its copy
        copy(queue_copy.elements + (queue_copy.size * element_size), elements, element_size);

        elements += element_size; // go to next element
    }

    return queue_copy; // return initialized queue copy
}

bool que_is_full(const queue_s queue) {
    return !(~queue.size); // check if queue's size has all bits set to one
}

bool que_is_empty(const queue_s queue) {
    return !(queue.size); // check if queue's size is zero
}

void que_enqueue(queue_s * queue, const void * element, const size_t element_size) {
    QUE_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUE_ASSERT(!(~queue->size) && "[ERROR] Queue size variable will overflow.");
    QUE_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    const size_t next_index = queue->current + queue->size;
    if (IS_CAPACITY_QUE(next_index)) { // if capacity is reached expand queue's elements array
        const size_t expand = EXPAND_CAPACITY_QUE(next_index);

        queue->elements = QUE_REALLOC(queue->elements, element_size * expand);
        QUE_ASSERT(queue->elements && "[ERROR] Memory allocation failed");
    }

    // enqueue element into queue
    memcpy(queue->elements + (next_index * element_size), element, element_size);
    queue->size++;
}

void que_peek(const queue_s queue, void * element, const size_t element_size) {
    QUE_ASSERT(queue.size && "[ERROR] Can't peek empty queue.");
    QUE_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    QUE_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    // copy start element into parameter
    memcpy(element, queue.elements + (queue.current * element_size), element_size);
}

void que_dequeue(queue_s * queue, void * element, const size_t element_size) {
    QUE_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUE_ASSERT(queue->size && "[ERROR] Can't peep empty queue.");
    QUE_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    QUE_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, queue->elements + (queue->current * element_size), element_size);
    queue->size--;
    queue->current++;

    if (!(IS_CAPACITY_QUE(queue->size))) {
        return;
    }

    queue->current = 0;
    if (queue->size) {
        queue->elements = QUE_REALLOC(queue->elements, element_size * queue->size);
    } else {
        QUE_FREE(queue->elements);
        queue->elements = NULL;
    }
}

void que_foreach(queue_s const * queue, const operate_fn operate, const size_t element_size, void * arguments) {
    QUE_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUE_ASSERT(operate && "[ERROR] 'operate' parameter is NULL.");
    QUE_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    char * elements = queue->elements + (queue->current * element_size);
    for (size_t i = 0; i < queue->size && operate(elements, element_size, arguments); ++i) {
        elements += element_size;
    }
}

void que_map(queue_s const * queue, const manage_fn manage, const size_t element_size, void * arguments) {
    QUE_ASSERT(queue && "[ERROR] 'queue' parameter is NULL.");
    QUE_ASSERT(manage && "[ERROR] 'manage' parameter is NULL.");
    QUE_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    manage(queue->elements + (queue->current * element_size), queue->size, element_size, arguments);
}
