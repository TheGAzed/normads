#include <scale/sequential/queue/squeue.h>

#include <string.h>

#ifndef ASSERT_SQUE
#   include <assert.h>
#   define ASSERT_SQUE assert
#endif

#if !defined(REALLOC_SQUE) && !defined(FREE_SQUE)
#   include <stdlib.h>
#   ifndef REALLOC_SQUE
#       define REALLOC_SQUE realloc
#   endif
#   ifndef FREE_SQUE
#       define FREE_SQUE free
#   endif
#elif !defined(REALLOC_SQUE)
#   error Reallocator macro is not defined!
#elif !defined(FREE_SQUE)
#   error Free macro is not defined!
#endif

#if !defined(IS_CAPACITY_SQUE) && !defined(EXPAND_CAPACITY_SQUE)
#   ifndef REALLOC_CHUNK_SQUE
#       define REALLOC_CHUNK_SQUE (1 << 5)
#   elif REALLOC_CHUNK_SQUE <= 0
#       error 'REALLOC_CHUNK_SQUE' cannot be less than or equal to 0
#   endif
#   define IS_CAPACITY_SQUE(size) (!((size) % REALLOC_CHUNK_SQUE)) // Checks if size has reached capacity.
#   define EXPAND_CAPACITY_SQUE(capacity) ((capacity) + REALLOC_CHUNK_SQUE) // Calculates next queue's capacity.
#elif !defined(IS_CAPACITY_SQUE)
#   error Is capacity reached check is not defined.
#elif !defined(EXPAND_CAPACITY_SQUE)
#   error Expand capacity size is not defined.
#endif

squeue_s sque_create(void) {
    return (squeue_s) { 0 };
}

void sque_destroy(squeue_s * queue, const destroy_fn destroy, const size_t element_size) {
    ASSERT_SQUE(queue && "[ERROR] 'queue' parameter is NULL.");
    ASSERT_SQUE(destroy && "[ERROR] 'destroy' parameter is NULL.");
    ASSERT_SQUE(element_size && "[ERROR] Element's size can't be zero.");

    // while queue isn't empty
    for (char * elements = (char*)(queue->elements) + (queue->current * element_size); queue->size; queue->size--) {
        destroy(elements); // destroy single element
        elements += element_size; // go to next element
    }

    // free elements array
    FREE_SQUE(queue->elements);
    queue->elements = NULL;
}

squeue_s sque_copy(const squeue_s queue, const copy_fn copy, const size_t element_size) {
    ASSERT_SQUE(copy && "[ERROR] 'copy' parameter is NULL.");
    ASSERT_SQUE(element_size && "[ERROR] Element's size can't be zero.");

    squeue_s queue_copy = { 0 }; // create temporary copy to return

    char const * elements = (char*)queue.elements + (queue.current * element_size); // save elements array as char pointer
    for (queue_copy.size = 0; queue_copy.size < queue.size; queue_copy.size++) {
        if (IS_CAPACITY_SQUE(queue_copy.size)) { // if queue copy reaches capacity expand it
            const size_t expand = EXPAND_CAPACITY_SQUE(queue_copy.size);

            queue_copy.elements = REALLOC_SQUE(queue_copy.elements, element_size * expand);
            ASSERT_SQUE(queue_copy.elements && "[ERROR] Memory allocation failed");
        }
        // copy each queue's element into its copy
        copy((char*)queue_copy.elements + (queue_copy.size * element_size), elements, element_size);

        elements += element_size; // go to next element
    }

    return queue_copy; // return initialized queue copy
}

bool sque_is_full(const squeue_s queue) {
    return !(~queue.size); // check if queue's size has all bits set to one
}

bool sque_is_empty(const squeue_s queue) {
    return !(queue.size); // check if queue's size is zero
}

void sque_enqueue(squeue_s * queue, const void * element, const size_t element_size) {
    ASSERT_SQUE(queue && "[ERROR] 'queue' parameter is NULL.");
    ASSERT_SQUE((~queue->size) && "[ERROR] Queue size variable will overflow.");
    ASSERT_SQUE(element_size && "[ERROR] Element's size can't be zero.");

    const size_t next_index = queue->current + queue->size;
    if (IS_CAPACITY_SQUE(next_index)) { // if capacity is reached expand queue's elements array
        const size_t expand = EXPAND_CAPACITY_SQUE(next_index);

        queue->elements = REALLOC_SQUE(queue->elements, element_size * expand);
        ASSERT_SQUE(queue->elements && "[ERROR] Memory allocation failed");
    }

    // enqueue element into queue
    memcpy((char*)queue->elements + (next_index * element_size), element, element_size);
    queue->size++;
}

void sque_peek(const squeue_s queue, void * element, const size_t element_size) {
    ASSERT_SQUE(queue.size && "[ERROR] Can't peek empty queue.");
    ASSERT_SQUE(element && "[ERROR] 'element' parameter is NULL.");
    ASSERT_SQUE(element_size && "[ERROR] Element's size can't be zero.");

    // copy start element into parameter
    memcpy(element, (char*)queue.elements + (queue.current * element_size), element_size);
}

void sque_dequeue(squeue_s * queue, void * element, const size_t element_size) {
    ASSERT_SQUE(queue && "[ERROR] 'queue' parameter is NULL.");
    ASSERT_SQUE(queue->size && "[ERROR] Can't peep empty queue.");
    ASSERT_SQUE(element && "[ERROR] 'element' parameter is NULL.");
    ASSERT_SQUE(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, (char*)queue->elements + (queue->current * element_size), element_size);
    queue->size--;
    queue->current++;

    if (!(IS_CAPACITY_SQUE(queue->size))) {
        return;
    }

    if (queue->size) {
        void * temporary = REALLOC_SQUE(NULL, element_size * queue->size);
        memcpy(temporary, (char*)queue->elements + (queue->current * element_size), queue->size * element_size);

        FREE_SQUE(queue->elements);
        queue->elements = temporary;
    } else {
        FREE_SQUE(queue->elements);
        queue->elements = NULL;
    }
    queue->current = 0;
}

void sque_foreach(squeue_s const * queue, const operate_fn operate, const size_t element_size, void * arguments) {
    ASSERT_SQUE(queue && "[ERROR] 'queue' parameter is NULL.");
    ASSERT_SQUE(operate && "[ERROR] 'operate' parameter is NULL.");
    ASSERT_SQUE(element_size && "[ERROR] Element's size can't be zero.");

    char * elements = (char*)queue->elements + (queue->current * element_size);
    for (size_t i = 0; i < queue->size && operate(elements, element_size, arguments); ++i) {
        elements += element_size;
    }
}

void sque_map(squeue_s const * queue, const manage_fn manage, const size_t element_size, void * arguments) {
    ASSERT_SQUE(queue && "[ERROR] 'queue' parameter is NULL.");
    ASSERT_SQUE(manage && "[ERROR] 'manage' parameter is NULL.");
    ASSERT_SQUE(element_size && "[ERROR] Element's size can't be zero.");

    manage((char*)queue->elements + (queue->current * element_size), queue->size, element_size, arguments);
}
