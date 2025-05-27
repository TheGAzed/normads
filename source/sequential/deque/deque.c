#include <sequential/deque/deque.h>

#include <string.h>

#ifndef DEQ_ASSERT
#   include <assert.h>
#   define DEQ_ASSERT assert
#endif

#if !defined(DEQ_REALLOC) && !defined(DEQ_FREE)
#   include <stdlib.h>
#   ifndef DEQ_REALLOC
#       define DEQ_REALLOC realloc
#   endif
#   ifndef DEQ_FREE
#       define DEQ_FREE free
#   endif
#elif !defined(DEQ_REALLOC)
#   error Reallocator macro is not defined!
#elif !defined(DEQ_FREE)
#   error Free macro is not defined!
#endif

#if !defined(IS_CAPACITY_DEQ) && !defined(EXPAND_CAPACITY_DEQ)
#   ifndef REALLOC_DEQ_CHUNK
#       define REALLOC_DEQ_CHUNK (1 << 5)
#   elif REALLOC_DEQ_CHUNK <= 0
#       error 'REALLOC_DEQ_CHUNK' cannot be less than or equal to 0
#   endif
#   define IS_CAPACITY_DEQ(size) (!((size) % REALLOC_DEQ_CHUNK)) // Checks if size has reached capacity.
#   define EXPAND_CAPACITY_DEQ(capacity) ((capacity) + REALLOC_DEQ_CHUNK) // Calculates next deque's capacity.
#elif !defined(IS_CAPACITY_DEQ)
#   error Is capacity reached check is not defined.
#elif !defined(EXPAND_CAPACITY_DEQ)
#   error Expand capacity size is not defined.
#endif

inline deque_s deq_create(void) {
    return (deque_s) { 0 };
}

void deq_destroy(deque_s * deque, const destroy_fn destroy, const size_t element_size) {
    DEQ_ASSERT(deque && "[ERROR] 'deque' parameter is NULL.");
    DEQ_ASSERT(destroy && "[ERROR] 'destroy' parameter is NULL.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    // destroy elements from current index until capacity
    char * elements = deque->elements + (deque->current * element_size);
    const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
    for (size_t i = 0; i < right_size; ++i) {
        destroy(elements);
        elements += element_size;
    }

    // reset elements array to the beginning and subtract removed size from deque's and destroy the rest
    elements = deque->elements;
    for (size_t i = 0; i < deque->size - right_size; ++i) {
        destroy(elements);
        elements += element_size;
    }

    deque->size = deque->current = deque->capacity = 0;
    DEQ_FREE(deque->elements);
    deque->elements = NULL;
}

deque_s deq_copy(const deque_s deque, const copy_fn copy, const size_t element_size) {
    DEQ_ASSERT(copy && "[ERROR] 'copy' parameter is NULL.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    deque_s deque_copy = { 0 };

    char const * elements = deque.elements + (deque.current * element_size);
    const size_t right_size = (deque.current + deque.size) > deque.capacity ? deque.capacity - deque.current : deque.size;
    for (size_t i = 0; i < right_size; ++i, deque_copy.size++) {
        if (IS_CAPACITY_DEQ(deque_copy.size)) {
            const size_t expand = deque_copy.capacity = EXPAND_CAPACITY_DEQ(deque_copy.size);

            deque_copy.elements = DEQ_REALLOC(deque_copy.elements, element_size * expand);
            DEQ_ASSERT(deque_copy.elements && "[ERROR] Memory allocation failed.");
        }

        copy(deque_copy.elements + (deque_copy.size * element_size), elements, element_size);
        elements += element_size;
    }

    elements = deque.elements;
    for (size_t i = 0; i < deque.size - deque_copy.size; ++i) {
        if (IS_CAPACITY_DEQ(deque_copy.size)) {
            const size_t expand = deque_copy.capacity = EXPAND_CAPACITY_DEQ(deque_copy.size);

            deque_copy.elements = DEQ_REALLOC(deque_copy.elements, element_size * expand);
            DEQ_ASSERT(deque_copy.elements && "[ERROR] Memory allocation failed.");
        }

        copy(deque_copy.elements + (deque_copy.size * element_size), elements, element_size);
        elements += element_size;
    }

    return deque_copy;
}

bool deq_is_full(const deque_s deque) {
    return !(~deque.size);
}

bool deq_is_empty(const deque_s deque) {
    return !(deque.size);
}

void deq_enqueue_front(deque_s * deque, const void * element, const size_t element_size) {
    DEQ_ASSERT(deque && "[ERROR] 'deque' parameter is NULL.");
    DEQ_ASSERT(!(~deque->size) && "[ERROR] Queue size variable will overflow.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    if (IS_CAPACITY_DEQ(deque->size)) {
        const size_t expand = EXPAND_CAPACITY_DEQ(deque->size);

        void * temporary = DEQ_REALLOC(NULL, element_size * expand);
        DEQ_ASSERT(deque->elements && "[ERROR] Memory allocation failed.");

        const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
        memcpy(temporary, deque->elements + (deque->current * element_size), right_size * element_size);

        const size_t left_size = deque->size - right_size;
        memcpy(temporary + (right_size * element_size), deque->elements, left_size * element_size);

        DEQ_FREE(deque->elements);
        deque->elements = temporary;
        deque->capacity = expand;
        deque->current = 0;
    }

    const size_t next_front_index = (deque->current + deque->size) % deque->capacity;
    memcpy(deque->elements + (next_front_index * element_size), element, element_size);
    deque->size++;
}

void deq_enqueue_rear(deque_s * deque, const void * element, const size_t element_size) {
    DEQ_ASSERT(deque && "[ERROR] 'deque' parameter is NULL.");
    DEQ_ASSERT(!(~deque->size) && "[ERROR] Queue size variable will overflow.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    if (IS_CAPACITY_DEQ(deque->size)) {
        const size_t expand = EXPAND_CAPACITY_DEQ(deque->size);

        void * temporary = DEQ_REALLOC(NULL, element_size * expand);
        DEQ_ASSERT(deque->elements && "[ERROR] Memory allocation failed.");

        const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
        memcpy(temporary, deque->elements + (deque->current * element_size), right_size * element_size);

        const size_t left_size = deque->size - right_size;
        memcpy(temporary + (right_size * element_size), deque->elements, left_size * element_size);

        DEQ_FREE(deque->elements);
        deque->elements = temporary;
        deque->capacity = expand;
        deque->current = 0;
    }

    if (!deque->current) {
        deque->current = deque->capacity;
    }
    deque->current--;
    memcpy(deque->elements + (deque->current * element_size), element, element_size);
    deque->size++;
}

void deq_peek_front(const deque_s deque, void * element, const size_t element_size) {
    DEQ_ASSERT(deque.size && "[ERROR] Can't peek empty deque.");
    DEQ_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    const size_t current_front_index = (deque.current + deque.size - 1) % deque.capacity;
    memcpy(element, deque.elements + (current_front_index * element_size), element_size);
}

void deq_peek_rear(const deque_s deque, void * element, const size_t element_size) {
    DEQ_ASSERT(deque.size && "[ERROR] Can't peek empty deque.");
    DEQ_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, deque.elements + (deque.current * element_size), element_size);
}

void deq_dequeue_front(deque_s * deque, void * element, const size_t element_size) {
    DEQ_ASSERT(deque && "[ERROR] 'deque' parameter is NULL.");
    DEQ_ASSERT(deque->size && "[ERROR] Can't peep empty deque.");
    DEQ_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    const size_t current_front_index = (deque->current + deque->size - 1) % deque->capacity;
    memcpy(element, deque->elements + (current_front_index * element_size), element_size);
    deque->size--;

    if (!(IS_CAPACITY_DEQ(deque->size))) {
        return;
    }

    if (deque->size) {
        void * temporary = DEQ_REALLOC(NULL, element_size * deque->size);
        DEQ_ASSERT(deque->elements && "[ERROR] Memory allocation failed.");

        const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
        memcpy(temporary, deque->elements + (deque->current * element_size), right_size * element_size);

        const size_t left_size = deque->size - right_size;
        memcpy(temporary + (right_size * element_size), deque->elements, left_size * element_size);

        DEQ_FREE(deque->elements);
        deque->elements = temporary;
        deque->capacity = deque->size;
        deque->current = 0;
    } else {
        DEQ_FREE(deque->elements);
        deque->elements = NULL;
    }
}

void deq_dequeue_rear(deque_s * deque, void * element, const size_t element_size) {
    DEQ_ASSERT(deque && "[ERROR] 'deque' parameter is NULL.");
    DEQ_ASSERT(deque->size && "[ERROR] Can't peep empty deque.");
    DEQ_ASSERT(element && "[ERROR] 'element' parameter is NULL.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    memcpy(element, deque->elements + (deque->current * element_size), element_size);
    deque->capacity++;
    deque->size--;
    if (deque->capacity == deque->capacity) {
        deque->current = 0;
    }

    if (!(IS_CAPACITY_DEQ(deque->size))) {
        return;
    }

    if (deque->size) {
        void * temporary = DEQ_REALLOC(NULL, element_size * deque->size);
        DEQ_ASSERT(deque->elements && "[ERROR] Memory allocation failed.");

        const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
        memcpy(temporary, deque->elements + (deque->current * element_size), right_size * element_size);

        const size_t left_size = deque->size - right_size;
        memcpy(temporary + (right_size * element_size), deque->elements, left_size * element_size);

        DEQ_FREE(deque->elements);
        deque->elements = temporary;
        deque->capacity = deque->size;
        deque->current = 0;
    } else {
        DEQ_FREE(deque->elements);
        deque->elements = NULL;
    }
}

void deq_foreach(deque_s const * deque, const operate_fn operate, const size_t element_size, void * arguments) {
    DEQ_ASSERT(deque && "[ERROR] 'deque' parameter is NULL.");
    DEQ_ASSERT(operate && "[ERROR] 'operate' parameter is NULL.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    char * elements = deque->elements + (deque->current * element_size);
    const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
    for (size_t i = 0; i < right_size && operate(elements, element_size, arguments); ++i) {
        elements += element_size;
    }

    elements = deque->elements;
    for (size_t i = 0; i < deque->size - right_size && operate(elements, element_size, arguments); ++i) {
        elements += element_size;
    }
}

void deq_map(deque_s const * deque, const manage_fn manage, const size_t element_size, void * arguments) {
    DEQ_ASSERT(deque && "[ERROR] 'deque' parameter is NULL.");
    DEQ_ASSERT(manage && "[ERROR] 'manage' parameter is NULL.");
    DEQ_ASSERT(element_size && "[ERROR] Element's size can't be zero.");

    // create temporary array to have elements in order
    void * elements_array = REALLOC_DEQ_CHUNK(NULL, deque->size * element_size);
    DEQ_ASSERT((!(deque->size) || elements_array) && "[ERROR] Memory allocation failed.");

    // copy deque's elements into temporary array
    char * elements = deque->elements + (deque->current * element_size);
    const size_t right_size = (deque->current + deque->size) > deque->capacity ? deque->capacity - deque->current : deque->size;
    memcpy(elements_array, elements, right_size * element_size);
    memcpy(elements_array + (right_size * element_size), deque->elements, deque->size - right_size);

    manage(elements_array, deque->size, element_size, arguments);

    // copy managed elements back into deque
    memcpy(elements, elements_array, right_size * element_size);
    memcpy(deque->elements, elements_array + (right_size * element_size), deque->size - right_size);

    DEQ_FREE(elements_array);
}
